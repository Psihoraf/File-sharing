#include "tcpserverhandler.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QDataStream>
#include <QDateTime>

TcpServerHandler::TcpServerHandler(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_clientSocket(nullptr)
    , m_isRunning(false)
    , m_pathManager(new FilePathManager(this))
{
    connect(m_tcpServer, &QTcpServer::newConnection, this, &TcpServerHandler::onNewConnection);
}

QString TcpServerHandler::serverAddress() const { return m_serverAddress; }
bool TcpServerHandler::isRunning() const { return m_isRunning; }
QString TcpServerHandler::receivedFileName() const { return m_receivedFileName; }
FilePathManager* TcpServerHandler::pathManager() const { return m_pathManager; }

void TcpServerHandler::startServer()
{
    if (m_isRunning) {
        emit errorOccurred("Сервер уже запущен");
        return;
    }

    if (!m_tcpServer->listen(QHostAddress::Any, 12345)) {
        emit errorOccurred("Не удалось запустить сервер: " + m_tcpServer->errorString());
        return;
    }

    m_isRunning = true;

    // Получаем IP-адрес сервера
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for (const QHostAddress &address : ipAddressesList) {
        if (address != QHostAddress::LocalHost && address.protocol() == QAbstractSocket::IPv4Protocol) {
            ipAddress = address.toString();
            break;
        }
    }

    if (ipAddress.isEmpty()) {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    m_serverAddress = ipAddress + ":" + QString::number(m_tcpServer->serverPort());

    qDebug() << "Сервер запущен на" << m_serverAddress;
    qDebug() << "Файлы сохраняются в:" << m_pathManager->savePath();
    emit isRunningChanged();
    emit serverAddressChanged();
}

void TcpServerHandler::stopServer()
{
    if (m_isRunning) {
        m_tcpServer->close();
        if (m_clientSocket) {
            m_clientSocket->close();
            m_clientSocket->deleteLater();
            m_clientSocket = nullptr;
        }

        m_isRunning = false;
        m_serverAddress.clear();

        qDebug() << "Сервер остановлен";
        emit isRunningChanged();
        emit serverAddressChanged();
    }
}

void TcpServerHandler::onNewConnection()
{
    m_clientSocket = m_tcpServer->nextPendingConnection();
    if (m_clientSocket) {
        connect(m_clientSocket, &QTcpSocket::readyRead, this, &TcpServerHandler::onReadyRead);
        connect(m_clientSocket, &QTcpSocket::disconnected, this, [this]() {
            m_clientSocket->deleteLater();
            m_clientSocket = nullptr;
        });

        qDebug() << "Новое подключение от" << m_clientSocket->peerAddress().toString();
    }
}

void TcpServerHandler::onReadyRead()
{
    if (!m_clientSocket) {
        return;
    }

    static qint64 expectedFileSize = 0;
    static QString expectedFileName;
    static QFile *outputFile = nullptr;
    static qint64 bytesReceived = 0;

    QDataStream in(m_clientSocket);
    in.setVersion(QDataStream::Qt_6_0);

    if (expectedFileSize == 0) {
        if (m_clientSocket->bytesAvailable() < sizeof(qint64) * 2) {
            return;
        }

        in >> expectedFileSize >> expectedFileName;

        // Используем FilePathManager для получения пути файла
        QString filePath = m_pathManager->getFilePath(expectedFileName);

        outputFile = new QFile(filePath);
        if (!outputFile->open(QIODevice::WriteOnly)) {
            emit errorOccurred("Не удалось создать файл: " + filePath);
            delete outputFile;
            outputFile = nullptr;
            expectedFileSize = 0;
            return;
        }

        m_receivedFileName = QFileInfo(filePath).fileName();
        emit receivedFileNameChanged();

        qDebug() << "Прием файла:" << m_receivedFileName;
        qDebug() << "Сохраняется в:" << filePath;
    }

    if (outputFile && outputFile->isOpen()) {
        QByteArray data = m_clientSocket->readAll();
        qint64 bytesWritten = outputFile->write(data);
        bytesReceived += bytesWritten;

        if (bytesReceived >= expectedFileSize) {
            outputFile->close();
            delete outputFile;
            outputFile = nullptr;

            qDebug() << "Файл успешно принят:" << m_receivedFileName;
            emit fileReceived(m_receivedFileName);

            expectedFileSize = 0;
            bytesReceived = 0;
        }
    }
}
