#ifndef SERVER_H
#define SERVER_H

#define SERVER_NAME "FunNotes v0.0.1"

#include <QTcpServer>
#include <QTcpSocket>
#include <database.h>
#include "httprequest.h"
#include "httpresponse.h"
#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server();
    ~Server();
    void startServer();
    HttpResponse manageRequest(const HttpRequest &request);

private slots:

    void incomingConnection(qintptr handle) override;
    void sockReady();
    void sockDisconnect();
private:
    QTcpSocket* socket = nullptr;
    QByteArray recievedData;
    quint16 port = 8080;
    Database_interface db;

    HttpResponse browserRequest (const HttpRequest &arr);
    void sendToBrowser(const QString &message);
    QString decodeMessage(const QByteArray &messageArr);
    bool bitOn(const char &ch,const qint16 &n);


    struct startCodes{
        char shortText = 0x81;
        char shortBinary = 0x82;
        char CloseConnection = 0x88;
        char shortPing = 0x89;
        char shortPong = 0x8A;
    };

    startCodes startCodes;

    enum statusCodes{
        OK = 200,
        BAD_REQUEST = 400,
        FORBIDDEN = 403,
        CREATED = 201,
        CONFLICT = 409
    };


};

#endif // SERVER_H
