#include "todolist.h"
#include "ui_todolist.h"

TodoList::TodoList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TodoList)
{
    ui->setupUi(this);
}

TodoList::~TodoList()
{
    delete ui;
}
