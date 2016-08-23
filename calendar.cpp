#include "calendar.h"
#include "dateitem.h"

#include <QLabel>
#include <QSignalMapper>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QMimeData>


Calendar::Calendar(QWidget *parent)
    : QWidget(parent), grid(new QGridLayout(this))
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAcceptDrops(true);
    setWindowOpacity(0.7);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    grid->setVerticalSpacing(1);
    grid->setHorizontalSpacing(1);
    grid->setMargin(0);
    setLayout(grid);
    initCalendar();
}

void Calendar::initCalendar()
{
    // week
    for (int y = 1; y <= 6; ++y)
    {
        QLabel *label = new QLabel("??", this);
        label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        label->show();
        grid->addWidget(label, y, 0);
    }

    // day of week
    QString dayOfWeekAbbr[] = {tr("Sun."), tr("Mon."), tr("Tue."),
                               tr("Wed."), tr("Thur."), tr("Fri."),
                               tr("Sat.")};
    for (int x = 1; x <= 7; ++x)
    {
        QString c = (x == 1 || x == 7) ? "red" : "black";
        QLabel *label = new QLabel("<font color=" + c + "><strong>" + dayOfWeekAbbr[x - 1] + "</strong></font>", this);
        label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        label->show();
        grid->addWidget(label, 0, x);
    }

    QLabel *label = new QLabel(tr("Wandai's<br>Calendar"), this);
    label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    label->show();
    grid->addWidget(label, 0, 0);

    QSignalMapper *mapperClicked = new QSignalMapper(this);
    QSignalMapper *mapperSelected = new QSignalMapper(this);
    for (int y = 1; y <= 6; ++y)
    {
        for (int x = 1; x <= 7; ++x)
        {
            DateItem *label = new DateItem(this);
            //label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
            connect(label, SIGNAL(doubleClicked()), mapperClicked, SLOT(map()));
            mapperClicked->setMapping(label, label);
            connect(label, SIGNAL(clicked()), mapperSelected, SLOT(map()));
            mapperSelected->setMapping(label, label);
            label->show();
            grid->addWidget(label, y, x);
        }
    }
    connect(mapperClicked, SIGNAL(mapped(QWidget *)), this, SLOT(itemClicked(QWidget *)));
    connect(mapperSelected, SIGNAL(mapped(QWidget *)), this, SLOT(itemSelected(QWidget *)));

    QDate today = QDate::currentDate();
    setMonth(today.year(), today.month());
}

void Calendar::itemClicked(QWidget *w)
{
    auto label = static_cast<DateItem *>(w);
    if (!label)
    {
        return;
    }
    qDebug() << label->date() << "double clicked";
    // TODO
    // label->setColor(Qt::GlobalColor::red);
}

void Calendar::itemSelected(QWidget *w)
{
    auto label = static_cast<DateItem *>(w);
    if (!label)
    {
        return;
    }
    qDebug() << label->date() << "selected";
    if (label->date().month() != month)
    {
        setMonth(label->date().year(), label->date().month());
    }

    for (int y = 1; y <= 6; ++y)
    {
        for (int x = 1; x <= 7; ++x)
        {
            auto label = static_cast<DateItem *>(grid->itemAtPosition(y, x)->widget());
            label->unselect();
        }
    }
    label->select();
}

void Calendar::setMonth(int year, int month)
{
    this->year = year;
    this->month = month;
    QDate monthFirstDay = QDate(year, month, 1);
    QDate firstDay = monthFirstDay.addDays(-(monthFirstDay.dayOfWeek() % 7));
    for (int y = 1; y <= 6; ++y)
    {
        for (int x = 1; x <= 7; ++x)
        {
            auto label = static_cast<DateItem *>(grid->itemAtPosition(y, x)->widget());
            Q_ASSERT(label != nullptr);
            QDate date = firstDay.addDays((y - 1) * 7 + x - 1);
            label->setDate(date);
            label->setEnabled(date.month() == month);
        }
    }

    for (int y = 1; y <= 6; ++y)
    {
        auto label = static_cast<QLabel *>(grid->itemAtPosition(y, 0)->widget());
        Q_ASSERT(label != nullptr);
        label->setText("<font color=red><strong>" + QString::number(monthFirstDay.weekNumber() + y - 1) + "</strong></font>");
    }
}

void Calendar::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), Qt::GlobalColor::cyan);
}


void Calendar::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
    {
        return;
    }
    isMousePressed = true;
    lastMousePos = e->globalPos();
    lastPos = pos();
}

void Calendar::mouseMoveEvent(QMouseEvent *e)
{
    if (isMousePressed)
    {
        this->move(lastPos + e->globalPos() - lastMousePos);
    }
    else
    {
        e->ignore();
    }
}

void Calendar::mouseReleaseEvent(QMouseEvent *e)
{
    isMousePressed = false;
}

void Calendar::dragEnterEvent(QDragEnterEvent *e)
{
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls() || mime->hasText())
    {
        e->accept();
    }
    else
    {
        e->ignore();
    }
}

void Calendar::dragMoveEvent(QDragMoveEvent *e)
{
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls() || mime->hasText())
    {
        e->accept();
    }
    else
    {
        e->ignore();
    }
    auto label = static_cast<DateItem *>(childAt(e->pos()));
    if (!label || !label->enabled())
    {
        return;
    }
    itemSelected(label);
}

void Calendar::dropEvent(QDropEvent *e)
{
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls() || mime->hasText())
    {
        if (e->source() == this)
        {
            e->setDropAction(Qt::MoveAction);
            e->accept();
        }
        else
        {
            e->acceptProposedAction();
        }
    }
    else
    {
        e->ignore();
        return;
    }
    auto label = static_cast<DateItem *>(childAt(e->pos()));
    if (!label)
    {
        return;
    }
    label->setColor(Qt::GlobalColor::cyan);
    const QList<QUrl> &urls = mime->urls();
    qDebug() << label->date() << "dropped";
    for (const auto &url : urls)
    {
        qDebug() << url.toString();
    }
    qDebug() << mime->text();
}
