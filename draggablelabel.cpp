#include "draggablelabel.h"

#include <cstdlib>

#include <QMimeData>
#include <QDrag>
#include <QUrl>
#include <QDebug>

DraggableLabel::DraggableLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    _defaultPal = palette();
    setMargin(5);
}

DraggableLabel::~DraggableLabel()
{
    qDebug() << "~DraggableLabel";
}

void DraggableLabel::setSelected(bool selected)
{
    setAutoFillBackground(true);
    if (selected)
    {
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::GlobalColor::darkBlue);
        pal.setColor(QPalette::Foreground, Qt::GlobalColor::white);
        pal.setColor(QPalette::Text, Qt::GlobalColor::white);
        setPalette(pal);
    }
    else
    {
        setPalette(_defaultPal);
    }

}

void DraggableLabel::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "DraggableLabel" << e->pos() << pos();
    emit clicked(this);
    // QPoint hotSpot = e->pos(); // - pos();

    QList<QUrl> urls;
    urls.append(QUrl::fromLocalFile(filePath()));

    QMimeData *mimeData = new QMimeData();
    mimeData->setUrls(urls);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    // drag->setObjectName("file");
    // drag->setHotSpot(hotSpot);

    qDebug() << drag->exec(Qt::CopyAction, Qt::CopyAction);
    delete drag;
}

void DraggableLabel::mouseDoubleClickEvent(QMouseEvent *e)
{
    // qDebug() << "double clicked" << e->pos();
    // system(QString("open %1").arg(filePath).toStdString().c_str());
}
