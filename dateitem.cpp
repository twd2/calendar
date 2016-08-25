#include "dateitem.h"
#include "global.h"

#include <QPaintEvent>
#include <QPainter>
#include <QTextDocument>

DateItem::DateItem(QWidget *parent) : QWidget(parent)
{

}

void DateItem::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.fillRect(rect(), colorBackground);

    if (_enabled)
    {
        if (_date.dayOfWeek() >= 1 && _date.dayOfWeek() <= 5)
        {
            p.setPen(_colorWeekdayText);
        }
        else
        {
            p.setPen(_colorWeekendText);
        }
    }
    else
    {
        p.setPen(_colorDisabledText);
    }
    QFont f;
    //f.setPixelSize(13);
    //p.setFont(f);
    QTextDocument doc;
    doc.setHtml(_text);
    doc.drawContents(&p);
    //p.drawText(rect(), Qt::AlignTop | Qt::AlignLeft, _text);
    f.setPixelSize(13);
    f.setBold(true);
    p.setFont(f);
    p.drawText(rect(), Qt::AlignCenter, QString::number(_date.day()));

    if (_selected && _enabled)
    {
        p.fillRect(this->rect(), QColor::fromRgba64(0, 0, 0, 16384));
    }
}

void DateItem::mouseReleaseEvent(QMouseEvent *e)
{
    if (rect().contains(e->pos()))
    {
        emit clicked();
    }
    e->ignore();
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
    this->_colorWeekdayText = color;
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
    return _colorWeekdayText;
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

void DateItem::setBackgroundColor(QColor color)
{
    this->colorBackground = color;
    this->_colorWeekdayText = Global::getTextColor(color);
}
