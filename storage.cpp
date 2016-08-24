#include "storage.h"
#include "todoitem.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

Storage *Storage::instance = nullptr;
const QString Storage::defaultFileName = "calendar.json";

Storage::Storage()
{

}

Storage *Storage::i()
{
    if (!instance)
    {
        instance = new Storage();
    }
    return instance;
}

int Storage::add(TodoItem item)
{
    item.id = _items.count();
    _items.append(item);
    save();
    return item.id;
}

void Storage::del(int id)
{
    if (id >= 0 && id <= _items.count() - 1)
    {
        _items.removeAt(id);
    }
    save();
}

QVector<TodoItem> Storage::get(const QDate &date)
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

TodoItem Storage::get(int id)
{
    return _items[id];
}

void Storage::set(int id, const TodoItem &item)
{
    _items[id] = item;
    save();
}

void Storage::load()
{
    load(defaultFileName);
}

void Storage::save()
{
    save(defaultFileName);
}

void Storage::load(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray array = doc.array();
    _items.clear();
    for (const auto &ref : array)
    {
        QJsonObject obj = ref.toObject();
        TodoItem i;
        i.year = obj["year"].toInt();
        i.month = obj["month"].toInt();
        i.day = obj["day"].toInt();
        i.dayOfWeek = obj["dayOfWeek"].toInt();
        i.text = obj["text"].toString();

        auto colorArray = obj["color"].toArray();
        QColor c = QColor::fromRgb(colorArray[0].toInt(),
                                   colorArray[1].toInt(),
                                   colorArray[2].toInt());
        i.color = c;
        _items.append(i);
    }
}

void Storage::save(const QString &fileName)
{
    QJsonArray array;
    for (const TodoItem &i : _items)
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
        array.append(obj);
    }

    QJsonDocument doc(array);
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.write(doc.toJson());
}
