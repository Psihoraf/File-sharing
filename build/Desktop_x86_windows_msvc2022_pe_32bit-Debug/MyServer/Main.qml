import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import FileServer 1.0

ApplicationWindow {
    id: mainWindow
    title: "File Transfer Server"
    width: 500
    height: 600
    visible: true
    minimumWidth: 400
    minimumHeight: 400

    // Для работы с буфером обмена
    property var clipboard: Qt.createQmlObject(
        "import QtQuick 2.0; Clipboard {}",
        mainWindow,
        "ClipboardObject"
    )

    // C++ обработчик сервера
    TcpServerHandler {
        id: serverHandler
        Component.onCompleted: {
            console.log("TcpServerHandler created successfully!");
        }


    }

    // Модель для списка полученных файлов
    ListModel {
        id: receivedFilesModel
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // Заголовок
        Label {
            Layout.fillWidth: true
            text: "📁 File Transfer Server"
            font.bold: true
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
        }

        // Статус сервера
        Label {
            id: statusLabel
            Layout.fillWidth: true
            text: "Сервер остановлен"
            color: "gray"
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
        }

        // Кнопки управления сервером
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                id: toggleButton
                Layout.fillWidth: true
                text: serverHandler.isRunning ? "🛑 Остановить сервер" : "🚀 Запустить сервер"
                onClicked: {
                    if (serverHandler.isRunning) {
                        serverHandler.stopServer();
                        statusLabel.text = "Сервер остановлен";
                        statusLabel.color = "gray";
                    } else {
                        serverHandler.startServer();
                    }
                }
            }

            Button {
                id: copyButton
                Layout.fillWidth: true
                text: "📋 Копировать адрес"
                enabled: serverHandler.isRunning && serverHandler.serverAddress
                onClicked: {
                    if (serverHandler.serverAddress) {
                        // Простое копирование текста
                        ipText.text = serverHandler.serverAddress;
                        ipText.selectAll();
                        ipText.copy();
                        ipText.deselect();

                        statusLabel.text = "Адрес скопирован в буфер обмена!";
                        statusLabel.color = "darkblue";
                        resetStatusTimer.start();
                    }
                }
            }
        }

        // Скрытый Text для копирования
        Text {
            id: ipText
            visible: false
        }

        // Разделитель
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#e0e0e0"
        }

        // Информация о подключении
        GroupBox {
            Layout.fillWidth: true
            title: "📡 Информация о сервере"

            ColumnLayout {
                width: parent.width
                spacing: 8

                // Статус
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Статус:"
                        font.bold: true
                        color: "gray"
                    }
                    Label {
                        Layout.fillWidth: true
                        text: serverHandler.isRunning ? "✅ Запущен" : "❌ Остановлен"
                        color: serverHandler.isRunning ? "green" : "red"
                    }
                }

                // Адрес
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Адрес:"
                        font.bold: true
                        color: "gray"
                    }
                    Label {
                        Layout.fillWidth: true
                        text: serverHandler.serverAddress || "недоступен"
                        color: "blue"
                        wrapMode: Text.Wrap
                    }
                }

                // Количество файлов
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Получено файлов:"
                        font.bold: true
                        color: "gray"
                    }
                    Label {
                        text: receivedFilesModel.count + " шт."
                    }
                }
            }
        }

        // Кнопка выбора файла для отправки
        Button {
            Layout.fillWidth: true
            text: "📤 Выбрать файл для отправки"
            onClicked: fileDialog.open()
        }

        // Разделитель
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#e0e0e0"
        }

        // Список полученных файлов
        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "📂 Полученные файлы (" + receivedFilesModel.count + ")"

            ScrollView {
                anchors.fill: parent
                clip: true

                ListView {
                    id: filesListView
                    width: parent.width
                    model: receivedFilesModel
                    spacing: 5

                    delegate: Rectangle {
                        width: filesListView.width
                        height: 50
                        radius: 5
                        color: index % 2 === 0 ? "#f8f9fa" : "#ffffff"
                        border.color: "#e9ecef"

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 10

                            Label {
                                text: "📄"
                                font.pixelSize: 16
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2

                                Label {
                                    text: fileName
                                    elide: Text.ElideMiddle
                                    font.bold: true
                                }

                                Label {
                                    text: "Время: " + time
                                    font.pixelSize: 10
                                    color: "gray"
                                }
                            }

                            Label {
                                text: status
                                color: "green"
                                font.bold: true
                            }
                        }
                    }
                }
            }
        }

        // Кнопка очистки списка
        Button {
            Layout.fillWidth: true
            text: "🧹 Очистить список"
            enabled: receivedFilesModel.count > 0
            onClicked: {
                receivedFilesModel.clear();
                statusLabel.text = "Список очищен";
                statusLabel.color = "orange";
                resetStatusTimer.start();
            }
        }
    }

    // Диалог выбора файла - ИСПРАВЛЕННАЯ ВЕРСИЯ
    FileDialog {
        id: fileDialog
        title: "Выберите файл для отправки"


        onAccepted: {
            // Безопасное получение пути к файлу
            var filePath = "";
            if (fileDialog.fileUrl) {
                filePath = fileDialog.fileUrl.toString();
                // Убираем префикс "file:///" для Windows
                if (filePath.startsWith("file:///")) {
                    filePath = filePath.substring(8); // Для Windows
                }
            }

            statusLabel.text = "Выбран файл: " + (filePath || "неизвестный файл");
            statusLabel.color = "purple";
            resetStatusTimer.start();

            // Здесь будет вызов метода отправки файла
            console.log("Selected file:", filePath);
        }

        onRejected: {
            statusLabel.text = "Выбор файла отменен";
            statusLabel.color = "gray";
            resetStatusTimer.start();
        }
    }

    // Таймер для сброса статусного сообщения
    Timer {
        id: resetStatusTimer
        interval: 3000
        onTriggered: {
            if (serverHandler.isRunning) {
                statusLabel.text = "Сервер запущен: " + serverHandler.serverAddress;
                statusLabel.color = "green";
            } else {
                statusLabel.text = "Сервер остановлен";
                statusLabel.color = "gray";
            }
        }
    }
}
