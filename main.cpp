#include <QCoreApplication>
#include <QTest>
#include <iostream>
#include "httprequest.h"
#include "httpresponse.h"
#include "server.h"


QJsonObject getSettings(){
    QJsonDocument settingsDoc;
    QFile settingsFile;
    settingsFile.setFileName("settings.json");
    if(settingsFile.exists()){
        if (settingsFile.open(QIODevice::ReadOnly)){
            settingsDoc = QJsonDocument::fromJson(settingsFile.readAll());
            settingsFile.close();
            return settingsDoc.object();
        }
        else return settingsDoc.object();
    }
    else {
        if (settingsFile.open(QIODevice::WriteOnly)){
            QJsonObject settingsObj;
            settingsObj.insert("IP","localhost");
            settingsObj.insert("port",2000);
            settingsObj.insert("database","QMYSQL");
            settingsObj.insert("databaseHost","localhost");
            settingsObj.insert("databaseName","funnotes");
            settingsObj.insert("databaseUsername","root");
            settingsObj.insert("databasePassword","");
            settingsDoc.setObject(settingsObj);
            settingsFile.write(settingsDoc.toJson());
            settingsFile.close();
            return settingsObj;
        }
        else return settingsDoc.object();
    }
}

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    getSettings();
    Server server;
    server.startServer(getSettings());



    return a.exec();
}
