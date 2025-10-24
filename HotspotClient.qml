import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import MyServer 1.0  // Используем тот же модуль, что и в main.cpp

Page {
    id: hotspotPage
    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: "← Назад"
                onClicked: stackView.pop()
            }
            Label {
                Layout.fillWidth: true
                text: "🌐 Обмен через интернет"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    HotspotChat {
        id: hotspotChat

        onRedirected: (url) => {
            console.log("Redirected to:", url)
            addressField.text = url
            statusLabel.text = "✅ Подключено к пиру: " + url
            statusLabel.color = "green"
            greetAddress(url)
            addressField.focus = false
            resetStatusTimer.start()
        }

        onMessagesChanged: {
            if (hotspotChat.messages.length > 0) {
                let lastMessage = hotspotChat.messages[hotspotChat.messages.length - 1]
                if (lastMessage.from === "remote" && lastMessage.type === "file") {
                    transfersModel.append({
                        "fileName": lastMessage.text,
                        "status": "✓ Получен",
                        "time": new Date().toLocaleTimeString(),
                        "type": "received",
                        "filePath": lastMessage.path
                    })
                    statusLabel.text = "📥 Получен файл: " + lastMessage.text
                    statusLabel.color = "blue"
                    resetStatusTimer.start()
                }
            }
        }

        onConnectionStateChanged: {
            updateConnectionStatus()
        }

        Component.onCompleted: {
            updateConnectionStatus()
            greetAddress("h://94.159.103.63:17172")
        }
    }

    ListModel {
        id: transfersModel
    }

    function updateConnectionStatus() {
        switch(hotspotChat.connectionState) {
            case HotspotChat.Disconnected:
                statusLabel.text = "❌ Не подключено"
                statusLabel.color = "red"
                connectionStatus.text = "Отключено"
                connectionIndicator.color = "red"
                break
            case HotspotChat.ConnectedToRouter:
                statusLabel.text = "🔄 Подключено к серверу, ожидание пира..."
                statusLabel.color = "orange"
                connectionStatus.text = "Ожидание пира"
                connectionIndicator.color = "orange"
                break
            case HotspotChat.ConnectedToPeer:
                statusLabel.text = "✅ Подключено к пиру"
                statusLabel.color = "green"
                connectionStatus.text = "Подключено"
                connectionIndicator.color = "green"
                break
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // ИНФОРМАЦИЯ О ПУТИ СОХРАНЕНИЯ
        GroupBox {
            Layout.fillWidth: true
            title: "📁 Настройки сохранения"

            RowLayout {
                width: parent.width

                Button {
                    text: "📁 Выбрать путь"
                    onClicked: folderDialog.open()
                }

                Label {
                    Layout.fillWidth: true
                    text: "Путь: " + (hotspotChat.pathManager ? hotspotChat.pathManager.savePath : "Загрузки")
                    elide: Text.ElideLeft
                    wrapMode: Text.Wrap
                    color: "blue"
                    font.pixelSize: 12
                }
            }
        }

        // Информация о подключении
        GroupBox {
            Layout.fillWidth: true
            title: "🔗 Статус подключения"

            ColumnLayout {
                width: parent.width
                spacing: 10

                RowLayout {
                    Label {
                        text: "Локальный порт:"
                        font.bold: true
                    }
                    Label {
                        text: hotspotChat.port
                        color: "blue"
                    }
                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        color: connectionIndicator.color
                        id: connectionIndicator
                    }
                    Label {
                        text: connectionStatus.text
                        color: connectionIndicator.color
                        font.bold: true
                        id: connectionStatus
                    }
                }

                Label {
                    text: "Relay сервер: 94.159.103.63:17172"
                    font.pixelSize: 12
                    color: "gray"
                }
            }
        }

        // Поле для ручного ввода адреса
                GroupBox {
                    Layout.fillWidth: true
                    title: "🎯 Подключение к пиру"

                    ColumnLayout {
                        width: parent.width
                        spacing: 10

                        TextField {
                            id: addressField
                            Layout.fillWidth: true
                            placeholderText: "h://192.168.1.100:12345"
                            text: "h://127.0.0.1:17171"
                        }

                        RowLayout {
                            Button {
                                text: "Приветствовать"
                                onClicked: {
                                    if (addressField.text) {
                                        hotspotChat.greetAddress(addressField.text)
                                        statusLabel.text = "Отправляю приветствие..."
                                        statusLabel.color = "blue"
                                    }
                                }
                            }

                            Button {
                                text: "Отправить сообщение"
                                enabled: messageField.text.length > 0 && hotspotChat.connected
                                onClicked: {
                                    hotspotChat.send(messageField.text)
                                    messageField.text = ""
                                }
                            }
                        }

                        TextField {
                            id: messageField
                            Layout.fillWidth: true
                            placeholderText: "Введите сообщение..."
                            enabled: hotspotChat.connected
                        }
                    }
                }

                // Статус
                Label {
                    id: statusLabel
                    Layout.fillWidth: true
                    text: "Подготовка к работе..."
                    color: "gray"
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap
                    font.pixelSize: 14
                }

                // Передача файлов
                GroupBox {
                    Layout.fillWidth: true
                    title: "📁 Передача файлов"

                    ColumnLayout {
                        width: parent.width
                        spacing: 10

                        Button {
                            Layout.fillWidth: true
                            text: "📁 Выбрать файл для отправки"
                            enabled: hotspotChat.connected
                            onClicked: fileDialog.open()

                            background: Rectangle {
                                color: parent.enabled ? (parent.down ? "#4CAF50" : "#8BC34A") : "#cccccc"
                                radius: 5
                            }
                        }

                        Label {
                            text: "Или перетащите файлы в область ниже"
                            font.pixelSize: 11
                            color: "gray"
                            horizontalAlignment: Text.AlignHCenter
                            Layout.fillWidth: true
                        }
                    }
                }

                // Разделитель
                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: "#e0e0e0"
                }

        // История передач
        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "📋 История передач (" + transfersModel.count + ")"

            ColumnLayout {
                anchors.fill: parent
                spacing: 5

                // Статистика
                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Отправлено: " + sentCount
                        color: "green"
                        font.pixelSize: 12
                    }
                    Label {
                        text: "Получено: " + receivedCount
                        color: "blue"
                        font.pixelSize: 12
                    }
                    Item { Layout.fillWidth: true }
                    Button {
                        text: "🧹 Очистить"
                        enabled: transfersModel.count > 0
                        onClicked: transfersModel.clear()
                    }
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ListView {
                        id: listView
                        width: parent.width
                        model: transfersModel
                        spacing: 5

                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 60
                            radius: 8
                            color: {
                                if (type === "received") return "#e8f5e8"
                                else if (type === "sent") return "#fff3e0"
                                else return "#f0f0f0"
                            }
                            border.color: {
                                if (type === "received") return "#4CAF50"
                                else if (type === "sent") return "#FF9800"
                                else return "#cccccc"
                            }
                            border.width: 1

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 8
                                spacing: 10

                                Label {
                                    text: {
                                        if (type === "received") return "📥"
                                        else if (type === "sent") return "📤"
                                        else return "ℹ️"
                                    }
                                    font.pixelSize: 18
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 2

                                    Label {
                                        text: fileName
                                        elide: Text.ElideMiddle
                                        font.bold: true
                                        Layout.fillWidth: true
                                    }

                                    RowLayout {
                                        Label {
                                            text: "Время: " + time
                                            font.pixelSize: 10
                                            color: "gray"
                                        }
                                        Item { Layout.fillWidth: true }
                                        Label {
                                            text: status
                                            color: status.includes("✓") ? "green" : "orange"
                                            font.pixelSize: 10
                                            font.bold: true
                                        }
                                    }
                                }

                                Button {
                                    visible: type === "received" && filePath
                                    text: "📂"
                                    ToolTip.visible: hovered
                                    ToolTip.text: "Открыть папку с файлом"
                                    onClicked: {
                                        if (filePath) {
                                            Qt.openUrlExternally(filePath)
                                        }
                                    }
                                }
                            }
                        }

                        // Область перетаскивания файлов
                        opacity: dropArea.containsDrag ? 0.7 : 1.0

                        DropArea {
                            id: dropArea
                            anchors.fill: parent
                            enabled: hotspotChat.connected

                            onEntered: (drag) => {
                                if (drag.hasUrls) {
                                    drag.accept()
                                }
                            }

                            onDropped: (drop) => {
                                for (let i = 0; i < drop.urls.length; i++) {
                                    let url = drop.urls[i]
                                    hotspotChat.sendFile(url)

                                    let fileName = url.toString()
                                    fileName = fileName.substring(fileName.lastIndexOf("/") + 1)
                                    if (fileName.startsWith("file://")) {
                                        fileName = fileName.substring(7)
                                    }

                                    transfersModel.append({
                                        "fileName": fileName,
                                        "status": "⏳ Отправляется...",
                                        "time": new Date().toLocaleTimeString(),
                                        "type": "sent"
                                    })
                                }
                            }

                            Rectangle {
                                anchors.fill: parent
                                color: "transparent"
                                border.color: dropArea.containsDrag ? "#2196F3" : "transparent"
                                border.width: 2
                                radius: 5

                                Label {
                                    anchors.centerIn: parent
                                    text: "Перетащите файлы сюда"
                                    color: "#666"
                                    font.pixelSize: 16
                                    visible: dropArea.containsDrag
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // ДИАЛОГ ВЫБОРА ПАПКИ
       FolderDialog {
           id: folderDialog
           title: "Выберите папку для сохранения файлов"

           onAccepted: {
               if (hotspotChat.pathManager) {
                   var folderPath = selectedFolder.toString();
                   if (folderPath.startsWith("file:///")) {
                       folderPath = folderPath.substring(8);
                   }
                   hotspotChat.pathManager.setSavePath(folderPath);
               }
           }
       }

       // Диалог выбора файлов
       FileDialog {
           id: fileDialog
           title: "Выберите файлы для отправки"
           nameFilters: ["Все файлы (*)"]
           fileMode: FileDialog.OpenFiles

           onAccepted: {
               for (let i = 0; i < selectedFiles.length; i++) {
                   let fileUrl = selectedFiles[i]
                   hotspotChat.sendFile(fileUrl)

                   let fileName = fileUrl.toString()
                   fileName = fileName.substring(fileName.lastIndexOf("/") + 1)
                   if (fileName.startsWith("file://")) {
                       fileName = fileName.substring(7)
                   }

                   transfersModel.append({
                       "fileName": fileName,
                       "status": "⏳ Отправляется...",
                       "time": new Date().toLocaleTimeString(),
                       "type": "sent"
                   })
               }
           }
       }

       // Таймер сброса статуса
       Timer {
           id: resetStatusTimer
           interval: 5000
           onTriggered: {
               updateConnectionStatus()
           }
       }

    // Вычисляемые свойства для статистики
    property int sentCount: {
        let count = 0
        for (let i = 0; i < transfersModel.count; i++) {
            if (transfersModel.get(i).type === "sent") count++
        }
        return count
    }

    property int receivedCount: {
        let count = 0
        for (let i = 0; i < transfersModel.count; i++) {
            if (transfersModel.get(i).type === "received") count++
        }
        return count
    }

    // Информация при первом открытии
    Component.onCompleted: {
        if (transfersModel.count === 0) {
            transfersModel.append({
                "fileName": "Добро пожаловать в режим интернет-обмена!",
                "status": "ℹ️",
                "time": new Date().toLocaleTimeString(),
                "type": "info"
            })
        }
    }
}
