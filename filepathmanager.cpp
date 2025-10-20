#include "filepathmanager.h"
#include <QDebug>

FilePathManager::FilePathManager(QObject *parent)
    : QObject(parent)
{
    // Устанавливаем папку загрузок по умолчанию
    setToDownloads();
}

QString FilePathManager::savePath() const
{
    return m_savePath;
}

void FilePathManager::setSavePath(const QString &path)
{
    if (m_savePath == path)
        return;

    QDir dir(path);

    // Создаем папку, если не существует
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    m_savePath = dir.absolutePath();
    qDebug() << "Путь сохранения установлен:" << m_savePath;
    emit savePathChanged();
}

void FilePathManager::setToDownloads()
{
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    // Если путь пустой, создаем папку Downloads в домашней директории
    if (downloadsPath.isEmpty()) {
        downloadsPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Downloads";
    }

    setSavePath(downloadsPath);
}

QString FilePathManager::getFilePath(const QString &fileName)
{
    QDir dir(m_savePath);
    QString filePath = dir.absoluteFilePath(fileName);

    // Если файл уже существует, добавляем номер
    if (QFile::exists(filePath)) {
        QFileInfo fileInfo(filePath);
        QString baseName = fileInfo.baseName();
        QString suffix = fileInfo.completeSuffix();

        int counter = 1;
        QString newFilePath;

        do {
            QString newFileName = baseName + " (" + QString::number(counter) + ")";
            if (!suffix.isEmpty()) {
                newFileName += "." + suffix;
            }
            newFilePath = dir.absoluteFilePath(newFileName);
            counter++;
        } while (QFile::exists(newFilePath));

        return newFilePath;
    }

    return filePath;
}
