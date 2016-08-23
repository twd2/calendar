#include "todoedit.h"
#include "global.h"

#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QDebug>

TodoEdit::TodoEdit(QWidget *parent)
    : QDialog(parent), mainLayout(new QVBoxLayout(this)), boxLayout(new QHBoxLayout()),
      textboxLayout(new QHBoxLayout()), okLayout(new QHBoxLayout())
{
    mainLayout->addLayout(boxLayout);
    mainLayout->addLayout(textboxLayout);
    mainLayout->addLayout(okLayout);
    setLayout(mainLayout);

    initControllers();
}

void TodoEdit::initControllers()
{
    yearBox = new QComboBox(this);
    yearBox->addItem(tr("any year"), -1);
    for (int i = 2000; i < 2100; ++i)
    {
        yearBox->addItem(QString::number(i), i);
    }
    connect(yearBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setYearByIndex(int)));
    yearBox->show();
    boxLayout->addWidget(yearBox);

    monthBox = new QComboBox(this);
    monthBox->addItem(tr("any month"), -1);
    for (int i = 1; i <= 12; ++i)
    {
        monthBox->addItem(Global::monthString[i - 1], i);
    }
    connect(monthBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMonthByIndex(int)));
    monthBox->show();
    boxLayout->addWidget(monthBox);

    dayBox = new QComboBox(this);
    dayBox->addItem(tr("any day"), -1);
    for (int i = 1; i <= 31; ++i)
    {
        dayBox->addItem(QString::number(i), i);
    }
    connect(dayBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDayByIndex(int)));
    dayBox->show();
    boxLayout->addWidget(dayBox);

    dayOfWeekBox = new QComboBox(this);
    dayOfWeekBox->addItem(tr("any day of week"), -1);
    for (int i = 1; i <= 7; ++i)
    {
        dayOfWeekBox->addItem(Global::dayOfWeekString[i - 1], i);
    }
    connect(dayOfWeekBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDayOfWeekByIndex(int)));
    dayOfWeekBox->show();
    boxLayout->addWidget(dayOfWeekBox);

    _item.year = _item.month = _item.day = _item.dayOfWeek = -1;

    labText = new QLabel(tr("Things To Do:"), this);
    textboxLayout->addWidget(labText);
    labText->show();

    text = new QLineEdit(this);
    textboxLayout->addWidget(text);
    text->setText(_item.text);
    text->show();

    QPushButton *btnCancel = new QPushButton(tr("Cancel"), this);
    btnCancel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
    btnCancel->show();
    okLayout->addWidget(btnCancel);

    QPushButton *btnOK = new QPushButton(tr("OK"), this);
    btnOK->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    connect(btnOK, SIGNAL(clicked(bool)), this, SLOT(ok()));
    btnOK->setDefault(true);
    btnOK->show();
    okLayout->addWidget(btnOK);
    okLayout->setAlignment(Qt::AlignRight);
}

void TodoEdit::setYearByIndex(int index)
{
    setYear(yearBox->itemData(index).toInt());
}

void TodoEdit::setMonthByIndex(int index)
{
    setMonth(monthBox->itemData(index).toInt());
}

void TodoEdit::setDayByIndex(int index)
{
    setDay(dayBox->itemData(index).toInt());
}

void TodoEdit::setDayOfWeekByIndex(int index)
{
    setDayOfWeek(dayOfWeekBox->itemData(index).toInt());
}

void TodoEdit::setYear(int year)
{
    _item.year = year;
    updateDays();
}

void TodoEdit::setMonth(int month)
{
    _item.month = month;
    updateDays();
}

void TodoEdit::setDay(int day)
{
    _item.day = day;
}

void TodoEdit::setDayOfWeek(int dow)
{
    _item.dayOfWeek = dow;
}

void TodoEdit::updateDays()
{
    int days = 31;
    if (_item.month == 2)
    {
        days = 29;
    }
    if (_item.year > 0 && _item.month > 0)
    {
        QDate date(_item.year, _item.month, 1);
        days = date.daysInMonth();
    }
    int dayIndexBackup = dayBox->currentIndex();
    dayBox->clear();
    dayBox->addItem(tr("any day"), -1);
    for (int i = 1; i <= days; ++i)
    {
        dayBox->addItem(QString::number(i), i);
    }
    if (dayIndexBackup < dayBox->count())
    {
        dayBox->setCurrentIndex(dayIndexBackup);
    }
}

void TodoEdit::ok()
{
    _item.text = text->text();
    // check day of week
    int year = yearBox->currentData().toInt(),
        month = monthBox->currentData().toInt(),
        day = dayBox->currentData().toInt(),
        dow = dayOfWeekBox->currentData().toInt();
    qDebug() << year << month << day << dow;
    if (year > 0 && month > 0 && day > 0 && dow > 0)
    {
        QDate date(year, month, day);
        if (dow != date.dayOfWeek())
        {
            if (QMessageBox(QMessageBox::Warning, tr("Warning"), tr("The rule cannot match any day, continue?"),
                            QMessageBox::Ok | QMessageBox::Cancel).exec() == QMessageBox::Cancel)
            {
                qDebug() << "cancelled";
                return;
            }
        }
    }
    accept();
}

void TodoEdit::setItem(TodoItem item)
{
    _item = item;
    text->setText(_item.text);

    if (_item.day > 0)
    {
        dayBox->setCurrentIndex(_item.day);
    }
    else
    {
        dayBox->setCurrentIndex(0);
    }

    if (_item.year > 0)
    {
        yearBox->setCurrentIndex(_item.year - 1999);
    }
    else
    {
        yearBox->setCurrentIndex(0);
    }

    if (_item.month > 0)
    {
        monthBox->setCurrentIndex(_item.month);
    }
    else
    {
        monthBox->setCurrentIndex(0);
    }

    if (_item.dayOfWeek > 0)
    {
        dayOfWeekBox->setCurrentIndex(_item.dayOfWeek);
    }
    else
    {
        dayOfWeekBox->setCurrentIndex(0);
    }

    updateDays();
}

TodoItem TodoEdit::item()
{
    return _item;
}
