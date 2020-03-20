#ifndef HTTPOBJECT_H
#define HTTPOBJECT_H

#include<QString>
#include<QPair>
#include<QTextStream>
#include <QDebug>


#define OK_SERVER "200 OK"
#define HTTP_VERSION "HTTP/1.1"

class HttpObject
{
public:
    HttpObject();
    HttpObject(const QString &reqest);

    void parseHttp(const QString &request);

    QString lastError() const;
    QString getMethod() const;
    QString getURI() const;
    QString getHTTPVersion() const;

private:
    void parseRequestLine(const QString &requestLineString);
    void parseHeaders(QList<QString> &headersList);

    QString error = "";

    QString method = "";
    QString URI = "";
    QString HTTPVersion = "";

    QMap<QString,QString> requestHeaders;
    QString requestBody;
};

#endif // HTTPOBJECT_H
