#include "tcpserverhandler.h"
#include <QNetworkInterface>
#include <QDateTime>
#include <QDir>
TcpServerHandler::TcpServerHandler(QObject *parent)
    : QObject{parent}, m_tcpServer(nullptr), m_clientSocket(nullptr)
{
    m_serverStatus = "Server stoped";
}

QString TcpServerHandler::serverStatus(){
    return m_serverStatus;
}

QString TcpServerHandler::serverIpPort(){
    return m_serverIpPort;
}

QString TcpServerHandler::receivedFileName(){
    return m_receivedFileName;
}

void TcpServerHandler::startServer(){
    if(m_tcpServer){
        stopServer();
    }

    m_tcpServer = new QTcpServer(this);
    connect (m_tcpServer, &QTcpServer::newConnection, this, &TcpServerHandler::onNewConnection);

    if (m_tcpServer->listen(QHostAddress::Any, 0)){
        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for (const QHostAddress &address:ipAddressesList) {
            if(address != QHostAddress::LocalHost&&address.toIPv4Address()){
                ipAddress = address.toString();
                break;
            }

        }
        if (ipAddress.isEmpty()){
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        }

        m_serverIpPort = ipAddress + ":" +QString::number(m_tcpServer->serverPort());
        m_serverStatus = "Server started. Waiting to connecting";

        qDebug() << "Server started on" << m_serverIpPort;
        emit serverIpPortChanged();
        emit serverStatusChanged();

    }
    else{
        m_serverStatus = "Error: " + m_tcpServer->errorString();
    }
    qDebug() << m_serverStatus;
    emit serverStatusChanged();
}

void TcpServerHandler::stopServer(){
    if (m_tcpServer){
        m_tcpServer->close();
        m_tcpServer->deleteLater();
        m_tcpServer = nullptr;
    }
    if(m_clientSocket){
        m_clientSocket->close();
        m_clientSocket->deleteLater();
        m_clientSocket=nullptr;
    }

    m_serverStatus = "Server stopped";
    m_serverIpPort = "";
    emit serverStatusChanged();
    emit serverIpPortChanged();
}

void TcpServerHandler::onNewConnection(){
    m_clientSocket = m_tcpServer->nextPendingConnection();
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &TcpServerHandler::onReadyRead);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &TcpServerHandler::onClientDisconnected);

    m_serverStatus = "Client was connected";
    emit serverStatusChanged();

    m_receivedFileName = "received_file_" +QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+".dat";
    m_file.setFileName(m_receivedFileName);

    if (!m_file.open(QIODevice::WriteOnly)){
        qDebug() << "Couldn't open file to writing:" << m_receivedFileName;
        m_clientSocket->disconnectFromHost();
    }
    emit receivedFileNameChanged();
}

void TcpServerHandler::onReadyRead(){
    if(!m_clientSocket||!m_file.isOpen()) return;
    QByteArray data = m_clientSocket->readAll();
    m_file.write(data);
}

void TcpServerHandler::onClientDisconnected(){
    m_file.close();
    m_clientSocket->deleteLater();
    m_clientSocket = nullptr;

    m_serverStatus = "Client disconnected. File saved:" + m_receivedFileName;
    emit serverStatusChanged();
    emit fileSaved(true, QDir::toNativeSeparators(m_file.fileName()));
}
