#ifndef HOTSPOTCHAT_H
#define HOTSPOTCHAT_H

#include <QQmlEngine>
#include <QUdpSocket>
#include "filepathmanager.h"  // Добавляем

class HotspotChat : public QUdpSocket
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(quint16 port READ port NOTIFY portChanged FINAL)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged FINAL)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged FINAL)
    Q_PROPERTY(QVariantList messages READ messages NOTIFY messagesChanged FINAL)
    Q_PROPERTY(ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged FINAL)
    Q_PROPERTY(FilePathManager* pathManager READ pathManager CONSTANT FINAL)  // Добавляем свойство

public:
    enum ConnectionState {
        Disconnected,
        ConnectedToRouter,
        ConnectedToPeer
    }; Q_ENUM(ConnectionState)

public:
    explicit HotspotChat(QObject *parent = nullptr);
    ~HotspotChat();

    quint16 port();

    QUrl url() const;
    void setUrl(const QUrl &url);

    bool connected();

    QVariantList messages() const;

    FilePathManager* pathManager() const;  // Добавляем геттер

    Q_INVOKABLE void send(QString text);

    Q_INVOKABLE void greetAddress(QUrl url);

    Q_INVOKABLE void sendFile(QUrl url);

    ConnectionState connectionState() const;

private:
    void setConnectionState(ConnectionState newConnectionState);

private:
    quint16 m_port = 0;
    QUrl m_url;
    QVariantList m_messages;
    FilePathManager* m_pathManager;  // Добавляем указатель
    ConnectionState m_connectionState = Disconnected;

signals:
    void portChanged();
    void urlChanged();
    void connectedChanged();
    void messagesChanged();
    void redirected(QUrl url);
    void connectionStateChanged();

private slots:
    void readyRead();
};

#endif // HOTSPOTCHAT_H
