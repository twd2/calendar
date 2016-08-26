#include "calendar.h"
#include "dateitem.h"
#include "todoitem.h"
#include "todolist.h"
#include "global.h"
#include "storage.h"
#include "searchresult.h"

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
#include <QFileDialog>
#include <QResizeEvent>
#include <QBitmap>
#include <QGraphicsView>
#include <QtGlobal>
#include <QMessageBox>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif


Calendar::Calendar(QWidget *parent)
    : QWidget(parent), mainLayout(new QVBoxLayout(this)),
      controllers(new QHBoxLayout()), grid(new QGridLayout()), settingLayout(new QVBoxLayout())
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAcceptDrops(true);
    // setWindowOpacity(0.7);
    // setAttribute(Qt::WA_TransparentForMouseEvents, true);
    // setAttribute(Qt::WA_TranslucentBackground, true);
    // setWindowTransparent(true);

    // set background color
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor::fromRgb(0x5f, 0x9f, 0xd6));
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

    QPushButton *btnImport = new QPushButton(tr("Import"), this);
    connect(btnImport, SIGNAL(clicked(bool)), this, SLOT(importTodo()));
    btnImport->show();
    controllers->addWidget(btnImport);

    QPushButton *btnExport = new QPushButton(tr("Export"), this);
    connect(btnExport, SIGNAL(clicked(bool)), this, SLOT(exportTodo()));
    btnExport->show();
    controllers->addWidget(btnExport);

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
        monthBox->addItem(Global::monthAbbr(i), i + 1);
    }
    connect(monthBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMonth(int)));
    monthBox->show();
    controllers->addWidget(monthBox);

    QPushButton *btnToday = new QPushButton(tr("Today"), this);
    connect(btnToday, SIGNAL(clicked(bool)), this, SLOT(setToday()));
    btnToday->show();
    controllers->addWidget(btnToday);

    QLabel *labQuery = new QLabel(tr("Search:"));
    labQuery->show();
    controllers->addWidget(labQuery);

    txtQuery = new QLineEdit(this);
    connect(txtQuery, SIGNAL(returnPressed()), this, SLOT(search()));
    txtQuery->show();
    controllers->addWidget(txtQuery);

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
        QLabel *label = new QLabel("<font color=" + c + "><strong>" + Global::dayOfWeekAbbr(x - 1) + "</strong></font>", this);
        label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        label->show();
        grid->addWidget(label, 0, x);
    }

    /*QLabel *label = new QLabel(tr("Wandai's<br>Calendar"), this);
    label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    label->show();*/

    // TODO user setting for these:
    movable = new QPushButton(tr("Movable"), this);
    movable->setCheckable(true);
    movable->setChecked(true);
    movable->show();
    connect(movable, SIGNAL(toggled(bool)), this, SLOT(movableChanged()));
    settingLayout->addWidget(movable);
    acceptDnD = new QPushButton(tr("DnD"), this);
    acceptDnD->setCheckable(true);
    acceptDnD->setChecked(true);
    acceptDnD->show();
    settingLayout->addWidget(acceptDnD);
    grid->addLayout(settingLayout, 0, 0);
    // grid->addWidget(label, 0, 0);

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
    isMousePressed = false;
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
    delete list;
    updateTodo();
}

void Calendar::itemSelected(QWidget *w)
{

    auto label = static_cast<DateItem *>(w);
    if (!label)
    {
        setSelected(QDate());
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
        label->setText("<font color=red><strong>" + QString::number(_month.addDays((y - 1) * 7).weekNumber()) + "</strong></font>");
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

}

void Calendar::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
    {
        e->ignore();
        return;
    }
    if (!movable->isChecked())
    {
        e->ignore();
        return;
    }
    isMousePressed = true;
    lastMousePos = e->globalPos();
    lastPos = pos();
}

void Calendar::mouseMoveEvent(QMouseEvent *e)
{
    if (!movable->isChecked())
    {
        e->ignore();
        return;
    }
    if (isMousePressed)
    {
        move(lastPos + e->globalPos() - lastMousePos);
    }
}

void Calendar::mouseReleaseEvent(QMouseEvent *e)
{
    if (!movable->isChecked())
    {
        e->ignore();
        return;
    }
    qDebug() << "mouseReleaseEvent";
    isMousePressed = false;
}

void Calendar::dragEnterEvent(QDragEnterEvent *e)
{
    if (!acceptDnD->isChecked())
    {
        e->ignore();
        return;
    }
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls())
    {
        e->accept();
    }
    else
    {
        e->ignore();
        return;
    }

    // check file
    const QList<QUrl> urls = mime->urls();
    for (const auto &url : urls)
    {
        if (!url.isLocalFile())
        {
            e->ignore();
            return;
        }
        QFileInfo fi(url.toLocalFile());
        if (fi.isDir())
        {
            e->ignore();
            return;
        }
    }
}

