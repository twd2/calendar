#ifndef DRAGGABLELIST_H
#define DRAGGABLELIST_H

#include "draggablelabel.h"

#include <QWidget>
#include <QVBoxLayout>

class DraggableList : public QWidget
{
    Q_OBJECT
public:
    DraggableLabel *selectedLabel = nullptr;
private:
    QVBoxLayout *mainLayout;
public:
    explicit DraggableList(QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *);
signals:

public slots:
    void clear();
    void append(DraggableLabel *);
    void select(DraggableLabel *);
};

#endif // DRAGGABLELIST_H
