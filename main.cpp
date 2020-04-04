#include <QCoreApplication>
#include <QTest>
#include <iostream>
#include "httprequest.h"
#include "httpresponse.h"
#include "server.h"
int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    Server server;
    server.startServer();

    /*HttpRequest request;
    request.parseHttp("GET /1855011600 HTTP/1.1\nUser-Agent: Android 10 FunNotes v0.0.1\nAccept-Language: ru en\nDate: 24.03.2020 11: 15: 13 MSK\nAuthorization: Basic 3698830527\n\n");
    HttpResponse response = server.manageRequest(request);
    qDebug() << response.getResponse();

    request.parseHttp("GET /185511600 HTTP/1.1\nUser-Agent: Android 10 FunNotes v0.0.1\nAccept-Language: ru en\nDate: 24.03.2020 11: 15: 13 MSK\nAuthorization: Basic 3698830527\n\n");
    response = server.manageRequest(request);
    qDebug() << response.getResponse();

    request.parseHttp("GET/185511600 HTTP/1.1\nUser-Aget: Android 10 FunNotes v0.0.1\nAccept-Language: ru en\nDate: 24.03.2020 11: 15: 13 MSK\nAuthorization: Basic 3698830527\n\n");
    response = server.manageRequest(request);
    qDebug() << response.getResponse();

    request.parseHttp("GET /1855011600/16 HTTP/1.1\nUser-Agent: Android 10 FunNotes v0.0.1\nAccept-Language: ru en\nDate: 24.03.2020 11: 15: 13 MSK\nAuthorization: Basic 3698830527\n\n");
    response = server.manageRequest(request);
    qDebug() << response.getResponse();

    request.parseHttp("PUT /test1 HTTP/1.1\nUser-Agent: Android 10 FunNotes v0.0.1\nAccept-Language: ru en\nDate: 24.03.2020 11: 15: 13 MSK\nAuthorization: Basic test1\n\n");
    response = server.manageRequest(request);
    qDebug() << response.getResponse();
*/


    //QTest::qExec(new RequestHandlerTester, argc,argv);


    return a.exec();
}
