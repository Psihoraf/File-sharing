#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>

class TcpServerHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString serverStatus READ serverStatus NOTIFY serverStatusChanged)
    Q_PROPERTY(QString serverIpPort READ serverIpPort NOTIFY serverIpPortChanged)
    Q_PROPERTY(QString receivedFileName READ receivedFileName NOTIFY receivedFileNameChanged)

public:
    explicit TcpServerHandler(QObject *parent = nullptr);

    QString serverStatus();
    QString serverIpPort();
    QString receivedFileName();

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();


signals:
    void serverStatusChanged();
    void serverIpPortChanged();
    void receivedFileNameChanged();
    void fileSaved(bool success, const QString &path);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *m_tcpServer;
    QTcpSocket *m_clientSocket;
    QString m_serverStatus;
    QString m_serverIpPort;
    QString m_receivedFileName;
    QFile m_file;
};

#endif // TCPSERVERHANDLER_H
