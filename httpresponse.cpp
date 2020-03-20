#include "httpresponse.h"


HttpResponse::HttpResponse()
{

}

HttpResponse::HttpResponse(const QString &httpVersion, const quint16 &statusCode, const QString &statusText, const QMap<QString, QString> &responseHeaders, const QString &responseBody)
{
    this->httpVersion = httpVersion.simplified();
    this->statusCode = statusCode;
    this->statusText = statusText.simplified();
    this->responseHeaders = responseHeaders;
    this->responseBody = responseBody;
}

HttpResponse::HttpResponse(const quint16 &statusCode, const QString &statusText, const QMap<QString, QString> &responseHeaders, const QString &responseBody)
{
    this->statusCode = statusCode;
    this->statusText = statusText.simplified();
    this->responseHeaders = responseHeaders;
    this->responseBody = responseBody;
}

HttpResponse::HttpResponse(const quint16 &statusCode, const QString &statusText, const QMap<QString, QString> &responseHeaders)
{
    this->statusCode = statusCode;
    this->statusText = statusText.simplified();
    this->responseHeaders = responseHeaders;
}

QString HttpResponse::getHttpVersion() const
{
    return httpVersion;
}

void HttpResponse::setHttpVersion(const QString &value)
{
    httpVersion = value.simplified();
}

quint16 HttpResponse::getStatusCode() const
{
    return statusCode;
}

void HttpResponse::setStatusCode(const quint16 &value)
{
    statusCode = value;
}

QString HttpResponse::getStatusText() const
{
    return statusText.simplified();
}

void HttpResponse::setStatusText(const QString &value)
{
    statusText = value;
}

QMap<QString, QString> HttpResponse::getHeaders() const
{
    return responseHeaders;
}

void HttpResponse::setHeaders(const QMap<QString, QString> &value)
{
    responseHeaders = value;
}

void HttpResponse::addHeader(const QString &key, const QString &value)
{
    this->responseHeaders.insert(key.simplified(),value.simplified());
}

void HttpResponse::deleteHeader(const QString &key)
{
    this->responseHeaders.remove(key.simplified());
}

QString HttpResponse::getBody() const
{
    return responseBody;
}

void HttpResponse::setBody(const QString &value)
{
    responseBody = value.simplified();
}

QString HttpResponse::getResponse()
{
    QString responseString = "";
    if (this->httpVersion == ""){
        this->error = "NO HTTP VERSION";
        return "";
    }
    responseString += this->httpVersion;

    responseString += " ";
    if (this->statusCode == 0){
        this->error = "NO STATUS CODE";
        return "";
    }
    responseString += QString::number(this->statusCode);

    responseString += " ";
    if (this->statusText == ""){
        this->error = "NO STATUS TEXT";
        return "";
    }
    responseString += this->statusText;
    responseString += "\r\n";



    for(qint32 i = 0; i < this->responseHeaders.size() ;i++){
        responseString += this->responseHeaders.keys()[i];
        responseString += ": ";
        responseString += this->responseHeaders.values()[i];
        responseString += "\r\n";
    }
    responseString += "\r\n";
    responseString += responseBody;

    return responseString;
}




