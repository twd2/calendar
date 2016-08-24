#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QGridLayout>
#include <QDate>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

class Calendar : public QWidget
{
    Q_OBJECT
protected:
    const int yearRange = 50; // -yearRange ~ +yearRange
    QVBoxLayout *mainLayout;
    QHBoxLayout *controllers;
    QGridLayout *grid;
    QVBoxLayout *settingLayout;
    QDate _month;
    QComboBox *yearBox, *monthBox;
    QDate selectedDate;
    QPushButton *movable, *acceptDnD;
public:
    explicit Calendar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

    void dragEnterEvent(QDragEnterEvent *) override;
    void dragMoveEvent(QDragMoveEvent *) override;
    void dropEvent(QDropEvent *) override;

signals:

public slots:
    void itemDoubleClicked(QWidget *);
    void itemSelected(QWidget *);
    void setMonth(const QDate &);
    void setMonth(int);
    void setYear(int);
    void changeMonth(int);
    void setSelected(const QDate &);
    void updateTodo();
    void setToday();
private:
    void initConrtollers();
    void initCalendar();
    QPoint lastPos, lastMousePos;
    bool isMousePressed = false;
};

#endif // CALENDAR_H
