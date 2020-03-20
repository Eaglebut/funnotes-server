#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H


#include<QString>
#include<QMap>

class HttpRequest
{
public:
    HttpRequest();
    HttpRequest(const QString &reqest);

    void parseHttp(const QString &request);

    QString lastError() const;
    QString getMethod() const;
    QString getURI() const;
    QString getHTTPVersion() const;
    QMap<QString,QString> getHeaders() const;
    QString getHeaderValue(const QString &header) const;
    QString getBody() const;

private:
    void parseRequestLine(const QString &requestLineString);
    void parseHeaders(QList<QString> &headersList);

    QString error = "";

    QString method = "";
    QString URI = "";
    QString httpVersion = "";

    QMap<QString,QString> requestHeaders;
    QString requestBody;
};


#endif // HTTPREQUEST_H
