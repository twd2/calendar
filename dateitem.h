#ifndef DATEITEM_H
#define DATEITEM_H

#include <QWidget>
#include <QDate>

class DateItem : public QWidget
{
    Q_OBJECT
public:
    explicit DateItem(QWidget *parent = 0);
    QColor color();
    QString html();
    QDate date();
    bool enabled();
    bool selected();
    void setBackgroundColor(QColor);
    QColor backgroundColor();
protected:
    void paintEvent(QPaintEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
signals:
    void clicked();
    void doubleClicked();
public slots:
    void setColor(QColor);
    void setHtml(QString);
    void setDate(QDate);
    void setEnabled(bool);
    void setSelected(bool);
private:
    QString _html = "";
    QColor _colorBackground = Qt::GlobalColor::white;
    QColor _colorWeekdayText = Qt::GlobalColor::black;
    QColor _colorWeekendText = Qt::GlobalColor::red;
    QColor _colorDisabledText = Qt::GlobalColor::gray;
    QDate _date;
    bool _enabled = true;
    bool _selected = false;
};

#endif // DATEITEM_H
