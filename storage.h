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
    const QString fileName = "calendar.json";
    Storage();
    static Storage *i();
    QVector<TodoItem> get(const QDate &date);
    int add(TodoItem item);
    void del(int ID);
    TodoItem get(int ID);
    void set(int ID, const TodoItem &item);
    QString putFile(QString source);
    QString getFile(const QString &ID);
    void load();
    void save();
    // QByteArray getFileData(const QString &ID);
};

#endif // STORAGE_H
