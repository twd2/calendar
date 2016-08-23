#ifndef CALENDAR_H
#define CALENDAR_H

#include <QCalendarWidget>
#include <QPainter>
#include <QColor>
#include <QDate>
#include <QPen>
#include <QBrush>
#include <QVector>

class Calendar : public QCalendarWidget
{
    Q_OBJECT
public:
    Calendar(QWidget *parent = nullptr);
    ~Calendar();

    void setColor(QColor &color);
    QColor getColor();
protected:
    void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const override;

private slots:
    void dateSelected(const QDate &);

private:
    QPoint lastPos, lastMousePos;
    bool isMousePressed = false;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void dragEnterEvent(QDragEnterEvent *) override;
    void dragMoveEvent(QDragMoveEvent *) override;
    void dropEvent(QDropEvent *) override;

};

#endif // CALENDAR_H
