#include <QCoreApplication>
#include <QTest>
#include <iostream>
#include "httprequest.h"
#include "httpresponse.h"
#include "server.h"
#include <QDebug>


QJsonObject getSettings(){
    QJsonDocument settingsDoc;
    QFile settingsFile;
    settingsFile.setFileName("settings.json");
    if(settingsFile.exists()){
        if (settingsFile.open(QIODevice::ReadOnly)){
            settingsDoc = QJsonDocument::fromJson(settingsFile.readAll());
            settingsFile.close();
            if (!settingsDoc.isNull())
                qInfo() << "Settings loaded";
            else
                qCritical() << "Failed to load settings";
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
            qInfo() << "Settings file created";
            return settingsObj;
        }
        else{
            qCritical() << "Failed to create settings file";
            return settingsDoc.object();
        }
    }
}


void logOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{

    QDir logDir("");
    logDir.mkdir("log");

    static QString logFilename;
    if (logFilename.isEmpty()){
        logFilename = QDateTime::currentDateTime().toString("log/dd_MM_hh_mm_ss") + ".txt";
    }
    QFile logFile;
    logFile.setFileName(logFilename);
    logFile.exists();

    if(!logFile.open(QIODevice::Append | QIODevice::Text)){
        return;
    }

    QString sCurrDateTime = "[" +
              QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz") + "]";
    QTextStream logStream(&logFile);

    switch(type){
    case QtDebugMsg:
        logStream << QString("%1 Debug - %2\n").arg(sCurrDateTime).arg(msg);
        break;
    case QtWarningMsg:
        logStream << QString("%1 Warning - %2\n").arg(sCurrDateTime).arg(msg);
        break;
    case QtCriticalMsg:
        logStream << QString("%1 Critical - %2\n").arg(sCurrDateTime).arg(msg);
        break;
    case QtInfoMsg:
        logStream << QString("%1 Info - %2\n").arg(sCurrDateTime).arg(msg);
        break;
    case QtFatalMsg:
        logStream << QString("%1 Fatal - %2\n").arg(sCurrDateTime).arg(msg);
        abort();
    }

    logStream.flush();
    logFile.flush();
    logFile.close();

}



int main(int argc, char *argv[])
{



    QCoreApplication a(argc, argv);

    qInstallMessageHandler(logOutput);
    qInfo() << "Starting server";
    Server server;
    QJsonObject settings = getSettings();
    server.startServer(settings);
    return a.exec();
}
