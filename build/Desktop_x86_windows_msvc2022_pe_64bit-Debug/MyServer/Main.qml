import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

ApplicationWindow {
    id: mainWindow
    width: 400
    height: 500
    title: "File Transfer Server"
    visible: true



    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // Отображение полученного имени файла
        Label {
            Layout.fillWidth: true
            text: serverHandler.receivedFileName
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 16
        }

        // Отображение IP и порта сервера
        Label {
            id: ipPortLabel
            Layout.fillWidth: true
            text: serverHandler.serverAddress
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            font.pixelSize: 14
        }

        // Статус сервера
        Label {
            id: statusLabel
            Layout.fillWidth: true
            text: "Сервер остановлен"
            horizontalAlignment: Text.AlignHCenter
            color: "gray"
        }

        // Кнопка для запуска/остановки сервера
        Button {
            id: toggleButton
            Layout.fillWidth: true
            text: "Запустить сервер"

            onClicked: {
                if (text === "Запустить сервер") {
                    serverHandler.startServer();
                    text = "Остановить сервер";
                    statusLabel.text = "Сервер запущен";
                    statusLabel.color = "green";
                } else {
                    serverHandler.stopServer();
                    text = "Запустить сервер";
                    statusLabel.text = "Сервер остановлен";
                    statusLabel.color = "gray";
                }
            }
        }

        // Кнопка для копирования адреса в буфер обмена
        Button {
            Layout.fillWidth: true
            text: "Скопировать адрес"

            onClicked: {
                // Копируем текст из Label в буфер обмена
                ipPortLabel.selectAll();
                ipPortLabel.copy();
                ipPortLabel.deselect();
                statusLabel.text = "Адрес скопирован в буфер!";
                statusLabel.color = "blue";

                // Сбрасываем статус через 2 секунды
                resetStatusTimer.start();
            }
        }
    }

    // Таймер для сброса статусного сообщения
    Timer {
        id: resetStatusTimer
        interval: 2000
        onTriggered: {
            if (toggleButton.text === "Остановить сервер") {
                statusLabel.text = "Сервер запущен";
                statusLabel.color = "green";
            } else {
                statusLabel.text = "Сервер остановлен";
                statusLabel.color = "gray";
            }
        }
    }
}
