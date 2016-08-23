#include "calendar.h"
#include "dateitem.h"
#include "todoitem.h"
#include "todolist.h"
#include "global.h"
#include "storage.h"

#include <QVector>
#include <QLabel>
#include <QSignalMapper>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QMimeData>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>


Calendar::Calendar(QWidget *parent)
    : QWidget(parent), mainLayout(new QVBoxLayout(this)),
      controllers(new QHBoxLayout()), grid(new QGridLayout())
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAcceptDrops(true);
    setWindowOpacity(1.0);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // set background color
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::GlobalColor::lightGray);
    setAutoFillBackground(true);
    setPalette(pal);

    controllers->setSpacing(1);
    controllers->setMargin(0);
    grid->setSpacing(1);
    grid->setMargin(0);
    mainLayout->addLayout(controllers);
    mainLayout->addLayout(grid);
    mainLayout->setSpacing(1);
    mainLayout->setMargin(0);
    setLayout(mainLayout);
    initConrtollers();
    initCalendar();
}

void Calendar::initConrtollers()
{
    QSignalMapper *mapper = new QSignalMapper(this);
    QPushButton *btnPrev = new QPushButton(tr("<< Previous month"), this);
    mapper->setMapping(btnPrev, -1);
    connect(btnPrev, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    btnPrev->show();
    QPushButton *btnNext = new QPushButton(tr("Next month >>"), this);
    mapper->setMapping(btnNext, +1);
    connect(btnNext, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    btnNext->show();
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(changeMonth(int)));

    controllers->addWidget(btnPrev);

    yearBox = new QComboBox(this);
    for (int i = 0; i < yearRange * 2 + 1; ++i)
    {
        yearBox->addItem("", 0);
    }
    connect(yearBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setYear(int)));
    yearBox->show();
    controllers->addWidget(yearBox);

    monthBox = new QComboBox(this);
    for (int i = 0; i < 12; ++i)
    {
        monthBox->addItem(Global::monthAbbr[i], i + 1);
    }
    connect(monthBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMonth(int)));
    monthBox->show();
    controllers->addWidget(monthBox);

    controllers->addWidget(btnNext);
}

void Calendar::initCalendar()
{
    // week
    for (int y = 1; y <= 6; ++y)
    {
        QLabel *label = new QLabel("??", this);
        label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        label->setAttribute(Qt::WA_TranslucentBackground, true);
        label->show();
        grid->addWidget(label, y, 0);
    }

    // day of week
    for (int x = 1; x <= 7; ++x)
    {
        QString c = (x == 1 || x == 7) ? "red" : "black";
        QLabel *label = new QLabel("<font color=" + c + "><strong>" + Global::dayOfWeekAbbr[x - 1] + "</strong></font>", this);
        label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        label->show();
        grid->addWidget(label, 0, x);
    }

    QLabel *label = new QLabel(tr("Wandai's<br>Calendar"), this);
    label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
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
    connect(mapperClicked, SIGNAL(mapped(QWidget *)), this, SLOT(itemDoubleClicked(QWidget *)));
    connect(mapperSelected, SIGNAL(mapped(QWidget *)), this, SLOT(itemSelected(QWidget *)));

    QDate now = QDate::currentDate();
    setMonth(now);
    setSelected(now);
}

void Calendar::itemDoubleClicked(QWidget *w)
{
    auto label = static_cast<DateItem *>(w);
    if (!label)
    {
        return;
    }
    qDebug() << label->date() << "double clicked";
    TodoList *list = new TodoList(label->date(), this);
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry();
    list->resize(screen.width() / 2, screen.height() / 2);
    list->exec();
    updateTodo();
}

void Calendar::itemSelected(QWidget *w)
{

    auto label = static_cast<DateItem *>(w);
    if (!label)
    {
        return;
    }
    qDebug() << label->date() << "selected";
    setSelected(label->date());
    if (label->date().month() != _month.month())
    {
        setMonth(label->date());
    }
}

