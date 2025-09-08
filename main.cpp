#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QDebug>
#include <QQuickView>

// Включаем наш заголовочный файл
#include "tcpserverhandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Регистрируем наш C++ класс как QML-тип
    qmlRegisterType<TcpServerHandler>("FileServer", 1, 0, "TcpServerHandler");

    // СОЗДАЕМ ПРОСТОЙ QML КОД ПРЯМО В КОДЕ, чтобы не зависеть от файла
    QString qmlCode = R"(
        import QtQuick 2.15
        import QtQuick.Controls 2.15
        import QtQuick.Layouts 1.15
        import FileServer 1.0

        ApplicationWindow {
            title: "File Receiver Server"
            width: 400
            height: 300
            visible: true

            TcpServerHandler {
                id: serverHandler
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                Button {
                    Layout.fillWidth: true
                    text: serverHandler.isRunning ? "Остановить сервер" : "Запустить сервер"
                    onClicked: {
                        if (serverHandler.isRunning) {
                            serverHandler.stopServer();
                        } else {
                            serverHandler.startServer();
                        }
                    }
                }

                Text {
                    Layout.fillWidth: true
                    text: "Статус: " + (serverHandler.isRunning ? "Запущен" : "Остановлен")
                    color: serverHandler.isRunning ? "green" : "red"
                }

                Text {
                    Layout.fillWidth: true
                    text: "Адрес: " + (serverHandler.serverAddress || "недоступен")
                    wrapMode: Text.Wrap
                }

                Text {
                    Layout.fillWidth: true
                    text: "Полученный файл: " + (serverHandler.receivedFileName || "еще нет")
                    wrapMode: Text.Wrap
                }
            }
        }
    )";

    QQmlApplicationEngine engine;
    engine.loadData(qmlCode.toUtf8()); // Загружаем код из строки, а не из файла

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML from data!";
        return -1;
    }

    qDebug() << "Application started successfully!";
    return app.exec();
}
