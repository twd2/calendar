#ifndef TODOTABLE_H
#define TODOTABLE_H

#include "todoitem.h"

#include <QWidget>
#include <QTableWidget>
#include <QResizeEvent>

class TodoTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit TodoTable(QWidget *parent = nullptr);
    QVector<int> idsSelected();
protected:
    void resizeEvent(QResizeEvent *event);
signals:
    void itemDoubleClicked(int);
public slots:
    void setTodoItems(const QVector<TodoItem> &);
private slots:
    void tableRowDoubleClicked(int);
private:
    QMap<int, int> indexToID;
};

#endif // TODOTABLE_H
