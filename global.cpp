#include "global.h"

#include <QObject>

QString Global::getTextColorName(QColor backgroundColor)
{
    int r, g, b;
    backgroundColor.getRgb(&r, &g, &b);
    double gray = r * 0.299 + g * 0.587 + b * 0.114;
    QString textColor = "black";
    if (gray < 128)
    {
        textColor = "white";
    }
    return textColor;
}

QColor Global::getTextColor(QColor backgroundColor)
{
    int r, g, b;
    backgroundColor.getRgb(&r, &g, &b);
    double gray = r * 0.299 + g * 0.587 + b * 0.114;
    QColor textColor = Qt::GlobalColor::black;
    if (gray < 128)
    {
        textColor = Qt::GlobalColor::white;
    }
    return textColor;
}

QString Global::monthAbbr(int i)
{
    QString monthAbbr[] = {QObject::tr("Jan"), QObject::tr("Feb"), QObject::tr("Mar"), QObject::tr("Apr"),
                           QObject::tr("May"), QObject::tr("Jun"), QObject::tr("Jul"), QObject::tr("Aug"),
                           QObject::tr("Sept"), QObject::tr("Oct"), QObject::tr("Nov"), QObject::tr("Dec")};
    return monthAbbr[i];
}

QString Global::monthString(int i)
{
    QString monthString[] = {QObject::tr("January"), QObject::tr("February"), QObject::tr("March"), QObject::tr("April"),
                             QObject::tr("May"), QObject::tr("June"), QObject::tr("July"), QObject::tr("August"),
                             QObject::tr("September"), QObject::tr("October"), QObject::tr("November"), QObject::tr("December")};
    return monthString[i];
}

QString Global::dayOfWeekAbbr(int i)
{
    QString dayOfWeekAbbr[] = {QObject::tr("Sun"), QObject::tr("Mon"), QObject::tr("Tue"),
                               QObject::tr("Wed"), QObject::tr("Thur"), QObject::tr("Fri"),
                               QObject::tr("Sat")};
    return dayOfWeekAbbr[i];
}

QString Global::dayOfWeekString(int i)
{
    QString dayOfWeekString[] = {QObject::tr("Monday"), QObject::tr("Tuesday"), QObject::tr("Wednesday"),
                                 QObject::tr("Thursday"), QObject::tr("Friday"), QObject::tr("Saturday"),
                                 QObject::tr("Sunday")};
    return dayOfWeekString[i];
}
