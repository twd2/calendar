#ifndef TODOSTORAGE_H
#define TODOSTORAGE_H

#include "todoitem.h"

#include <QDate>
#include <QJsonDocument>
#include <QString>
#include <QVector>

class TodoStorage
{
protected:
    QVector<TodoItem> _items;
public:
    static const QString defaultFileName;

    static QJsonDocument toJsonDoc(const QVector<TodoItem> &items);
    static QVector<TodoItem> fromJsonDoc(const QJsonDocument &doc);

    TodoStorage();

    virtual int add(const TodoItem &item);
    virtual TodoItem get(int id);
    virtual QVector<TodoItem> get(const QDate &date);
    virtual QVector<TodoItem> get(const QString &query);
    virtual QVector<TodoItem> getAll();
    virtual void set(int id, const TodoItem &item);
    virtual void delAll();
    virtual void del(int id);

    virtual void load();
    virtual void save();
    virtual void importFile(const QString &fileName, bool overwrite);
    virtual void exportFile(const QString &fileName);
};

#endif // TODOSTORAGE_H
