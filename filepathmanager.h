#ifndef FILEPATHMANAGER_H
#define FILEPATHMANAGER_H

#include <QObject>
#include <QStandardPaths>
#include <QDir>

class FilePathManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString savePath READ savePath WRITE setSavePath NOTIFY savePathChanged)

public:
    explicit FilePathManager(QObject *parent = nullptr);

    QString savePath() const;

    Q_INVOKABLE void setSavePath(const QString &path);
    Q_INVOKABLE void setToDownloads();
    Q_INVOKABLE QString getFilePath(const QString &fileName);

signals:
    void savePathChanged();

private:
    QString m_savePath;
};

#endif // FILEPATHMANAGER_H
