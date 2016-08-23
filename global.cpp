#include "global.h"

#include <QObject>

QString Global::monthAbbr[] = {QObject::tr("Jan"), QObject::tr("Feb"), QObject::tr("Mar"), QObject::tr("Apr"),
                               QObject::tr("May"), QObject::tr("Jun"), QObject::tr("Jul"), QObject::tr("Aug"),
                               QObject::tr("Sept"), QObject::tr("Oct"), QObject::tr("Nov"), QObject::tr("Dec")};
QString Global::monthString[] = {QObject::tr("January"), QObject::tr("February"), QObject::tr("March"), QObject::tr("April"),
                                 QObject::tr("May"), QObject::tr("June"), QObject::tr("July"), QObject::tr("August"),
                                 QObject::tr("September"), QObject::tr("October"), QObject::tr("November"), QObject::tr("December")};
QString Global::dayOfWeekAbbr[] = {QObject::tr("Sun"), QObject::tr("Mon"), QObject::tr("Tue"),
                                   QObject::tr("Wed"), QObject::tr("Thur"), QObject::tr("Fri"),
                                   QObject::tr("Sat")};
QString Global::dayOfWeekString[] = {QObject::tr("Monday"), QObject::tr("Tuesday"), QObject::tr("Wednesday"),
                                     QObject::tr("Thursday"), QObject::tr("Friday"), QObject::tr("Saturday"),
                                     QObject::tr("Sunday")};

Global::Global()
{

}
