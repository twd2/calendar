#ifndef DRAGGABLELABEL_H
#define DRAGGABLELABEL_H

#include "draggablelabel.h"

#include <QLabel>
#include <QWidget>
#include <QString>
#include <QMouseEvent>
#include <QLabel>
#include <QGridLayout>
#include <QVariant>

class DraggableLabel : public QLabel
{
    Q_OBJECT
private:
    QPalette _defaultPal;
public:
    QString filePath;
    QVariant userData;
    DraggableLabel(const QString &text, QWidget *parent = nullptr);
    ~DraggableLabel();
    void setSelected(bool);
protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
signals:
    void clicked(DraggableLabel *);
};

#endif // DRAGGABLELABEL_H
