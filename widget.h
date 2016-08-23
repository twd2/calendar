#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "calendar.h"

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    Calendar *calendar;
signals:

public slots:
};

#endif // WIDGET_H
