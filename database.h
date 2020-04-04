#ifndef MYSQL_H
#define MYSQL_H
#include <QSqlError>
#include<QSqlDatabase>
#include<QString>
#include<QVector>
#include<QDebug>
#include<QSqlQuery>
#include<QTextStream>
#include<QVariant>
#include<QDateTime>
#include<QMap>

class Database_interface
{
public:
    Database_interface();
    ~Database_interface();

    void connectDatabase();
    bool authorise(QString username,QString password);
    bool addUser(QString username,QString password);
    QString getUserId(QString username,QString password);
    bool changeUserPassword(QString username,QString password,QString newPassword);
    QString addEvent(QString userId,QString startTime,QString endTime,QString title,QString description);
    bool modifyEventStartTime(QString userId,QString eventId, QString NewStartTime);
    bool modifyEventEndTime(QString userId,QString eventId, QString NewEndTime);
    bool modifyEventTitle(QString userId,QString eventId, QString NewTitle);
    bool modifyEventDescription(QString userId,QString eventId, QString NewEventDescription);
    bool modifyEvent(QString userId,QString eventId,QString startTime,QString endTime,QString title,QString description);
    QMap <QString,QString> getEvent(QString userId,QString eventId);
    QVector<QMap <QString,QString>> getUserEvents(QString userId);
    bool deleteEvent(QString userId,QString eventId);
    void testDatabase();
private:
    QSqlDatabase db;
    QSqlQuery* query;

    enum EventParts{
        ID,
        USER_ID,
        START_TIME,
        END_TIME,
        TITLE,
        DESCRIPTION,
        TYPE
    };
};


#endif // MYSQL_H
