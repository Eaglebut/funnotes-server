#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <database.h>

class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server();
    ~Server();
    void startServer();

private slots:

    void incomingConnection(qintptr handle) override;
    void sockReady();
    void sockDisconnect();
private:
    QTcpSocket* socket = nullptr;
    QByteArray recievedData;
    quint16 port = 8080;
    Database_interface db;

};

#endif // SERVER_H
