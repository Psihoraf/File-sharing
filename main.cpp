#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QDebug>
#include "tcpserverhandler.h"
#include "tcpclienthandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Регистрируем оба C++ класса
    qmlRegisterType<TcpServerHandler>("FileServer", 1, 0, "TcpServerHandler");
    qmlRegisterType<TcpClientHandler>("FileClient", 1, 0, "TcpClientHandler");

    // Загружаем главное окно
    QString mainQmlPath = "C:/Users/HONOR/Documents/File_sharing_Client_version-Psihoraf-patch-1/Main.qml";

    if (!QFile::exists(mainQmlPath)) {
        qCritical() << "Main QML file not found at:" << mainQmlPath;
        return -1;
    }

    qDebug() << "Loading main QML from:" << mainQmlPath;
    engine.load(QUrl::fromLocalFile(mainQmlPath));

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML!";
        return -1;
    }

    qDebug() << "Application started successfully!";
    return app.exec();
}
