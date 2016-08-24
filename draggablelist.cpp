#include "draggablelist.h"

#include <QDebug>

DraggableList::DraggableList(QWidget *parent)
    : QWidget(parent), mainLayout(new QVBoxLayout(this))
{
    mainLayout->setMargin(0);
    mainLayout->setSpacing(2);
    setLayout(mainLayout);
}

void DraggableList::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "DraggableList" << e->pos();
    select(nullptr);
}

void DraggableList::clear()
{
    while (mainLayout->count() > 0)
    {
        auto *ptr = mainLayout->takeAt(0);
        if (ptr)
        {
            ptr->widget()->close();
            delete ptr->widget();
            delete ptr;
        }
    }
    update();
}

void DraggableList::append(DraggableLabel *label)
{
    mainLayout->addWidget(label);
    connect(label, SIGNAL(clicked(DraggableLabel*)), this, SLOT(select(DraggableLabel*)));
}

void DraggableList::select(DraggableLabel *label)
{
    for (int i = 0; i < mainLayout->count(); ++i)
    {
        DraggableLabel *l = static_cast<DraggableLabel *>(mainLayout->itemAt(i)->widget());
        l->setSelected(l == label);
    }
    selectedLabel = label;
}
