#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QColor>

class Global
{
public:
    static QString monthAbbr[];
    static QString monthString[];
    static QString dayOfWeekAbbr[];
    static QString dayOfWeekString[];
    static QString getTextColorName(QColor backgroundColor);
    static QColor getTextColor(QColor backgroundColor);
    Global() = delete;
};

#endif // GLOBAL_H
