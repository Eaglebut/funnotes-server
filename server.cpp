#include "server.h"


Server::Server(){}

Server::~Server(){}

void Server::startServer(){

    db.connectDatabase();
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

}


void Server::sockReady(){
    HttpRequest request;
    HttpResponse response;
    recievedData = socket->readAll();
    qDebug() << recievedData;
    request.parseHttp(recievedData);
    response = manageRequest(request);
    socket->write(response.getResponse().toUtf8());
    socket->disconnectFromHost();
}



void Server::sockDisconnect(){
    qDebug()<<"disconected";
    socket->deleteLater();
}

HttpResponse Server::browserRequest(const HttpRequest &request)
{
    HttpResponse response;
    response.setStatusCode(101);
    response.setStatusText("Switching Protocols");
    response.addHeader("Upgrade",request.getHeaderValue("Upgrade"));
    response.addHeader("Access-Control-Allow-Credentials","false");
    response.addHeader("Access-Control-Allow-Methods","GET");
    response.addHeader("Access-Control-Allow-Headers","content-type");
    response.addHeader("Connection","Upgrade");
    response.addHeader("Access-Control-Allow-Origin","*");

    QString key = request.getHeaderValue("Sec-WebSocket-Key");
    QString addition = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    QString returnKey = key + addition;
    QByteArray hash = QCryptographicHash::hash(returnKey.toUtf8(),QCryptographicHash::Sha1);
    response.addHeader("Sec-Websocket-Accept", QString::fromUtf8(hash.toBase64()));
    return response;
}

void Server::sendToBrowser(const QString &message)
{
    QByteArray messageArray;
    messageArray.append(this->startCodes.shortText);
    if (message.length() * sizeof(char) < 125)
        messageArray.append(message.length() * sizeof(char));
    else if (message.length() * sizeof (char) < 65536){
        messageArray.append(126);
        messageArray.append(message.length() * sizeof (char));
    }
    else
    {
        messageArray.append(127);
        messageArray.append(message.length() * sizeof (char));
    }

    messageArray.append(message);
    socket->write(messageArray);
}

QString Server::decodeMessage(const QByteArray &messageArr) //TODO later
{
    QString message;
    if (messageArr.at(0) == this->startCodes.shortText){
        if(bitOn( messageArr.at(1),0)){

        }
    }
    return "";
}

bool Server::bitOn(const char &ch, const qint16 &n)
{
    return bool((1 << n)  &  ch);
}

HttpResponse Server::manageRequest(const HttpRequest &request)
{
    HttpResponse response;
    QList <QString> splitedURI;
    QList <QString> splitedPassword;
    QString userId , login, password;
    response.addHeader("Server",SERVER_NAME);
    response.addHeader("Date",QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss t"));
    if (request.lastError() == ""){
        splitedURI = request.getURI().split("/");
        splitedURI.removeFirst();
        splitedPassword = request.getHeaderValue("Authorization").split(" ");
        login = splitedURI.takeFirst();
        password = splitedPassword.takeLast();
        userId = db.getUserId(login,password);
        if (userId != ""){
            if (request.getMethod() == "GET"){
                if (splitedURI.length() == 0)
                {
                    QVector<QMap <QString,QString>> events_vector = db.getUserEvents(userId);
                    QJsonArray events;
                    QJsonObject* event;
                    for (QMap<QString,QString> &event_map: events_vector){
                        event = new QJsonObject;
                        for (qint32 i = 0; i < event_map.size();i++){
                            if (event_map.keys().at(i) == "startTime"){
                                event->insert(event_map.keys().at(i),event_map.values().at(i).toInt());
                            }
                            else if (event_map.keys().at(i) == "endTime"){
                                event->insert(event_map.keys().at(i),event_map.values().at(i).toInt());
                            }
                            else
                                event->insert(event_map.keys().at(i),event_map.values().at(i));
                        }
                        event->remove("type");
                        events.push_back(*event);
                        delete event;
                    }
                    QJsonObject body;
                    body.insert("events",events);
                    QJsonDocument doc(body);
                    response.setBody(doc.toJson(QJsonDocument::Compact));
                    response.setStatusCode(OK);
                    response.setStatusText("OK");
                }
                else if (splitedURI.length() == 1){
                    QMap <QString,QString> event_map = db.getEvent(userId, splitedURI.takeLast());
                    QJsonObject event;
                    for (qint32 i = 0; i < event_map.size();i++){
                        if (event_map.keys().at(i) == "startTime"){
                            event.insert(event_map.keys().at(i),event_map.values().at(i).toInt());
                        }
                        else if (event_map.keys().at(i) == "endTime"){
                            event.insert(event_map.keys().at(i),event_map.values().at(i).toInt());
                        }
                        else
                            event.insert(event_map.keys().at(i),event_map.values().at(i));
                    }
                    event.remove("type");
                    QJsonDocument doc(event);
                    response.setBody(doc.toJson(QJsonDocument::Compact));
                    response.setStatusCode(OK);
                    response.setStatusText("OK");
                }
                else
                {
                    response.setStatusCode(BAD_REQUEST);
                    response.setStatusText("Bad Request");
                }
            }
            else
            {
                response.setStatusCode(BAD_REQUEST);
                response.setStatusText("Bad Request");
            }
        }
        else
        {
            response.setStatusCode(FORBIDDEN);
            response.setStatusText("Forbidden");
        }
    }
    else
    {
        response.setStatusCode(BAD_REQUEST);
        response.setStatusText("Bad Request");
    }
    return response;
}
