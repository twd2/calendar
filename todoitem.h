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
    int year = -1, month = -1, day = -1, dayOfWeek = -1,
        hour = 0, minute = 0, second = 0;
    QString except = "|";
    QString text = "";
    QColor color = Qt::GlobalColor::white;
    TodoItem();
    explicit TodoItem(const QDate &);
    bool isMulti() const;
    bool match(const QDate &) const;
    bool fullMatch(const QDate &) const;
    void addExcept(const QDate &);
    QString ruleToString() const;
    ~TodoItem();
};

#endif // TODOITEM_H
