#include "dateitem.h"
#include <QPaintEvent>
#include <QPainter>

DateItem::DateItem(QWidget *parent) : QWidget(parent)
{

}

void DateItem::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    if (_selected && _enabled)
    {
        p.fillRect(this->rect(), _colorSelected);
    }
    else
    {
        p.fillRect(this->rect(), Qt::GlobalColor::white);
    }

    if (_enabled)
    {
        if (_date.dayOfWeek() >= 1 && _date.dayOfWeek() <= 5)
        {
            p.setPen(_colorWeekday);
        }
        else
        {
            p.setPen(_colorWeekend);
        }
    }
    else
    {
        p.setPen(_colorDisabled);
    }
    QFont f;
    f.setPixelSize(24);
    p.setFont(f);
    p.drawText(rect(), Qt::AlignCenter, QString::number(_date.day()));
    f.setPixelSize(15);
    p.setFont(f);
    p.drawText(rect(), Qt::AlignTop | Qt::AlignLeft, _text);
}

void DateItem::mouseReleaseEvent(QMouseEvent *e)
{
    if (rect().contains(e->pos()))
    {
        emit clicked();
    }
}

void DateItem::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (_enabled)
    {
        emit doubleClicked();
    }
}

void DateItem::setColor(QColor color)
{
    this->_colorWeekday = color;
    update();
}

void DateItem::setText(QString text)
{
    this->_text = text;
    update();
}

void DateItem::setDate(QDate date)
{
    this->_date = date;
    update();
}

QColor DateItem::color()
{
    return _colorWeekday;
}

QString DateItem::text()
{
    return _text;
}

QDate DateItem::date()
{
    return _date;
}

void DateItem::setEnabled(bool enabled)
{
    this->_enabled = enabled;
    update();
}

bool DateItem::enabled()
{
    return _enabled;
}

bool DateItem::selected()
{
    return _selected;
}

void DateItem::setSelected(bool selected)
{
    _selected = selected;
    update();
}
