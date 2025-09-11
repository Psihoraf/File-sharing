#include "qrgenerator.h"

QrGenerator::QrGenerator(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}


void QrGenerator::paint(QPainter *painter)
{
    if (m_link.isEmpty())
        return;

    QRcode *qrcode = QRcode_encodeString(m_link.toUtf8().constData(),
                                                 0, QR_ECLEVEL_Q, QR_MODE_8, 1);
    if (!qrcode)
        return;

    int scale = width() / qrcode->width;
    painter->fillRect(boundingRect(), Qt::white);

    for (int y = 0; y < qrcode->width; ++y) {
        for (int x = 0; x < qrcode->width; ++x) {
            if (qrcode->data[y * qrcode->width + x] & 1) {
                painter->fillRect(x * scale, y * scale, scale, scale, Qt::black);
            }
        }
    }

    QRcode_free(qrcode);
}

void QrGenerator::generatefromconfig(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл";
        return;
    }

    QByteArray data = file.readAll();
    file.close();


    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Неверный формат JSON";
        return;
    }

    QJsonObject root = doc.object();
    QJsonArray outbounds = root["outbounds"].toArray();
    if (outbounds.isEmpty()) return;

    QJsonObject out = outbounds[0].toObject();
    QJsonObject settings = out["settings"].toObject();
    QJsonArray vnext = settings["vnext"].toArray();
    QJsonObject vnext0 = vnext[0].toObject();
    QString address = vnext0["address"].toString();
    int port = vnext0["port"].toInt();
    QJsonArray users = vnext0["users"].toArray();
    QJsonObject user = users[0].toObject();

    QString id = user["id"].toString();
    QString flow = user["flow"].toString();
    QString encryption = user["encryption"].toString();

    QJsonObject streamSettings = out["streamSettings"].toObject();
    QString security = streamSettings["security"].toString();
    QString network = streamSettings["network"].toString();

    QJsonObject reality = streamSettings["realitySettings"].toObject();
    QString sni = reality["serverName"].toString();
    QString pbk = reality["publicKey"].toString();
    QString sid = reality["shortId"].toString();
    QString fp = reality["fingerprint"].toString();


    QUrlQuery query;
    query.addQueryItem("encryption", encryption);
    query.addQueryItem("flow", flow);
    query.addQueryItem("security", security);
    query.addQueryItem("fp", fp);
    query.addQueryItem("sni", sni);
    query.addQueryItem("pbk", pbk);
    query.addQueryItem("sid", sid);
    query.addQueryItem("type", network);

    QString vlessUrl = QString("vless://%1@%2:%3?%4#VLESS-Reality")
            .arg(id)
            .arg(address)
            .arg(port)
            .arg(query.toString(QUrl::FullyEncoded));

    qDebug() << vlessUrl;
    m_link = vlessUrl;
    update();


}

void QrGenerator::setlink(const QString& link)
{
    if (m_link != link)
    {
        m_link = link;
        update();
        emit linkChanged();
    }
}

void QrGenerator::generatefromconfig(const QString &id,
                      const QString &host,
                      int port,
                      const QString &flow,
                      const QString &fingerprint,
                      const QString &serverName,
                      const QString &publicKey,
                      const QString &shortId,
                      const QString &network,
                      const QString &encryption,
                      const QString &security,
                      const QString &label)
{
    QUrlQuery query;
    query.addQueryItem("encryption", encryption);
    if (!flow.isEmpty())
        query.addQueryItem("flow", flow);
    query.addQueryItem("security", security);
    query.addQueryItem("fp", fingerprint);
    query.addQueryItem("sni", serverName);
    query.addQueryItem("pbk", publicKey);
    query.addQueryItem("sid", shortId);
    query.addQueryItem("type", network);

    QString url = QString("vless://%1@%2:%3?%4#%5")
            .arg(id)
            .arg(host)
            .arg(port)
            .arg(query.toString(QUrl::FullyEncoded))
            .arg(label);

    m_link = url;
    update();
}


