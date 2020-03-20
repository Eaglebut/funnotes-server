#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QString>
#include <QMap>

class HttpResponse
{
public:
    HttpResponse();
    HttpResponse(const QString &httpVersion,
                 const quint16 &statusCode,
                 const QString &statusText,
                 const QMap<QString,QString> &responseHeaders,
                 const QString &responseBody);
    HttpResponse(const quint16 &statusCode,
                 const QString &statusText,
                 const QMap<QString,QString> &responseHeaders,
                 const QString &responseBody);
    HttpResponse(const quint16 &statusCode,
                 const QString &statusText,
                 const QMap<QString,QString> &responseHeaders);


    QString getHttpVersion() const;
    void setHttpVersion(const QString &value);

    quint16 getStatusCode() const;
    void setStatusCode(const quint16 &value);

    QString getStatusText() const;
    void setStatusText(const QString &value);

    QMap<QString, QString> getHeaders() const;
    void setHeaders(const QMap<QString, QString> &value);
    QString getHeader(const QString &key) const;
    void addHeader (const QString &key,const QString &value);
    void deleteHeader (const QString &key);

    QString getBody() const;
    void setBody(const QString &value);

    QString getResponse();//TODO

private:
    QString httpVersion = "HTTP/1.1";
    quint16 statusCode = 0;
    QString statusText = "";

    QMap<QString,QString> responseHeaders;
    QString responseBody;

    QString error =  "";
};



#endif // HTTPRESPONSE_H
