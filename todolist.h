#ifndef TODOLIST_H
#define TODOLIST_H

#include <QDialog>

namespace Ui {
class TodoList;
}

class TodoList : public QDialog
{
    Q_OBJECT

public:
    explicit TodoList(QWidget *parent = 0);
    ~TodoList();

private:
    Ui::TodoList *ui;
};

#endif // TODOLIST_H
