#include "tester.h"

DatabaseIntefaceTester::DatabaseIntefaceTester(QObject* parent):
    QObject(parent)
{
    db.connectDatabase();
    generator.seed(QDateTime::currentSecsSinceEpoch());
    username = QString::number(generator.generate() );
    password = QString::number(generator.generate() );
}

void DatabaseIntefaceTester::authorise(){

    QVERIFY(db.authorise("test","test") && !db.authorise("wrong","wrong"));
}
void DatabaseIntefaceTester::addUser(){
    QVERIFY(db.addUser(username,password));
}
void DatabaseIntefaceTester::getUserId(){

    QCOMPARE(db.getUserId("test","test"), "1");
}
void DatabaseIntefaceTester::changeUserPassword(){
    QString newPassword = QString::number(generator.generate());

    QVERIFY(db.changeUserPassword(username,password,newPassword));
    password = newPassword;
}
void DatabaseIntefaceTester::addEvent(){

    eventId = db.addEvent(db.getUserId(username,password),
                          QString::number(QDateTime::currentSecsSinceEpoch()),
                          QString::number(QDateTime::currentSecsSinceEpoch()),
                          "test title",
                          "test description");
    QVERIFY(!eventId.isEmpty()) ;
}
void DatabaseIntefaceTester::modifyEventStartTime(){

    QVERIFY(db.modifyEventStartTime(db.getUserId(username,password),
                                    eventId,
                                    QString::number(QDateTime::currentSecsSinceEpoch())));
}
void DatabaseIntefaceTester::modifyEventEndTime(){

    QVERIFY(db.modifyEventEndTime(db.getUserId(username,password),
                                    eventId,
                                    QString::number(QDateTime::currentSecsSinceEpoch())));
}
void DatabaseIntefaceTester::modifyEventTitle(){

    QVERIFY(db.modifyEventTitle(db.getUserId(username,password),
                                    eventId,
                                    "changed title"));
}
void DatabaseIntefaceTester::modifyEventDescription(){

    QVERIFY(db.modifyEventDescription(db.getUserId(username,password),
                                    eventId,
                                    "changed description"));
}
void DatabaseIntefaceTester::getEvent()
{
    QVERIFY(!db.getEvent(db.getUserId(username,password),eventId).isEmpty());

}
void DatabaseIntefaceTester::getUserEvents(){

    QVERIFY(!db.getUserEvents(db.getUserId(username,password)).isEmpty());
}
void DatabaseIntefaceTester::deleteEvent(){

    QVERIFY(db.deleteEvent(db.getUserId(username,password),eventId));
}

RequestHandlerTester::RequestHandlerTester(QObject* parent):
    QObject(parent)
{

}

void RequestHandlerTester::handleTRACERequest()
{

}
void RequestHandlerTester::handleGETRequest()
{

}
void RequestHandlerTester::handleHEADRequest()
{

}
void RequestHandlerTester::handlePOSTRequest()
{

}
void RequestHandlerTester::handleDELETERequest()
{

}
