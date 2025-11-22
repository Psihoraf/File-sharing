import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import FileServer 1.0
import QtQuick.Dialogs

Page {
    padding: 20

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: "← Назад"
                onClicked: stackView.pop()
            }
            Label {
                Layout.fillWidth: true
                text: "📥 Режим приема"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 15

        GroupBox {
            Layout.fillWidth: true
            title: "📊 Статус сервера"

            GridLayout {
                columns: 2
                width: parent.width

                Label { text: "Адрес:"; font.bold: true }
                Label { text: serverHandler.serverAddress || "недоступен"; color: "blue" }

                Label { text: "Активных подключений:"; font.bold: true }
                Label {
                    text: serverHandler.activeConnections + " из 10"
                    color: serverHandler.activeConnections > 5 ? "orange" : "green"
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            title: "🔗 Активные подключения (" + connectionsModel.count + ")"

            ListView {
                anchors.fill: parent
                model: connectionsModel
                spacing: 2
                clip: true

                delegate: Rectangle {
                    width: parent.width
                    height: 25
                    color: "transparent"

                    RowLayout {
                        anchors.fill: parent
                        Label {
                            text: "🖥️ " + address
                            font.pixelSize: 12
                            elide: Text.ElideMiddle
                        }
                        Label {
                            text: status
                            color: "green"
                            font.pixelSize: 10
                        }
                    }
                }
            }
        }

        RowLayout {
            spacing: 10

            Button {
                Layout.fillWidth: true
                text: "📁 Выбрать путь сохранения"
                onClicked: folderDialog.open()
            }

            Label {
                Layout.fillWidth: true
                text: "Путь: " + serverHandler.pathManager.savePath
                elide: Text.ElideLeft
                wrapMode: Text.Wrap
                font.pixelSize: 12
            }
        }

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
                        connectionsModel.clear();
                    } else {
                        serverHandler.startServer();
                    }
                }
            }

            Button {
                Layout.fillWidth: true
                text: "📋 Копировать адрес"
                enabled: serverHandler.isRunning && serverHandler.serverAddress
                onClicked: {
                    if (serverHandler.serverAddress) {
                        var tempText = Qt.createQmlObject(
                            'import QtQuick 2.15; import QtQuick.Controls 2.15; TextField { visible: false }',
                            parent, "tempCopyField");
                        tempText.text = serverHandler.serverAddress;
                        tempText.selectAll();
                        tempText.copy();
                        tempText.destroy();

                        statusLabel.text = "Адрес скопирован!";
                        statusLabel.color = "blue";
                        resetStatusTimer.start();
                    }
                }
            }
        }

        Label {
            id: statusLabel
            Layout.fillWidth: true
            text: "Сервер остановлен"
            color: "gray"
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
        }

        // Разделитель
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#e0e0e0"
        }

        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 100
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
                        height: 60
                        radius: 5
                        color: index % 2 === 0 ? "#f8f9fa" : "#ffffff"

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 2

                            RowLayout {
                                Label {
                                    text: "📄 " + fileName
                                    elide: Text.ElideMiddle
                                    font.bold: true
                                    Layout.fillWidth: true
                                }
                                Label {
                                    text: status
                                    color: "green"
                                    font.bold: true
                                }
                            }

                            RowLayout {
                                Label {
                                    text: "От: " + client
                                    font.pixelSize: 10
                                    color: "gray"
                                    Layout.fillWidth: true
                                }
                                Label {
                                    text: "Время: " + time
                                    font.pixelSize: 10
                                    color: "gray"
                                }
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

    TcpServerHandler {
        id: serverHandler
        onFileReceived: function(fileName, clientAddress) {
            receivedFilesModel.append({
                "fileName": fileName,
                "status": "✓ Получен",
                "time": new Date().toLocaleTimeString(),
                "client": clientAddress
            });
            statusLabel.text = "Файл получен от " + clientAddress + ": " + fileName;
            statusLabel.color = "green";
            resetStatusTimer.start();
        }
        onErrorOccurred: function(errorMessage) {
            statusLabel.text = errorMessage;
            statusLabel.color = "red";
            resetStatusTimer.start();
        }
        onClientConnected: function(clientAddress) {
            statusLabel.text = "Подключился клиент: " + clientAddress;
            statusLabel.color = "blue";
            resetStatusTimer.start();
            connectionsModel.append({"address": clientAddress, "status": "✅ Подключен"});
        }
        onClientDisconnected: function(clientAddress) {
            // Удаляем клиента из списка подключений
            for (var i = 0; i < connectionsModel.count; i++) {
                if (connectionsModel.get(i).address === clientAddress) {
                    connectionsModel.remove(i);
                    break;
                }
            }
        }
    }

    ListModel {
        id: receivedFilesModel
    }

    ListModel {
        id: connectionsModel
    }

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
                statusLabel.text = "Сервер запущен: " + serverHandler.serverAddress +
                                  " | Подключений: " + serverHandler.activeConnections;
                statusLabel.color = "green";
            } else {
                statusLabel.text = "Сервер остановлен";
                statusLabel.color = "gray";
            }
        }
    }
}
