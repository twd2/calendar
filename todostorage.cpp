#include "todostorage.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>

const QString TodoStorage::defaultFileName = "calendar.json";

TodoStorage::TodoStorage()
{

}

int TodoStorage::add(TodoItem item)
{
    item.id = _items.count();
    _items.append(item);
    save();
    return item.id;
}

void TodoStorage::del(int id)
{
    if (id >= 0 && id <= _items.count() - 1)
    {
        _items.removeAt(id);
    }
    save();
}

QVector<TodoItem> TodoStorage::get(const QDate &date)
{
    QVector<TodoItem> items;
    for (int i = 0; i < _items.count(); ++i)
    {
        const TodoItem &item = _items[i];
        if (!item.match(date))
        {
            continue;
        }
        TodoItem newItem = item;
        newItem.id = i;
        items.append(newItem);
    }
    return items;
}

TodoItem TodoStorage::get(int id)
{
    return _items[id];
}

void TodoStorage::set(int id, const TodoItem &item)
{
    _items[id] = item;
    save();
}

void TodoStorage::load()
{
    importFile(defaultFileName, true);
}

void TodoStorage::save()
{
    exportFile(defaultFileName);
}

QVector<TodoItem> TodoStorage::fromJsonDoc(const QJsonDocument &doc)
{
    QJsonArray array = doc.array();

    QVector<TodoItem> items;
    for (const auto &ref : array)
    {
        QJsonObject obj = ref.toObject();
        TodoItem i;
        i.year = obj["year"].toInt();
        i.month = obj["month"].toInt();
        i.day = obj["day"].toInt();
        i.dayOfWeek = obj["dayOfWeek"].toInt();
        i.text = obj["text"].toString();
        i.except = obj["except"].toString();

        auto colorArray = obj["color"].toArray();
        QColor c = QColor::fromRgb(colorArray[0].toInt(),
                                   colorArray[1].toInt(),
                                   colorArray[2].toInt());
        i.color = c;
        items.append(i);
    }
    return items;
}

void TodoStorage::importFile(const QString &fileName, bool overwrite)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (overwrite)
    {
        _items = fromJsonDoc(doc);
    }
    else
    {
        auto items = fromJsonDoc(doc);

    }
}

QJsonDocument TodoStorage::toJsonDoc(const QVector<TodoItem> &items)
{
    QJsonArray array;
    for (const TodoItem &i : items)
    {
        QJsonObject obj;
        obj["year"] = i.year;
        obj["month"] = i.month;
        obj["day"] = i.day;
        obj["dayOfWeek"] = i.dayOfWeek;

        QJsonArray color;
        int r, g, b;
        i.color.getRgb(&r, &g, &b);
        color.append(r);
        color.append(g);
        color.append(b);
        obj["color"] = color;
        obj["text"] = i.text;
        obj["except"] = i.except;
        array.append(obj);
    }

    QJsonDocument doc(array);
    return doc;
}

void TodoStorage::exportFile(const QString &fileName)
{

    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.write(toJsonDoc(getAll()).toJson());
}

QVector<TodoItem> TodoStorage::get(const QString &query)
{

}

QVector<TodoItem> TodoStorage::getAll()
{
    return _items;
}
