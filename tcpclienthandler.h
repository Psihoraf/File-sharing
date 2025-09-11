#ifndef TCPCLIENTHANDLER_H
#define TCPCLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>

class TcpClientHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString currentFile READ currentFile NOTIFY currentFileChanged)

public:
    explicit TcpClientHandler(QObject *parent = nullptr);

    bool isConnected();
    QString status();
    int progress();
    QString currentFile();

    Q_INVOKABLE void connectToServer(const QString &address, quint16 port);
    Q_INVOKABLE void disconnectFromServer();
    Q_INVOKABLE void sendFile(const QString &filePath);

signals:
    void isConnectedChanged();
    void statusChanged();
    void progressChanged();
    void currentFileChanged();
    void fileSentSuccessfully(const QString &fileName);
    void errorOccurred(const QString &errorMessage);

private slots:
    void onConnected();
    void onDisconnected();
    void onBytesWritten(qint64 bytes);
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    void sendFileData();
    void finishFileTransfer();

    QTcpSocket *m_socket;
    QFile *m_currentFile;
    qint64 m_fileSize;
    qint64 m_bytesWritten;
    QString m_status;
    bool m_connected;
    int m_progress;
    QString m_currentFilePath;
};

#endif // TCPCLIENTHANDLER_H