void Calendar::dragMoveEvent(QDragMoveEvent *e)
{
    if (!acceptDnD->isChecked())
    {
        e->ignore();
        return;
    }
    const QMimeData *mime = e->mimeData();
    if (mime->hasUrls())
    {
        e->accept();
    }
    else
    {
        e->ignore();
        return;
    }
    // check file
    const QList<QUrl> urls = mime->urls();
    for (const auto &url : urls)
    {
        if (!url.isLocalFile())
        {
            e->ignore();
            return;
        }
        QFileInfo fi(url.toLocalFile());
        if (fi.isDir())
        {
            e->ignore();
            return;
        }
        qDebug() << url.toString();
    }
    auto label = dynamic_cast<DateItem *>(childAt(e->pos()));
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
    if (!acceptDnD->isChecked())
    {
        e->ignore();
        return;
    }
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
    // check file
    const QList<QUrl> urls = mime->urls();
    for (const auto &url : urls)
    {
        if (!url.isLocalFile())
        {
            e->ignore();
            return;
        }
        QFileInfo fi(url.toLocalFile());
        if (fi.isDir())
        {
            e->ignore();
            return;
        }
    }
    auto label = dynamic_cast<DateItem *>(childAt(e->pos()));
    if (!label || !label->enabled())
    {
        return;
    }
    qDebug() << label->date() << "dropped";
    for (const auto &url : urls)
    {
        qDebug() << url.toString();
        Storage::file()->putFile(url.toLocalFile(), label->date());
    }
    updateTodo();
}

void Calendar::updateTodo()
{
    for (int y = 1; y <= 6; ++y)
    {
        for (int x = 1; x <= 7; ++x)
        {
            auto label = static_cast<DateItem *>(grid->itemAtPosition(y, x)->widget());
            Q_ASSERT(label != nullptr);
            auto list = Storage::todo()->get(label->date());
            QStringList strlist;
            strlist << "<body style='margin:0px;padding:0px;'>";
            if (list.count() == 0)
            {
                label->setBackgroundColor(Qt::GlobalColor::white);
            }
            else
            {
                label->setBackgroundColor(Qt::GlobalColor::white);
                for (const TodoItem &i : list)
                {
                    int r, g, b;
                    i.color.getRgb(&r, &g, &b);
                    strlist << QString("<p style='background-color:rgb(%1, %2, %3);color:%4;margin:0px;padding:0px;'>%5</p>")
                                   .arg(r).arg(g).arg(b).arg(Global::getTextColorName(i.color))
                                   .arg(i.text.toHtmlEscaped());
                    if (i.fullMatch(label->date()))
                    {
                         label->setBackgroundColor(i.color);
                    }
                }
            }
            auto files = Storage::file()->getFileList(label->date());
            for (const FileInfo &fi : files)
            {
                strlist << QString("<p>📄<span style='margin:0px;padding:0px;font-style:italic;'>%1</span></p>").arg(fi.fileName.toHtmlEscaped());
            }
            strlist << "</body>";
            label->setText(strlist.join("\n"));
            label->update();
        }
    }
}

void Calendar::setToday()
{
    QDate now = QDate::currentDate();
    setSelected(now);
    setMonth(now);
}

void Calendar::importTodo()
{
    QFileDialog fd;
    QStringList filters;
    filters << tr("JSON file (*.json)")
            << tr("All files (*)");
    fd.setNameFilters(filters);
    fd.setWindowTitle(tr("Import"));
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    if (fd.exec())
    {
        qDebug() << fd.selectedFiles()[0];
        Storage::todo()->importFile(fd.selectedFiles()[0], false);
    }
    updateTodo();
}

void Calendar::exportTodo()
{
    QFileDialog fd;
    QStringList filters;
    filters << tr("JSON file (*.json)")
            << tr("All files (*)");
    fd.setNameFilters(filters);
    fd.setWindowTitle(tr("Export"));
    fd.setFileMode(QFileDialog::AnyFile);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    if (fd.exec())
    {
        qDebug() << fd.selectedFiles()[0];
        Storage::todo()->exportFile(fd.selectedFiles()[0]);
    }
}

void Calendar::setWindowMouseEventTransparent(bool trans)
{
#ifdef Q_OS_WIN
    if (trans)
    {
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) | WS_EX_TRANSPARENT);
    }
    else
    {
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
    }
#endif
}

void Calendar::movableChanged()
{
#ifdef Q_OS_WIN
    if (movable->isChecked())
    {
        setWindowMouseEventTransparent(false);
        setWindowOpacity(1.0);
    }
    else
    {
        setWindowMouseEventTransparent(true);
        setWindowOpacity(WINDOWOPACITY);

        // there is a little bit tricky
        QPushButton *btnRestore = new QPushButton(tr("Movable"));
        btnRestore->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        btnRestore->move(pos() + movable->pos());
        btnRestore->resize(movable->size());
        btnRestore->setAttribute(Qt::WA_DeleteOnClose);
        connect(btnRestore, SIGNAL(clicked(bool)), btnRestore, SLOT(close()));
        connect(btnRestore, SIGNAL(destroyed(QObject*)), this, SLOT(restoreClicked()));
        btnRestore->show();
    }
#endif
}

void Calendar::restoreClicked()
{
    movable->setChecked(true);
}

void Calendar::search()
{
    SearchResult result;
    result.setQuery(txtQuery->text());
    result.exec();
    updateTodo();
}
