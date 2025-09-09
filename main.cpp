#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QDebug>
#include "tcpserverhandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Регистрируем наш C++ класс
    qmlRegisterType<TcpServerHandler>("FileServer", 1, 0, "TcpServerHandler");

    // Путь к QML файлу в файловой системе
    QString qmlPath = "C:/Users/HONOR/Documents/File-sharing/Main.qml";

    // Проверяем существование файла
    if (!QFile::exists(qmlPath)) {
        qCritical() << "QML file not found at:" << qmlPath;
        qCritical() << "Current directory:" << QDir::currentPath();
        return -1;
    }

    qDebug() << "Loading QML from:" << qmlPath;

    // Загружаем QML из файловой системы
    engine.load(QUrl::fromLocalFile(qmlPath));

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML!";
        return -1;
    }

    qDebug() << "Application started successfully!";
    return app.exec();
}
