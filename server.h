#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server();
    ~Server();
public slots:
    void startServer();
    void incomingConnection(qintptr handle) override;
    void sockReady();
    void sockDisconnect();
private:
    QTcpSocket* socket = nullptr;
    QByteArray recievedData;
    quint16 port = 5555;
};

#endif // SERVER_H
