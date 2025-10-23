#include "tcpserverhandler.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QDataStream>
#include <QDateTime>

TcpServerHandler::TcpServerHandler(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_isRunning(false)
    , m_pathManager(new FilePathManager(this))
{
    connect(m_tcpServer, &QTcpServer::newConnection, this, &TcpServerHandler::onNewConnection);
}

TcpServerHandler::~TcpServerHandler()
{
    stopServer();
}

QString TcpServerHandler::serverAddress() const { return m_serverAddress; }
bool TcpServerHandler::isRunning() const { return m_isRunning; }
int TcpServerHandler::activeConnections() const { return m_clients.size(); }
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
    qDebug() << "Максимум подключений:" << MAX_CONNECTIONS;
    qDebug() << "Файлы сохраняются в:" << m_pathManager->savePath();
    emit isRunningChanged();
    emit serverAddressChanged();
}

void TcpServerHandler::stopServer()
{
    if (m_isRunning) {
        // Закрываем все клиентские подключения
        for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
            cleanupClient(it.key());
        }
        m_clients.clear();

        m_tcpServer->close();
        m_isRunning = false;
        m_serverAddress.clear();

        qDebug() << "Сервер остановлен";
        emit isRunningChanged();
        emit serverAddressChanged();
        emit activeConnectionsChanged();
    }
}

void TcpServerHandler::disconnectAllClients()
{
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
        it.key()->close();
    }
}

void TcpServerHandler::onNewConnection()
{
    if (m_clients.size() >= MAX_CONNECTIONS) {
        QTcpSocket *pendingSocket = m_tcpServer->nextPendingConnection();
        qDebug() << "Отклонено новое подключение - достигнут лимит" << MAX_CONNECTIONS;
        pendingSocket->close();
        pendingSocket->deleteLater();
        return;
    }

    QTcpSocket *clientSocket = m_tcpServer->nextPendingConnection();
    if (!clientSocket) {
        return;
    }

    // Инициализируем структуру для нового клиента
    ClientConnection connection;
    connection.socket = clientSocket;
    connection.expectedFileSize = 0;
    connection.bytesReceived = 0;
    connection.outputFile = nullptr;

    m_clients[clientSocket] = connection;

    // Подключаем сигналы для этого клиента
    connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServerHandler::onClientReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TcpServerHandler::onClientDisconnected);

    QString clientInfo = getClientIdentifier(clientSocket);
    qDebug() << "Новое подключение от:" << clientInfo;
    qDebug() << "Активных подключений:" << m_clients.size();

    emit clientConnected(clientInfo);
    emit activeConnectionsChanged();
}

void TcpServerHandler::onClientReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket || !m_clients.contains(clientSocket)) {
        return;
    }

    processClientData(clientSocket);
}

void TcpServerHandler::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) {
        return;
    }

    QString clientInfo = getClientIdentifier(clientSocket);
    qDebug() << "Клиент отключился:" << clientInfo;

    cleanupClient(clientSocket);
    m_clients.remove(clientSocket);

    qDebug() << "Активных подключений:" << m_clients.size();
    emit clientDisconnected(clientInfo);
    emit activeConnectionsChanged();
}

void TcpServerHandler::processClientData(QTcpSocket* clientSocket)
{
    if (!m_clients.contains(clientSocket)) {
        return;
    }

    ClientConnection &connection = m_clients[clientSocket];
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_6_0);

    // Если это начало нового файла
    if (connection.expectedFileSize == 0) {
        if (clientSocket->bytesAvailable() < sizeof(qint64) * 2) {
            return; // Ждем больше данных
        }

        in >> connection.expectedFileSize >> connection.expectedFileName;

        // Используем FilePathManager для получения пути файла
        QString filePath = m_pathManager->getFilePath(connection.expectedFileName);

        connection.outputFile = new QFile(filePath);
        if (!connection.outputFile->open(QIODevice::WriteOnly)) {
            emit errorOccurred("Не удалось создать файл: " + filePath);
            delete connection.outputFile;
            connection.outputFile = nullptr;
            connection.expectedFileSize = 0;
            return;
        }

        connection.receivedFileName = QFileInfo(filePath).fileName();
        connection.bytesReceived = 0;

        QString clientInfo = getClientIdentifier(clientSocket);
        qDebug() << "Начинаем прием файла от" << clientInfo;
        qDebug() << "Файл:" << connection.receivedFileName;
        qDebug() << "Размер:" << connection.expectedFileSize << "байт";
    }

    // Принимаем данные файла
    if (connection.outputFile && connection.outputFile->isOpen()) {
        QByteArray data = clientSocket->readAll();
        qint64 bytesWritten = connection.outputFile->write(data);
        connection.bytesReceived += bytesWritten;

        // Выводим прогресс для отладки
        if (connection.expectedFileSize > 0) {
            int progress = (connection.bytesReceived * 100) / connection.expectedFileSize;
            if (progress % 10 == 0) { // Логируем каждые 10%
                qDebug() << "Прогресс приема от" << getClientIdentifier(clientSocket)
                         << ":" << progress << "%";
            }
        }

        // Если файл полностью принят
        if (connection.bytesReceived >= connection.expectedFileSize) {
            connection.outputFile->close();
            delete connection.outputFile;
            connection.outputFile = nullptr;

            QString clientInfo = getClientIdentifier(clientSocket);
            qDebug() << "Файл успешно принят от" << clientInfo << ":" << connection.receivedFileName;

            emit fileReceived(connection.receivedFileName, clientInfo);

            // Сбрасываем состояние для следующего файла от этого клиента
            connection.expectedFileSize = 0;
            connection.bytesReceived = 0;
            QString finishedFileName = connection.receivedFileName;
            connection.receivedFileName.clear();

            // Клиент может сразу отправить следующий файл
            if (clientSocket->bytesAvailable() > 0) {
                qDebug() << "Обрабатываем следующий файл от" << clientInfo;
                processClientData(clientSocket);
            }
        }
    }
}

void TcpServerHandler::cleanupClient(QTcpSocket* clientSocket)
{
    if (!clientSocket) {
        return;
    }

    // Отключаем все сигналы
    clientSocket->disconnect();

    // Очищаем ресурсы
    if (m_clients.contains(clientSocket)) {
        ClientConnection &connection = m_clients[clientSocket];
        if (connection.outputFile && connection.outputFile->isOpen()) {
            connection.outputFile->close();
            delete connection.outputFile;
            connection.outputFile = nullptr;
        }
    }

    if (clientSocket->state() == QAbstractSocket::ConnectedState) {
        clientSocket->close();
    }
}

QString TcpServerHandler::getClientIdentifier(QTcpSocket* clientSocket) const
{
    if (!clientSocket) {
        return "Unknown";
    }
    return clientSocket->peerAddress().toString() + ":" + QString::number(clientSocket->peerPort());
}
