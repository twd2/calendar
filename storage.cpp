#include "storage.h"

Storage *Storage::instance = nullptr;

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
    return item.id;
}

void Storage::del(int ID)
{
    if (ID >= 0 && ID <= _items.count() - 1)
    {
        _items.removeAt(ID);
    }
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
