import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import FileServer 1.0
import QtQuick.Dialogs


Page {
    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: "← Назад"
                onClicked: {
                    // Закрываем это окно и возвращаемся к главному меню
                    stackView.pop();
                }
            }

            Label {
                Layout.fillWidth: true
                text: "📤 Режим отправки"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }


    Rectangle {

        width: parent.width
        height: parent.height
        color: "transparent"

        TcpServerHandler {
            id: serverHandler
            onFileReceived: function(fileName) {
                receivedFilesModel.append({"fileName": fileName, "status": "✓ Получен", "time": new Date().toLocaleTimeString()});
                statusLabel.text = "Файл получен: " + fileName;
                statusLabel.color = "green";
                resetStatusTimer.start();
            }
            onErrorOccurred: function(errorMessage) {
                statusLabel.text = errorMessage;
                statusLabel.color = "red";
                resetStatusTimer.start();
            }
        }

        ListModel {
            id: receivedFilesModel
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Button {
                Layout.fillWidth: true
                text: "📁 Выбрать путь сохранения"
                onClicked: folderDialog.open()
            }
            // Кнопки управления сервером
            RowLayout {
                Layout.fillWidth: true
                spacing: 10



                Button {
                    Layout.fillWidth: true
                    text: serverHandler.isRunning ? "🛑 Остановить сервер" : "🚀 Запустить сервер"
                    onClicked: {
                        if (serverHandler.isRunning) {
                            serverHandler.stopServer();
                            statusLabel.text = "Сервер остановлен";
                            statusLabel.color = "gray";
                        } else {
                            serverHandler.startServer();
                            statusLabel.text = "Сервер запущен";
                            statusLabel.color = "green";
                        }
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: "📋 Копировать адрес"
                    enabled: serverHandler.isRunning && serverHandler.serverAddress
                    onClicked: {
                        if (serverHandler.serverAddress) {
                            // Копирование в буфер обмена
                            ipText.text = serverHandler.serverAddress;
                            ipText.selectAll();
                            ipText.copy();
                            ipText.deselect();
                            statusLabel.text = "Адрес скопирован!";
                            statusLabel.color = "blue";
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

            // Статус сервера
            Label {
                id: statusLabel
                Layout.fillWidth: true
                text: "Сервер остановлен"
                color: "gray"
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
            }

            // Информация о сервере
            Label {
                Layout.fillWidth: true
                text: "Адрес: " + (serverHandler.serverAddress || "недоступен")
                wrapMode: Text.Wrap
                color: "blue"
                horizontalAlignment: Text.AlignHCenter
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
                        width: parent.width
                        model: receivedFilesModel
                        spacing: 5

                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 50
                            radius: 5
                            color: index % 2 === 0 ? "#f8f9fa" : "#ffffff"

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 5
                                spacing: 10

                                Label { text: "📄"; font.pixelSize: 16 }

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
                onClicked: receivedFilesModel.clear()
            }
        }

        // Диалог выбора папки
        FolderDialog {
            id: folderDialog
            title: "Выберите папку для сохранения файлов"

            onAccepted: {
                var folderPath = selectedFolder.toString();
                if (folderPath.startsWith("file:///")) {
                    folderPath = folderPath.substring(8);
                }
                serverHandler.pathManager.setSavePath(folderPath);
            }
        }
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
}
