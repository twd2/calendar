#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QGridLayout>


class Calendar : public QWidget
{
    Q_OBJECT
protected:
    QGridLayout *grid;
    int year, month;
public:
    explicit Calendar(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void itemClicked(QWidget *);
    void itemSelected(QWidget *);
    void setMonth(int year, int month);
private:
    void initCalendar();
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
