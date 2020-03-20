#include "httpobject.h"

HttpObject::HttpObject()
{

}

HttpObject::HttpObject(const QString &reqest)
{
    this->parseHttp(reqest);
}


QString HttpObject::lastError() const
{
    return this->error;
}

void HttpObject::parseRequestLine(const QString &requestLineString)
{
    QList<QString> requestLine = requestLineString.split(" ");
    if (requestLine.length() != 3){
        this->error = "INVALID HTTP: WRONG REQUEST LINE";
        return;
    }
    this->method = requestLine[0];
    this->URI = requestLine[1];
    this->HTTPVersion = requestLine[2];
}

void HttpObject::parseHeaders(QList<QString> &headersList)
{
    QList<QString> splitedHeader;
    qint32 index;
    for(QString& header:headersList){
        index = header.indexOf(":");
        if (index != -1){
            this->requestHeaders.insert(header.left(index).simplified(),
                                        header.right(header.length()- index - 1).simplified());

        }

    }
}

QString HttpObject::getHTTPVersion() const
{
    return HTTPVersion;
}

QString HttpObject::getURI() const
{
    return URI;
}

QString HttpObject::getMethod() const
{
    return method;
}

void HttpObject::parseHttp(const QString &request){
    qint32 index = request.indexOf("\r\n\r\n");
    QList<QString> requestHead;
    if (index == -1){
        index = request.indexOf("\n\n");
        if (index == -1){
            this->error = "INVALID HTTP";
            return;
        }
    }
    QString head = request.left(index);
    if (index < request.length()){
        this->requestBody = request.right(request.length()-index);
        if (this->requestBody.indexOf("\r\n") == -1){
            this->requestBody.remove(0,2);
        }
        else
        {
            this->requestBody.remove(0,4);
        }
    }
    if (head != ""){
        if (head.indexOf("\r\n") != -1){
            requestHead = head.split("\r\n");
        }
        else
        {
            requestHead = head.split("\n");
        }
    }
    else
    {
        this->error = "INVALID HTTP: NO HEAD";
    }

    parseRequestLine(requestHead[0]);
    if (error != "")
        return;
    requestHead.removeFirst();
    parseHeaders(requestHead);
    if (error != "")
        return;

}

