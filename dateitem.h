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
    QString text();
    QDate date();
    bool enabled();
    bool selected();
protected:
    void paintEvent(QPaintEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
signals:
    void clicked();
    void doubleClicked();
public slots:
    void setColor(QColor);
    void setText(QString);
    void setDate(QDate);
    void setEnabled(bool);
    void setSelected(bool);
private:
    QString _text = "";
    QColor _colorSelected = Qt::GlobalColor::gray;
    QColor _colorWeekday = Qt::GlobalColor::black;
    QColor _colorWeekend = Qt::GlobalColor::red;
    QColor _colorDisabled = Qt::GlobalColor::gray;
    QDate _date;
    bool _enabled = true;
    bool _selected = false;
};

#endif // DATEITEM_H
