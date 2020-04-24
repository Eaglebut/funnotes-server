#include "database.h"

Database_interface::Database_interface(){
}

Database_interface::~Database_interface(){
    delete query;
}

void Database_interface::connectDatabase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("funnotes");
    db.setUserName("server");
    db.setPassword("server_pass");
    if(db.open())
    {
        qDebug() << "database connected";
    }
    else
    {
        qDebug() << "database not connected\n" << db.lastError() ;
    }
    qDebug() << db.isValid();
    query = new QSqlQuery(db);

}

bool Database_interface::authorise(QString username,QString password){
    QString request = "SELECT `id` FROM `users` WHERE `username`= ? AND `password` = ?";
    query->prepare(request);
    query->addBindValue(username);
    query->addBindValue(password);
    if ( query->exec()){
        return query->next();
    }
    else
    {
        qDebug() << query->lastError();
        return false;
    }
}

bool Database_interface::addUser(QString username,QString password){
    QString request = "INSERT INTO `users` (`id`, `username`, `password`) VALUES (NULL, ?, ?)";
    query->prepare(request);
    query->addBindValue(username);
    query->addBindValue(password);
    if (query->exec()){
        return true;
    }
    else
    {
        qDebug() << query->lastError();
        return false;
    }
}

QString Database_interface::getUserId(QString username,QString password){
    QString request = "SELECT `id` FROM `users` WHERE `username`= ? AND `password` = ?";
    query->prepare(request);
    query->addBindValue(username);
    query->addBindValue(password);
    if (query->exec()){
       if( query->next()){
           return query->value(0).toString();
       }
       else
           return "";
    }
    else
        return "";
}

bool Database_interface::changeUserPassword(QString username,QString password,QString newPassword){
    QString request = "UPDATE `users` SET `password` = ? WHERE `users`.`username` = ? AND `users`.`password` = ?";
    query->prepare(request);
    query->addBindValue(newPassword);
    query->addBindValue(username);
    query->addBindValue(password);
    return query->exec();
}

QString Database_interface::addEvent(QString userId,QString startTime,QString endTime,QString title,QString description){
    QString request = "INSERT INTO `events` (`id`, `userid`, `start_time`, `end_time`, `title`, `description`, `type`) VALUES (NULL, ?, FROM_UNIXTIME(?), FROM_UNIXTIME(?), ?, ?, 0)";
    query->prepare(request);
    query->addBindValue(userId);
    query->addBindValue(startTime);
    query->addBindValue(endTime);
    query->addBindValue(title);
    query->addBindValue(description);
    if (query->exec())
        return QString::number(query->lastInsertId().toInt());
    else
    {
        qDebug() << query->lastError();
        return "";
    }

}

bool Database_interface::modifyEventStartTime(QString userId,QString eventId, QString newStartTime){
    QString request = "UPDATE `events` SET `start_time` =  FROM_UNIXTIME("
            + newStartTime
            + ") WHERE `events`.`id` = "
            + eventId
            + " AND `events`.`userid` = "
            + userId;


    if (query->exec(request))
        return true;
    else
    {
        qDebug() << query->lastError();
        return false;
    }

}

bool Database_interface::modifyEventEndTime(QString userId,QString eventId, QString NewEndTime){
    QString request = "UPDATE `events` SET `end_time` =  FROM_UNIXTIME("
            + NewEndTime
            + ") WHERE `events`.`id` = "
            + eventId
            + " AND `events`.`userid` = "
            + userId;


    if (query->exec(request))
        return true;
    else
    {
        qDebug() << query->lastError();
        return false;
    }

}

bool Database_interface::modifyEventTitle(QString userId,QString eventId, QString NewTitle){
    QString request = "UPDATE `events` SET `title` = '"
            + NewTitle
            + "' WHERE `events`.`id` = "
            + eventId
            + " AND `events`.`userid` = "
            + userId;
    if (query->exec(request))
        return true;
    else
    {
        qDebug() << query->lastError();
        return false;
    }

}

