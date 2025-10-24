import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    title: "File Transfer App"
    width: 400
    height: 400  // Увеличиваем высоту для четвертой кнопки
    minimumWidth: 350
    minimumHeight: 350
    visible: true

    // StackView для навигации между окнами
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainMenuPage
    }

    // Главное меню
    Component {
        id: mainMenuPage

        Page {
            background: Rectangle {
                color: "#f5f5f5"
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 15

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "📁 File Transfer"
                    font.bold: true
                    font.pixelSize: 24
                    color: "#333"
                }

                // ЛОКАЛЬНАЯ СЕТЬ
                Button {
                    Layout.preferredWidth: 280
                    Layout.preferredHeight: 50
                    text: "📤 Отправить файл (Локальная сеть)"
                    font.pixelSize: 14
                    onClicked: {
                        stackView.push(Qt.createComponent("Client.qml"))
                    }
                    background: Rectangle {
                        color: parent.down ? "#4CAF50" : "#8BC34A"
                        radius: 10
                    }
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    Layout.preferredWidth: 280
                    Layout.preferredHeight: 50
                    text: "📥 Принять файл (Локальная сеть)"
                    font.pixelSize: 14
                    onClicked: {
                        stackView.push(Qt.createComponent("Server.qml"))
                    }
                    background: Rectangle {
                        color: parent.down ? "#2196F3" : "#03A9F4"
                        radius: 10
                    }
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                // ЧЕРЕЗ ИНТЕРНЕТ
                Button {
                    Layout.preferredWidth: 280
                    Layout.preferredHeight: 50
                    text: "🌐 Обмен через интернет"
                    font.pixelSize: 14
                    onClicked: {
                        // Пробуем загрузить HotspotClient.qml
                        var component = Qt.createComponent("HotspotClient.qml")
                        if (component.status === Component.Ready) {
                            stackView.push(component)
                        } else {
                            console.error("Ошибка загрузки HotspotClient.qml:", component.errorString())
                            // Показываем сообщение об ошибке
                            errorPopup.text = "Не удалось загрузить модуль интернет-обмена:\n" + component.errorString()
                            errorPopup.open()
                        }
                    }
                    background: Rectangle {
                        color: parent.down ? "#FF9800" : "#FFC107"
                        radius: 10
                    }
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                // Альтернативная кнопка (если основная не работает)
                Button {
                    Layout.preferredWidth: 280
                    Layout.preferredHeight: 40
                    text: "🌐 Интернет (альтернатива)"
                    font.pixelSize: 12
                    visible: false // Скрыта по умолчанию
                    onClicked: {
                        // Прямой переход без создания компонента
                        stackView.push("HotspotClient.qml")
                    }
                    background: Rectangle {
                        color: parent.down ? "#9C27B0" : "#E91E63"
                        radius: 8
                    }
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Выберите режим работы"
                    color: "#666"
                    font.pixelSize: 12
                    Layout.topMargin: 10
                }

                // Подсказки
                ColumnLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 4

                    Label {
                        text: "💡 Локальная сеть - для устройств в одной WiFi/сети"
                        font.pixelSize: 10
                        color: "#888"
                    }
                    Label {
                        text: "🌐 Интернет - для устройств в разных сетях/домах"
                        font.pixelSize: 10
                        color: "#888"
                    }
                    Label {
                        text: "📡 Использует публичный сервер для соединения"
                        font.pixelSize: 10
                        color: "#888"
                    }
                }

                // Информация о версии
                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "v1.0 • Все режимы активны"
                    font.pixelSize: 10
                    color: "#aaa"
                    Layout.topMargin: 10
                }
            }
        }
    }

    // Попап для ошибок
    Popup {
        id: errorPopup
        anchors.centerIn: parent
        width: parent.width * 0.8
        height: 150
        modal: true
        focus: true

        property string text: ""

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Label {
                text: "❌ Ошибка"
                font.bold: true
                font.pixelSize: 16
                color: "red"
            }

            Label {
                text: errorPopup.text
                wrapMode: Text.Wrap
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Button {
                text: "OK"
                onClicked: errorPopup.close()
                Layout.alignment: Qt.AlignRight
            }
        }
    }

    // Обработчик ошибок навигации
    Connections {
        target: stackView
        function onCurrentItemChanged() {
            if (stackView.currentItem === null) {
                console.error("Ошибка при загрузке страницы")
            }
        }
    }
}
