#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QDebug>
#include "tcpclienthandler.h"
#include "tcpserverhandler.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<TcpServerHandler>("FileServer", 1, 0, "TcpServerHandler");
    qmlRegisterType<TcpClientHandler>("FileClient", 1, 0, "TcpClientHandler");

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
    engine.loadFromModule("MyServer", "Main");

    return app.exec();
}
