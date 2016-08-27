#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QGridLayout>
#include <QDate>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

class Calendar : public QWidget
{
    Q_OBJECT
protected:
    const qreal WINDOWOPACITY = 0.7;
    const int yearRange = 50; // -yearRange ~ +yearRange
    QVBoxLayout *mainLayout;
    QHBoxLayout *controllers;
    QGridLayout *grid;
    QVBoxLayout *settingLayout;
    QDate _month;
    QComboBox *yearBox, *monthBox;
    QDate selectedDate;
    QPushButton *movable, *acceptDnD;
    QLineEdit *txtQuery;
public:
    explicit Calendar(bool trans = false, QWidget *parent = nullptr);
    void setWindowMouseEventTransparent(bool);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

    void dragEnterEvent(QDragEnterEvent *) override;
    void dragMoveEvent(QDragMoveEvent *) override;
    void dropEvent(QDropEvent *) override;

    // void resizeEvent(QResizeEvent *) override;
signals:

public slots:
    void itemDoubleClicked(QWidget *);
    void itemClicked(QWidget *);
    void setMonth(const QDate &);
    void setMonth(int);
    void setYear(int);
    void changeMonth(int);
    void setSelected(const QDate &);
    void updateTodo();
    void setToday();
    void importTodo();
    void exportTodo();
    void movableChanged();
    void restoreClicked();
    void search();
private:
    void initConrtollers();
    void initCalendar();
    QPoint lastPos, lastMousePos;
    bool isMousePressed = false;
};

#endif // CALENDAR_H
