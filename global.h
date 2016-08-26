#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QColor>
#include <QRect>

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
    Global() = delete;
};

#endif // GLOBAL_H
