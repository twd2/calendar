#include "todoitem.h"

#include <QStringList>
#include <QDate>

TodoItem::TodoItem()
    : year(QDate::currentDate().year()),
      month(QDate::currentDate().month()),
      day(QDate::currentDate().day())
{
    // thing = QDateTime::currentDateTime().toString();
}

bool TodoItem::match(const QDate &date) const
{
    return (year == -1 || year == date.year())
           && (month == -1 || month == date.month())
           && (day == -1 || day == date.day())
           && (dayOfWeek == -1 || dayOfWeek == date.dayOfWeek());
}

QString TodoItem::matchToString() const
{
    QString monthString[] = {QObject::tr("January"), QObject::tr("February"), QObject::tr("March"), QObject::tr("April"),
                             QObject::tr("May"), QObject::tr("June"), QObject::tr("July"), QObject::tr("August"),
                             QObject::tr("September"), QObject::tr("October"), QObject::tr("November"), QObject::tr("December")};
    QString dayOfWeekString[] = {QObject::tr("Monday"), QObject::tr("Tuesday"), QObject::tr("Wednesday"),
                                 QObject::tr("Thursday"), QObject::tr("Friday"), QObject::tr("Saturday"),
                                 QObject::tr("Sunday")};
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
        list << monthString[month - 1];
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
        list << QObject::tr("only %1").arg(dayOfWeekString[dayOfWeek - 1]);
    }
    return list.join(QObject::tr(", "));
}

TodoItem::~TodoItem()
{

}
