#ifndef TODOITEM_H
#define TODOITEM_H

#include <QDate>
#include <QString>
#include <QObject>
#include <QColor>

class TodoItem
{
public:
    // -1 means to match all
    int id = 0;
    int year = -1, month = -1, day = -1, dayOfWeek = -1;
    QString text = "";
    QColor color = Qt::GlobalColor::white;
    TodoItem();
    explicit TodoItem(const QDate &);
    bool match(const QDate &) const;
    bool fullMatch(const QDate &) const;
    QString matchToString() const;
    ~TodoItem();
};

#endif // TODOITEM_H
