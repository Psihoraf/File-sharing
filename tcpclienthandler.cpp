#include "tcpclienthandler.h"
#include <QDebug>
#include <QHostAddress>
#include <QDataStream>
#include <QFileInfo>

TcpClientHandler::TcpClientHandler(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_currentFile(nullptr)
    , m_connected(false)
    , m_progress(0)
    , m_fileSize(0)
    , m_bytesWritten(0)
{
    connect(m_socket, &QTcpSocket::connected, this, &TcpClientHandler::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpClientHandler::onDisconnected);
    connect(m_socket, &QTcpSocket::bytesWritten, this, &TcpClientHandler::onBytesWritten);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &TcpClientHandler::onErrorOccurred);
}

bool TcpClientHandler::isConnected()  { return m_connected; }
QString TcpClientHandler::status()  { return m_status; }
int TcpClientHandler::progress()  { return m_progress; }
QString TcpClientHandler::currentFile()  { return m_currentFilePath; }

void TcpClientHandler::connectToServer(const QString &address, quint16 port)
{
    if (m_connected) {
        disconnectFromServer();
    }

    m_status = "Подключение к " + address + ":" + QString::number(port);
    emit statusChanged();

    m_socket->connectToHost(address, port);
}

void TcpClientHandler::disconnectFromServer()
{
    m_socket->disconnectFromHost();
    if (m_currentFile && m_currentFile->isOpen()) {
        m_currentFile->close();
        delete m_currentFile;
        m_currentFile = nullptr;
    }
}

void TcpClientHandler::sendFile(const QString &filePath)
{
    if (!m_connected) {
        emit errorOccurred("Нет подключения к серверу");
        return;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        emit errorOccurred("Файл не существует: " + filePath);
        return;
    }

    m_currentFile = new QFile(filePath);
    if (!m_currentFile->open(QIODevice::ReadOnly)) {
        emit errorOccurred("Не удалось открыть файл: " + filePath);
        delete m_currentFile;
        m_currentFile = nullptr;
        return;
    }

    m_currentFilePath = fileInfo.fileName();
    m_fileSize = m_currentFile->size();
    m_bytesWritten = 0;

    emit currentFileChanged();
    m_progress = 0;
    emit progressChanged();

    // Создаем и отправляем заголовок
    QByteArray header;
    QDataStream stream(&header, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_0);

    // Пишем в заголовок: размер файла + имя файла
    stream << m_fileSize << m_currentFilePath;

    // Отправляем заголовок
    m_socket->write(header);

    m_status = "Отправка файла: " + m_currentFilePath;
    emit statusChanged();

    // Начинаем отправку данных
    sendFileData();
}

void TcpClientHandler::sendFileData()
{
    if (!m_currentFile || !m_currentFile->isOpen() || !m_connected) {
        return;
    }

    // Читаем и отправляем данные порциями по 64KB
    const qint64 chunkSize = 65536;
    QByteArray data = m_currentFile->read(chunkSize);
    qint64 bytesToWrite = data.size();

    if (bytesToWrite > 0) {
        qint64 bytesWritten = m_socket->write(data);
        if (bytesWritten == -1) {
            emit errorOccurred("Ошибка записи в сокет: " + m_socket->errorString());
            return;
        }
        m_bytesWritten += bytesWritten;

        // Обновляем прогресс
        int newProgress = static_cast<int>((m_bytesWritten * 100) / m_fileSize);
        if (newProgress != m_progress) {
            m_progress = newProgress;
            emit progressChanged();
        }

        // Если файл полностью отправлен
        if (m_bytesWritten >= m_fileSize) {
            finishFileTransfer();
        }
    }
}

void TcpClientHandler::onBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes)

    // Если файл еще не полностью отправлен, продолжаем отправку
    if (m_currentFile && m_currentFile->isOpen() && m_bytesWritten < m_fileSize) {
        sendFileData();
    }
}

void TcpClientHandler::finishFileTransfer()
{
    if (m_currentFile && m_currentFile->isOpen()) {
        m_currentFile->close();
        delete m_currentFile;
        m_currentFile = nullptr;
    }

    emit fileSentSuccessfully(m_currentFilePath);

    m_status = "Файл успешно отправлен";
    emit statusChanged();
    m_progress = 100;
    emit progressChanged();

    // Очищаем для следующего файла
    m_currentFilePath.clear();
    m_fileSize = 0;
    m_bytesWritten = 0;
    emit currentFileChanged();
}

void TcpClientHandler::onConnected()
{
    m_connected = true;
    m_status = "Подключено к серверу";
    emit isConnectedChanged();
    emit statusChanged();
}

void TcpClientHandler::onDisconnected()
{
    m_connected = false;
    m_status = "Отключено от сервера";
    if (m_currentFile && m_currentFile->isOpen()) {
        m_currentFile->close();
        delete m_currentFile;
        m_currentFile = nullptr;
    }
    emit isConnectedChanged();
    emit statusChanged();
}

void TcpClientHandler::onErrorOccurred(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    m_connected = false;
    m_status = "Ошибка: " + m_socket->errorString();
    emit isConnectedChanged();
    emit statusChanged();
    emit errorOccurred(m_status);
}
