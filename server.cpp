#include "server.h"

Server::Server(){}

Server::~Server(){}

void Server::startServer(){
    if (this->listen(QHostAddress::Any,port)){
        qDebug()<< "Listening";
    }else
    {
        qDebug()<<"Not listening";
    }

}


void Server::incomingConnection(qintptr handle){
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);
    connect (socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisconnect()));

    qDebug() << socketDescriptor()<< " client connected";
    socket->write("Hello from server");
}


void Server::sockReady(){
    if (socket->waitForConnected(500)){
        recievedData = socket->readAll();
        qDebug() << recievedData;
    }

}

void Server::sockDisconnect(){
    qDebug()<<"disconected";
    socket->deleteLater();
}
