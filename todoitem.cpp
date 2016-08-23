#include "todoitem.h"

#include <QStringList>

TodoItem::TodoItem()
{

}

bool TodoItem::match(QDate &date)
{
    return (year == -1 || year == date.year())
           && (month == -1 || month == date.month())
           && (day == -1 || day == date.day())
           && (dayOfWeek == -1 || dayOfWeek == date.dayOfWeek());
}

QString TodoItem::matchToString()
{
    QString monthString[] = {tr("January"), tr("February"), tr("March"), tr("April"),
                             tr("May"), tr("June"), tr("July"), tr("August"),
                             tr("September"), tr("October"), tr("November"), tr("December")};
    QString dayOfWeekString[] = {tr("Monday"), tr("Tuesday"), tr("Wednesday"),
                                 tr("Thursday"), tr("Friday"), tr("Saturday"),
                                 tr("Sunday")};
    QStringList list;
    if (year == -1)
    {
        list << tr("any year");
    }
    else
    {
        list << tr("year %1").arg(year);
    }

    if (month == -1)
    {
        list << tr("any month");
    }
    else
    {
        list << monthString[month - 1];
    }

    if (day == -1)
    {
        list << tr("any day");
    }
    else
    {
        list << QString::number(day);
    }

    if (dayOfWeek != -1)
    {
        list << tr("only %1").arg(dayOfWeekString[dayOfWeek - 1]);
    }
    return list.join(tr(", "));
}

TodoItem::~TodoItem()
{

}
