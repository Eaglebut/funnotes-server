#ifndef TESTER_H
#define TESTER_H

#include <QtTest/QtTest>
#include "database.h"
#include <QVector>
#include <QString>
#include <QRandomGenerator>
#include "httpobject.h"


class DatabaseIntefaceTester: public QObject
{
    Q_OBJECT
public:
    explicit DatabaseIntefaceTester(QObject* parent = 0);
private slots:
    void authorise();
    void addUser();
    void getUserId();
    void changeUserPassword();
    void addEvent();
    void modifyEventStartTime();
    void modifyEventEndTime();
    void modifyEventTitle();
    void modifyEventDescription();
    void getUserEvents();
    void getEvent();
    void deleteEvent();

private:
    Database_interface db;
    QRandomGenerator generator;
    QString username;
    QString password;
    QString eventId;
};

class RequestHandlerTester: public QObject{
    Q_OBJECT
public:
    explicit RequestHandlerTester(QObject* parent = 0);

private slots:
    void handleTRACERequest();
    void handleGETRequest();
    void handleHEADRequest();
    void handlePOSTRequest();
    void handleDELETERequest();

private:
};

#endif // TESTER_H

























