#include "httprequest.h"



HttpRequest::HttpRequest()
{

}

HttpRequest::HttpRequest(const QString &reqest)
{
    this->parseHttp(reqest);
}


QString HttpRequest::lastError() const
{
    return this->error;
}

void HttpRequest::parseRequestLine(const QString &requestLineString)
{
    QList<QString> requestLine = requestLineString.split(" ");
    if (requestLine.length() != 3){
        this->error = "INVALID HTTP: WRONG REQUEST LINE";
        return;
    }
    this->method = requestLine[0];
    this->URI = requestLine[1];
    this->httpVersion = requestLine[2];
}

void HttpRequest::parseHeaders(QList<QString> &headersList)
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

QString HttpRequest::getBody() const
{
    return requestBody;
}

QString HttpRequest::getHTTPVersion() const
{
    return httpVersion;
}

QMap<QString, QString> HttpRequest::getHeaders() const
{
    return this->requestHeaders;
}

QString HttpRequest::getHeaderValue(const QString &header) const
{
    try {
        return this->requestHeaders.value(header);

    } catch (std::out_of_range &e) {
        return "";
    }
}

QString HttpRequest::getURI() const
{
    return URI;
}

QString HttpRequest::getMethod() const
{
    return method;
}

void HttpRequest::parseHttp(const QString &request){
    this->error = "";
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
        return;
    }

    parseRequestLine(requestHead[0]);
    if (error != "")
        return;
    requestHead.removeFirst();
    parseHeaders(requestHead);
    if (error != "")
        return;

}



