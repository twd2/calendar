#include "todoitem.h"
#include "global.h"

#include <QStringList>
#include <QDate>

TodoItem::TodoItem(const QDate &date)
    : year(date.year()),
      month(date.month()),
      day(date.day())
{

}

TodoItem::TodoItem()
    : TodoItem(QDate::currentDate())
{

}

bool TodoItem::isMulti() const
{
    return (year == -1 || month == -1 || day == -1);
}

bool TodoItem::match(const QDate &date) const
{
    return (year == -1 || year == date.year())
           && (month == -1 || month == date.month())
           && (day == -1 || day == date.day())
           && (dayOfWeek == -1 || dayOfWeek == date.dayOfWeek())
           && except.indexOf(date.toString("yyyyMMdd")) == -1; // not in except list
}

bool TodoItem::fullMatch(const QDate &date) const
{
    return year == date.year()
           && month == date.month()
           && day == date.day();
}

QString TodoItem::ruleToString() const
{
    QStringList list;
    if (year == -1)
    {
        list << QObject::tr("any year");
    }
    else
    {
        list << QObject::tr("year %1").arg(year);
    }

    if (month == -1)
    {
        list << QObject::tr("any month");
    }
    else
    {
        list << Global::monthString(month - 1);
    }

    if (day == -1)
    {
        list << QObject::tr("any day");
    }
    else
    {
        list << QString::number(day);
    }

    if (dayOfWeek != -1)
    {
        list << QObject::tr("only %1").arg(Global::dayOfWeekString(dayOfWeek - 1));
    }
    return list.join(QObject::tr(", "));
}

void TodoItem::addExcept(const QDate &date)
{
    except += date.toString("yyyyMMdd") + "|";
}

TodoItem::~TodoItem()
{

}
