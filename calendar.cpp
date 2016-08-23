#include "calendar.h"

#include <QtWidgets>
#include <QInputDialog>
#include <QTextCharFormat>
#include <QDebug>
#include <QtGlobal>
#include <QCalendarModel>

Calendar::Calendar(QWidget *parent)
    : QCalendarWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAcceptDrops(true);
    setWindowOpacity(0.7);
    connect(this, SIGNAL(activated(QDate)), this, SLOT(dateSelected(QDate)));
    // m_outlinePen.setColor(Qt::red);
    // m_transparentBrush.setColor(Qt::transparent);
}

Calendar::~Calendar()
{

}

void Calendar::dateSelected(const QDate &date)
{
    qDebug() << date.toString() << endl;
    /*QString memo = QInputDialog::getText( this,
            "Memo", "Description:" );
    QMessageBox(QMessageBox::Information, "calendar", memo).exec();
    if( !memo.isEmpty() ){
        QBrush brush;
        brush.setColor(Qt::yellow);

        QTextCharFormat cf = this->dateTextFormat(date);
        cf.setBackground(brush);
        this->setDateTextFormat(date, cf);
        if(dates.contains(date))
        {
            int index = dates.indexOf(date);
            todolist.replace(index, memo);
        }
        else
        {
            dates.append(date);
            todolist.append(memo);
        }
    }*/

}

void Calendar::setColor(QColor& color)
{
   // m_outlinePen.setColor(color);
}

QColor Calendar::getColor()
{
    return Qt::GlobalColor::black;
   // return (m_outlinePen.color());
}

void Calendar::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
   //qDebug()<<"come in paintCell"<<endl;
   QCalendarWidget::paintCell(painter, rect, date);

   /*Q_ASSERT(dates.size()==todolist.size());
   for (int i = 0; i < dates.size(); i++)
   {
       if (date == dates.at(i))
       {
           painter->setPen(m_outlinePen);
           painter->setBrush(m_transparentBrush);
           painter->drawRect(rect.adjusted(0, 0, -1, -1));
           painter->drawText(rect,todolist.at(i));
       }
   }*/
}

void Calendar::mousePressEvent(QMouseEvent *e)
{
    auto ch = childAt(e->pos());
    qDebug() << ch->metaObject()->className();
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
    const QList<QUrl> &urls = mime->urls();
    for (const auto &url : urls)
    {
        qDebug() << url.toString();
    }
    qDebug() << mime->text();
}

QDate dateAt(const QPoint &p)
{
    QCalendarModel *calendarModel = ::qobject_cast<QCalendarModel *>(model());
        if (!calendarModel)
            return QDate();

        QPoint pos = event->pos();
        QModelIndex index = indexAt(pos);
        QDate date = calendarModel->dateForCell(index.row(), index.column());
        if (date.isValid() && date >= calendarModel->minimumDate
                && date <= calendarModel->maximumDate) {
            return date;
        }
        return QDate();
}