bool Database_interface::modifyEventDescription(QString userId,QString eventId, QString NewEventDescription){
    QString request = "UPDATE `events` SET `description` = '"
            + NewEventDescription
            + "' WHERE `events`.`id` = "
            + eventId
            + " AND `events`.`userid` = "
            + userId;


    if (query->exec(request))
        return true;
    else
    {
        qDebug() << query->lastError();
        return false;
    }
}

bool Database_interface::modifyEvent(QString userId, QString eventId, QString startTime, QString endTime, QString title, QString description)
{
    QString request = "UPDATE `events` SET `start_time` = FROM_UNIXTIME("
            + startTime
            + "), `end_time` = FROM_UNIXTIME("
            + endTime
            + "), `title` = '"
            + title
            + "', `description` = '"
            + description
            + "' WHERE `events`.`id` = "
            + eventId
            + " AND `events`.`userid` = "
            + userId;
    if (query->exec(request))
        return true;
    else
    {
        qDebug() << query->lastError();
        return false;
    }
}

QMap <QString,QString> Database_interface::getEvent(QString userId,QString eventId)
{
    QMap <QString,QString> event;
    QString request = "SELECT * FROM `events` WHERE `userid` = '" + userId + "' "
            +"AND `id` = '" + eventId + "'";
    QDateTime datetime;
    QString time;

    if (query->exec(request))
    {
        if(query->next())
        {
            event.insert("id",query->value(ID).toString());
            time = query->value(START_TIME).toString();
            datetime = QDateTime::fromString(time,"yyyy-MM-ddThh:mm:ss");
            event.insert("startTime", QString::number(datetime.toSecsSinceEpoch()));

            time = query->value(END_TIME).toString();
            datetime = QDateTime::fromString(time,"yyyy-MM-ddThh:mm:ss");
            event.insert("endTime", QString::number(datetime.toSecsSinceEpoch()));

            event.insert("title",query->value(TITLE).toString());
            event.insert("description",query->value(DESCRIPTION).toString());
            event.insert("type",query->value(TYPE).toString());
        }
        else
            qDebug() << query->lastError();
    }
    else
        qDebug() << query->lastError();
    return event;
}

QVector<QMap <QString,QString>> Database_interface::getUserEvents(QString userId){
    QVector<QMap <QString,QString>> userEvents;
    QMap <QString,QString>* event;
    QString db_output;
    QString request = "SELECT * FROM `events` WHERE `userid` = '" + userId + "'";
    QString time;
    QDateTime datetime;
    if (query->exec(request))
        while (query->next()) {
            event = new QMap <QString,QString>;
            event->insert("id",query->value(EventParts::ID).toString());
            time = query->value(EventParts::START_TIME).toString();
            datetime = QDateTime::fromString(time,"yyyy-MM-ddThh:mm:ss");
            event->insert("startTime", QString::number(datetime.toSecsSinceEpoch()));

            time = query->value(EventParts::END_TIME).toString();
            datetime = QDateTime::fromString(time,"yyyy-MM-ddThh:mm:ss");
            event->insert("endTime", QString::number(datetime.toSecsSinceEpoch()));
            event->insert("title",query->value(EventParts::TITLE).toString());
            event->insert("description",query->value(EventParts::DESCRIPTION).toString());
            event->insert("type",query->value(EventParts::TYPE).toString());
            userEvents.push_back(*event);
            delete event;
        }
    else{

        qDebug() << query->lastError();
    }
    return userEvents;

}
bool Database_interface::deleteEvent(QString userId,QString eventId){
    QString request = "DELETE FROM `events` WHERE `events`.`id` ='" +eventId + "'"
            +"AND `events`.`userid` = '" + userId +"'";
    if (query->exec(request))
        return true;
    else
    {
        query->clear();
        qDebug() << query->lastError();
        return false;
    }
}

void Database_interface::testDatabase()
{
    qDebug() << db.lastError();
    qDebug() << db.isOpen();
    qDebug() << db.isValid();
    qDebug() << db.isOpenError();
}


