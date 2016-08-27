#ifndef GLOBAL_H
#define GLOBAL_H

#include <QColor>
#include <QDate>
#include <QRect>
#include <QString>

class Global
{
public:
    static QString monthAbbr(int);
    static QString monthString(int);
    static QString dayOfWeekAbbr(int);
    static QString dayOfWeekString(int);
    static QString getTextColorName(QColor backgroundColor);
    static QColor getTextColor(QColor backgroundColor);
    static QRect getScreen();
    static QString getFestival(const QDate &);
    Global() = delete;
};

#endif // GLOBAL_H
