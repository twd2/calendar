#ifndef STORAGE_H
#define STORAGE_H

#include "todoitem.h"

#include <QVector>
#include <QString>

class Storage
{
private:
    QVector<TodoItem> _items;
    static Storage *instance;
public:
    Storage();
    static Storage *i();
    QVector<TodoItem> get(const QDate &date);
    int add(TodoItem item);
    void del(int ID);
    QString putFile(QString source);
    QString getFile(const QString &ID);
    void Save();
    // QByteArray getFileData(const QString &ID);
};

#endif // STORAGE_H
