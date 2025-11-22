import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as Basic
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow

    title: "File Transfer App"
    width: 400
    height: 400
    minimumWidth: 350
    minimumHeight: 350
    visible: true

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainMenuPage
    }

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

                Basic.Button {
                    Layout.preferredWidth: 280
                    Layout.preferredHeight: 50
                    text: "📤 Отправить файл (Локальная сеть)"
                    font.pixelSize: 14
                    onClicked: stackView.push("Client.qml")
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

                Basic.Button {
                    Layout.preferredWidth: 280
                    Layout.preferredHeight: 50
                    text: "📥 Принять файл (Локальная сеть)"
                    font.pixelSize: 14
                    onClicked: stackView.push("Server.qml")
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

                Basic.Button {
                    Layout.preferredWidth: 280
                    Layout.preferredHeight: 50
                    text: "🌐 Обмен через интернет"
                    font.pixelSize: 14
                    onClicked: stackView.push("HotspotClient.qml")
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

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Выберите режим работы"
                    color: "#666"
                    font.pixelSize: 12
                    Layout.topMargin: 10
                }

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
}
