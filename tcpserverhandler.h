#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QMap>
#include <QSet>
#include "filepathmanager.h"

// Структура для хранения информации о клиенте
struct ClientConnection {
    QTcpSocket* socket;
    qint64 expectedFileSize;
    QString expectedFileName;
    QFile* outputFile;
    qint64 bytesReceived;
    QString receivedFileName;
};

class TcpServerHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(int activeConnections READ activeConnections NOTIFY activeConnectionsChanged)
    Q_PROPERTY(FilePathManager* pathManager READ pathManager CONSTANT)

public:
    explicit TcpServerHandler(QObject *parent = nullptr);
    ~TcpServerHandler();

    QString serverAddress() const;
    bool isRunning() const;
    int activeConnections() const;
    FilePathManager* pathManager() const;

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();
    Q_INVOKABLE void disconnectAllClients();

signals:
    void serverAddressChanged();
    void isRunningChanged();
    void activeConnectionsChanged();
    void fileReceived(const QString &fileName, const QString &clientAddress);
    void errorOccurred(const QString &errorMessage);
    void clientConnected(const QString &clientAddress);
    void clientDisconnected(const QString &clientAddress);

private slots:
    void onNewConnection();
    void onClientReadyRead();
    void onClientDisconnected();

private:
    void cleanupClient(QTcpSocket* clientSocket);
    void processClientData(QTcpSocket* clientSocket);
    QString getClientIdentifier(QTcpSocket* clientSocket) const;

private:
    QTcpServer *m_tcpServer;
    QString m_serverAddress;
    bool m_isRunning;
    FilePathManager *m_pathManager;

    // Хранилище активных подключений
    QMap<QTcpSocket*, ClientConnection> m_clients;
    const int MAX_CONNECTIONS = 10; // Максимальное количество одновременных подключений
};

#endif // TCPSERVERHANDLER_H
