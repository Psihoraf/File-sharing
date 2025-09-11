import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: mainWindow
    title: "File Transfer App"
    width: 400
    height: 300
    minimumWidth: 350
    minimumHeight: 250
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
                spacing: 20

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "📁 File Transfer"
                    font.bold: true
                    font.pixelSize: 24
                    color: "#333"
                }

                Button {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 60
                    text: "📤 Отправить файл"
                    font.pixelSize: 16
                    onClicked: {
                        // Загружаем клиентскую страницу при переходе
                        stackView.push(Qt.resolvedUrl("C:/Users/HONOR/Documents/File_sharing_Client_version-Psihoraf-patch-1/Client.qml"));
                    }

                    background: Rectangle {
                        color: parent.down ? "#4CAF50" : "#8BC34A"
                        radius: 10
                    }
                }

                Button {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 60
                    text: "📥 Принять файл"
                    font.pixelSize: 16
                    onClicked: {
                        // Загружаем серверную страницу при переходе
                        stackView.push(Qt.resolvedUrl("C:/Users/HONOR/Documents/File_sharing_Client_version-Psihoraf-patch-1/Server.qml"));
                    }

                    background: Rectangle {
                        color: parent.down ? "#2196F3" : "#03A9F4"
                        radius: 10
                    }
                }

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Выберите режим работы"
                    color: "#666"
                    font.pixelSize: 14
                }
            }
        }
    }
}
