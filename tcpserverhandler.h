#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

class TcpServerHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(QString receivedFileName READ receivedFileName NOTIFY receivedFileNameChanged)

public:
    explicit TcpServerHandler(QObject *parent = nullptr);

    QString serverAddress() const;
    bool isRunning() const;
    QString receivedFileName() const;

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();

signals:
    void serverAddressChanged();
    void isRunningChanged();
    void receivedFileNameChanged();
    void fileReceived(const QString &fileName);        // ДОБАВЛЯЕМ ЭТОТ СИГНАЛ
    void errorOccurred(const QString &errorMessage);   // ДОБАВЛЯЕМ ЭТОТ СИГНАЛ

private slots:
    void onNewConnection();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);  // ДОБАВЛЯЕМ ЭТОТ СЛОТ

private:
    QTcpServer *m_tcpServer;
    QTcpSocket *m_clientSocket;
    QString m_serverAddress;
    bool m_isRunning;
    QString m_receivedFileName;
};

#endif // TCPSERVERHANDLER_H
