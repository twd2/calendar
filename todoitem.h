#ifndef TODOITEM_H
#define TODOITEM_H

#include <QDate>
#include <QString>
#include <QObject>

class TodoItem
    : public QObject
{
    Q_OBJECT
public:
    // -1 means to match all
    int year, month, day, dayOfWeek;
    TodoItem();
    bool match(QDate &);
    QString matchToString();
    ~TodoItem();
};

#endif // TODOITEM_H
