#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), calendar(new Calendar(this))
{
    // calendar->setWindowOpacity(0.7);
    calendar->show();
}
