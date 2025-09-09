#include "tcpserverhandler.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QDataStream>
#include <QDateTime>
#include <QDir>

TcpServerHandler::TcpServerHandler(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_clientSocket(nullptr)
    , m_isRunning(false)
{
    connect(m_tcpServer, &QTcpServer::newConnection, this, &TcpServerHandler::onNewConnection);
}

QString TcpServerHandler::serverAddress() const { return m_serverAddress; }
bool TcpServerHandler::isRunning() const { return m_isRunning; }
QString TcpServerHandler::receivedFileName() const { return m_receivedFileName; }

void TcpServerHandler::startServer()
{
    if (m_isRunning) {
        emit errorOccurred("Сервер уже запущен");  // ТЕПЕРЬ ЭТОТ СИГНАЛ СУЩЕСТВУЕТ
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
            qDebug() << "Клиент отключился";
        });
        connect(m_clientSocket, &QTcpSocket::errorOccurred, this, &TcpServerHandler::onSocketError);

        qDebug() << "Новое подключение от" << m_clientSocket->peerAddress().toString();
    }
}

void TcpServerHandler::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    if (m_clientSocket) {
        emit errorOccurred("Ошибка сокета: " + m_clientSocket->errorString());
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

    // Если мы еще не начали принимать файл
    if (expectedFileSize == 0) {
        // Проверяем, достаточно ли данных для заголовка
        if (m_clientSocket->bytesAvailable() < sizeof(qint64) * 2) {
            return;
        }

        // Читаем заголовок: размер файла + имя файла
        in >> expectedFileSize >> expectedFileName;

        // Создаем папку для полученных файлов
        QDir dir;
        QString receivedDir = "received_files";
        if (!dir.exists(receivedDir)) {
            if (!dir.mkdir(receivedDir)) {
                emit errorOccurred("Не удалось создать папку для файлов");
                return;
            }
        }

        // Создаем файл для записи
        QString filePath = "C:/Users/HONOR/Downloads/" + expectedFileName;
        outputFile = new QFile(filePath);
        if (!outputFile->open(QIODevice::WriteOnly)) {
            emit errorOccurred("Не удалось создать файл: " + filePath);
            delete outputFile;
            outputFile = nullptr;
            expectedFileSize = 0;
            return;
        }

        m_receivedFileName = expectedFileName;
        emit receivedFileNameChanged();

        qDebug() << "Начинаем прием файла:" << expectedFileName << "размером:" << expectedFileSize << "байт";
        bytesReceived = 0;
    }

    // Принимаем данные файла
    if (outputFile && outputFile->isOpen()) {
        QByteArray data = m_clientSocket->readAll();
        qint64 bytesWritten = outputFile->write(data);
        bytesReceived += bytesWritten;

        qDebug() << "Принято:" << bytesReceived << "/" << expectedFileSize << "байт";

        // Если файл полностью принят
        if (bytesReceived >= expectedFileSize) {
            outputFile->close();
            delete outputFile;
            outputFile = nullptr;

            qDebug() << "Файл успешно принят:" << expectedFileName;
            emit fileReceived(expectedFileName);  // ТЕПЕРЬ ЭТОТ СИГНАЛ СУЩЕСТВУЕТ

            // Сбрасываем состояние для следующего файла
            expectedFileSize = 0;
            bytesReceived = 0;
            expectedFileName.clear();
        }
    }
}