void Calendar::changeMonth(int delta)
{
    setMonth(_month.addMonths(delta));
}

void Calendar::setSelected(const QDate &date)
{
    for (int y = 1; y <= 6; ++y)
    {
        for (int x = 1; x <= 7; ++x)
        {
            auto i = static_cast<DateItem *>(grid->itemAtPosition(y, x)->widget());
            i->setSelected(i->date() == date);
        }
    }
    selectedDate = date;
}

void Calendar::setMonth(const QDate &month)
{
    if (_month.year() == month.year()
        && _month.month() == month.month())
    {
        return;
    }

    _month = QDate(month.year(), month.month(), 1);
    monthBox->setCurrentIndex(_month.month() - 1);

    // update year box
    for (int i = 0; i < yearRange * 2 + 1; ++i)
    {
        int year = _month.year() + i - yearRange;
        yearBox->setItemText(i, QString::number(year));
        yearBox->setItemData(i, year);
    }
    yearBox->setCurrentIndex(yearRange);

    // update dateitems
    QDate firstDay = _month.addDays(-(_month.dayOfWeek() % 7));
    for (int y = 1; y <= 6; ++y)
    {
        for (int x = 1; x <= 7; ++x)
        {
            auto label = static_cast<DateItem *>(grid->itemAtPosition(y, x)->widget());
            Q_ASSERT(label != nullptr);
            QDate date = firstDay.addDays((y - 1) * 7 + x - 1);
            label->setDate(date);
            label->setEnabled(date.month() == _month.month());
        }
    }

    // update week numbers
    for (int y = 1; y <= 6; ++y)
    {
        auto label = static_cast<QLabel *>(grid->itemAtPosition(y, 0)->widget());
        Q_ASSERT(label != nullptr);
        label->setText("<font color=red><strong>" + QString::number(_month.weekNumber() + y - 1) + "</strong></font>");
    }


    updateTodo();
    // update selected
    setSelected(selectedDate);
}

void Calendar::setMonth(int index)
{
    if (index < 0)
    {
        return;
    }
    setMonth(QDate(_month.year(), monthBox->itemData(index).toInt(), 1));
}

void Calendar::setYear(int index)
{
    if (index < 0)
    {
        return;
    }
    setMonth(QDate(yearBox->itemData(index).toInt(), _month.month(), 1));
}

void Calendar::paintEvent(QPaintEvent *)
{

}

void Calendar::resizeEvent(QResizeEvent *)
{
//    QPainterPath path;
//    //QRectF rect = QRectF(0,0,200,100);
//    path.addRoundRect(rect(), 5, 5);
//    QPolygon polygon = path.toFillPolygon().toPolygon();
//    //获得这个路径上的所有的点
//    QRegion region(polygon);
//    //根据这个点构造这个区域
//    setMask(region);
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
    if (mime->hasUrls())
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
    if (mime->hasUrls())
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
        e->setDropAction(Qt::IgnoreAction);
        itemSelected(nullptr);
    }
    else
    {
        e->setDropAction(Qt::CopyAction);
        itemSelected(label);
    }
}

void Calendar::dropEvent(QDropEvent *e)
{
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls())
    {
        if (e->source() == this)
        {
            e->setDropAction(Qt::CopyAction);
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
    if (!label || !label->enabled())
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
}

void Calendar::updateTodo()
{
    for (int y = 1; y <= 6; ++y)
    {
        for (int x = 1; x <= 7; ++x)
        {
            auto label = static_cast<DateItem *>(grid->itemAtPosition(y, x)->widget());
            Q_ASSERT(label != nullptr);
            auto list = Storage::i()->get(label->date());
            QStringList strlist;
            if (list.count() == 0)
            {
                label->colorBackground = Qt::GlobalColor::white;
            }
            else
            {
                label->colorBackground = Qt::GlobalColor::yellow;
                for (const TodoItem &i : list)
                {
                    strlist << i.text;
                }
            }
            label->setText(strlist.join("\n"));
            label->update();
        }
    }
}
