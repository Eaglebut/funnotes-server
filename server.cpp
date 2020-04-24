#include "server.h"

#include <QNetworkInterface>


Server::Server(){
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
            qDebug() << address.toString();
    }
}

Server::~Server(){}

void Server::startServer(){

    db.connectDatabase();

    if (this->listen(QHostAddress("195.133.196.6"),port)){
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
    db.testDatabase();

}


void Server::sockReady(){

    HttpRequest request;
    HttpResponse response;
    recievedData = socket->readAll();
    qDebug() << recievedData;
    request.parseHttp(recievedData);
    response = manageRequest(request);
    qDebug() << response.getResponse();
    socket->write(response.getResponse().toUtf8());
    socket->disconnectFromHost();
}



void Server::sockDisconnect(){
    qDebug()<<"disconected";
    socket->deleteLater();
}

HttpResponse Server::manageGETUser(const QString userId)
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
    HttpResponse response = OK();
    response.setBody(doc.toJson(QJsonDocument::Compact));
    return response;
}

HttpResponse Server::manageGETEvent(const HttpRequest &request, QString userId)
{
    QJsonDocument doc = QJsonDocument::fromJson(request.getBody().toUtf8());
    if (doc.isObject()){
        QJsonObject obj = doc.object();
        QString eventId = QString::number( obj.value("id").toInt());
        QMap <QString,QString> event_map = db.getEvent(userId, eventId);
        if (event_map.keys().length() == 0)
            return forbidden();
        else{
            QJsonObject event;
            for (qint32 i = 0; i < event_map.size();i++){
                    event.insert(event_map.keys().at(i),event_map.values().at(i));
            }
            event.remove("type");
            doc.setObject(event);
            HttpResponse response = OK();
            response.setBody(doc.toJson(QJsonDocument::Compact));
            return response;
        }
    }
    else
        return badRequest();
}

HttpResponse Server::managePUTUser(const QString &login, const QString &password)
{
    if (db.addUser(login,password))
        return created();
    else
        return conflict();
}

HttpResponse Server::managePUTEvent(const HttpRequest &request, QString userId)
{
    QJsonDocument doc = QJsonDocument::fromJson(request.getBody().toUtf8());
    if (doc.isObject()){
        QJsonObject obj = doc.object();
        QString eventId = db.addEvent(userId,
                                      QString::number(obj.value("start_time").toInt()),
                                      QString::number(obj.value("end_time").toInt()),
                                      obj.value("title").toString(),
                                      obj.value("description").toString());
        if (eventId != ""){
            QJsonObject obj;
            obj.insert("id",eventId);
            doc.setObject(obj);
            HttpResponse response = created();
            response.setBody(doc.toJson(QJsonDocument::Compact));
            return response;
        }
        else return badRequest();
    }
    else
        return badRequest();
}

HttpResponse Server::managePOSTUser(const HttpRequest &request,const QString &login,const QString &password)
{
    QJsonDocument doc = QJsonDocument::fromJson(request.getBody().toUtf8());
    if (doc.isObject()){
        QJsonObject obj = doc.object();
        if(db.changeUserPassword(login,password,obj.value("password").toString()))
            return OK();
        else return badRequest();
    }
    else
        return badRequest();
}

HttpResponse Server::managePOSTEvent(const HttpRequest &request, QString userId)
{
    QJsonDocument doc = QJsonDocument::fromJson(request.getBody().toUtf8());
    if (doc.isObject()){
        QJsonObject obj = doc.object();
        if( db.modifyEvent(userId,
                           obj.value("id").toString(),
                           obj.value("start_time").toString(),
                           obj.value("end_time").toString(),
                           obj.value("title").toString(),
                           obj.value("description").toString()))
            return OK();
        else
            return badRequest();
    }
    else
        return badRequest();

}

HttpResponse Server::manageDELETEEvent(const HttpRequest &request, QString userId)
{

    QJsonDocument doc = QJsonDocument::fromJson(request.getBody().toUtf8());
    if (doc.isObject()){
        QJsonObject obj = doc.object();
        QString eventId = QString::number( obj.value("id").toInt());
        if (db.deleteEvent(userId, eventId)){
            HttpResponse response = OK();
            response.setBody(doc.toJson(QJsonDocument::Compact));
            return response;
        }
        else
            return badRequest();
    }
    else
        return badRequest();
}

HttpResponse Server::badRequest()
{
    HttpResponse response;
    response.addHeader("Server",SERVER_NAME);
    response.addHeader("Date",QDateTime::currentDateTime().toUTC().toString("dd.MM.yyyy hh:mm:ss"));
    response.setStatusCode(BAD_REQUEST);
    response.setStatusText("Bad request");
    return response;
}

HttpResponse Server::forbidden()
{
    HttpResponse response;
    response.addHeader("Server",SERVER_NAME);
    response.addHeader("Date",QDateTime::currentDateTime().toUTC().toString("dd.MM.yyyy hh:mm:ss"));
    response.setStatusCode(FORBIDDEN);
    response.setStatusText("Forbidden");
    return response;
}

HttpResponse Server::OK()
{
    HttpResponse response;
    response.addHeader("Server",SERVER_NAME);
    response.addHeader("Date",QDateTime::currentDateTime().toUTC().toString("dd.MM.yyyy hh:mm:ss"));
    response.setStatusCode(CODE_OK);
    response.setStatusText("OK");
    return response;
}

HttpResponse Server::conflict()
{
    HttpResponse response;
    response.addHeader("Server",SERVER_NAME);
    response.addHeader("Date",QDateTime::currentDateTime().toUTC().toString("dd.MM.yyyy hh:mm:ss"));
    response.setStatusCode(CONFLICT);
    response.setStatusText("Conflict");
    return response;
}

HttpResponse Server::created()
{
    HttpResponse response;
    response.addHeader("Server",SERVER_NAME);
    response.addHeader("Date",QDateTime::currentDateTime().toUTC().toString("dd.MM.yyyy hh:mm:ss"));
    response.setStatusCode(CREATED);
    response.setStatusText("Created");
    return response;
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

    if (request.lastError() == ""){
        splitedURI = request.getURI().split("/");
        splitedURI.removeFirst();
        splitedPassword = request.getHeaderValue("Authorization").split(" ");
        login = splitedURI.takeFirst();
        password = splitedPassword.takeLast();
        userId = db.getUserId(login,password);
        if (request.getMethod() == "PUT" && userId == 0 && splitedURI.length() == 0)
            return managePUTUser(login, password);
        else if (userId != "")
        {
            if (request.getMethod() == "GET"){
                if (splitedURI.length() == 0)
                    return manageGETUser(userId);
                else if(splitedURI.takeLast() == "event")
                    return manageGETEvent(request,userId);
                else return badRequest();
            }
            else if (request.getMethod() == "PUT"){
                if(splitedURI.length()==0)
                    return conflict();
                else if(splitedURI.takeLast() == "event")
                    return managePUTEvent(request,userId);
                else return badRequest();
            }
            else if (request.getMethod() == "POST"){
                if (splitedURI.length() == 0)
                    return managePOSTUser(request,login,password);
                else if(splitedURI.takeLast() == "event")
                    return managePOSTEvent(request,userId);
                else return badRequest();
            }
            else if (request.getMethod() == "DELETE"){
                if (splitedURI.takeLast() == "event")
                    return manageDELETEEvent(request,userId);
                else return badRequest();
            }
            else
                return badRequest();
        }
        else
            return forbidden();
    }
    else
        return badRequest();
}
