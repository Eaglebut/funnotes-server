#include <QCoreApplication>
#include "server.h"
#include "database.h"
#include "tester.h"
#include <QTest>
int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    Server server;
    server.startServer();



    //QTest::qExec(new DatabaseIntefaceTester , argc,argv);


    return a.exec();
}
