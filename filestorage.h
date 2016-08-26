#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include "fileinfo.h"

#include <QDate>
#include <QString>

class FileStorage
{
public:
    FileStorage();
    virtual ~FileStorage() = default;

    virtual FileInfo putFile(const QString &source, const QDate &date);
    virtual QVector<FileInfo> getFileList(const QDate &date);
    virtual int getFileCount(const QDate &date);
    virtual QString getFilePath(const QString &id);
    virtual QByteArray getFileData(const QString &id);
    virtual void delFile(const QString &id);

};

#endif // FILESTORAGE_H
