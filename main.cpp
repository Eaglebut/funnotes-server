#include <QCoreApplication>
#include "server.h"
#include "database.h"
#include "tester.h"
#include <QTest>
#include<iostream>
int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    /*Server server;
    server.startServer();
*/
    QString request = "GET / HTTP/1.1\r\n"
                      "Host: localhost:5555\r\n"
                      "Connection: Upgrade\r\n"
                      "Pragma: no-cache\r\n"
                      "Cache-Control: no-cache\r\n"
                      "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36\r\n"
                      "Upgrade: websocket\r\n"
                      "Origin: file://\r\n"
                      "Sec-WebSocket-Version: 13\r\n"
                      "Accept-Encoding: gzip, deflate, br\r\n"
                      "Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
                      "Sec-WebSocket-Key: JTQR7L0z/zzSOqDCXHHx9A==\r\n"
                      "Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\n"
                      "\r\n"
                      "12345";
    HttpObject object(request);

    //QTest::qExec(new RequestHandlerTester, argc,argv);


    return a.exec();
}
