#ifndef QRGENERATOR_H
#define QRGENERATOR_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QDebug>

#include "libqrencode/qrencode.h"


class QrGenerator : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString link READ link WRITE setlink NOTIFY linkChanged)
public:

    explicit QrGenerator(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;
    QString link() const { return m_link; }
    void setlink(const QString& link);
    Q_INVOKABLE void generatefromconfig(const QString &path);
    Q_INVOKABLE void generatefromconfig(const QString &id,
                          const QString &host,
                          int port,
                          const QString &flow,
                          const QString &fingerprint,
                          const QString &serverName,
                          const QString &publicKey,
                          const QString &shortId,
                          const QString &network = "tcp",
                          const QString &encryption = "none",
                          const QString &security = "reality",
                          const QString &label = "VLESS-Reality");
signals:

    void linkChanged();
private:

    QString m_link;
};

#endif // QRGENERATOR_H
