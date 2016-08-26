#include "filestorage.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

FileStorage::FileStorage()
{

}

FileInfo FileStorage::putFile(const QString &source, const QDate &date)
{
    QDir dir(date.toString("yyyyMMdd"));
    QFileInfo file(source);
    FileInfo fi;
    fi.fileName = file.fileName();
    fi.id = dir.filePath(fi.fileName);
    // TODO progress callback
    if (!dir.exists())
    {
        QDir::root().mkdir(dir.absolutePath());
    }
    qDebug() << "Copying" << file.absoluteFilePath() << "to" << fi.id;
    QFile::copy(file.absoluteFilePath(), fi.id);
    return fi;
}

QVector<FileInfo> FileStorage::getFileList(const QDate &date)
{
    QVector<FileInfo> fileInfos;
    QDir dir(date.toString("yyyyMMdd"));
    if (!dir.exists())
    {
        return fileInfos;
    }
    for (QFileInfo file : dir.entryInfoList())
    {
        if (file.isFile() && !file.isHidden())
        {
            FileInfo fi;
            fi.fileName = file.fileName();
            fi.id = dir.filePath(fi.fileName);
            fileInfos.append(fi);
        }
    }
    return fileInfos;
}

int FileStorage::getFileCount(const QDate &date)
{
    QDir dir(date.toString("yyyyMMdd"));
    if (!dir.exists())
    {
        return 0;
    }
    int count = 0;
    for (QFileInfo file : dir.entryInfoList())
    {
        if (file.isFile() && !file.isHidden())
        {
            ++count;
        }
    }
    return count;
}

QString FileStorage::getFilePath(const QString &id)
{
    // when we use filesystem rather than database to store user's files, id is actually the sub path.
    return QFileInfo(id).absoluteFilePath();
}

QByteArray FileStorage::getFileData(const QString &id)
{
    QFile file(id);
    file.open(QFile::ReadOnly);
    return file.readAll();
}

void FileStorage::delFile(const QString &id)
{
    QFile::remove(getFilePath(id));
}
