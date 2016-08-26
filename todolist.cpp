#include "todolist.h"
#include "todoitem.h"
#include "todoedit.h"
#include "storage.h"
#include "global.h"

#include <QPushButton>
#include <QSpacerItem>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>

TodoList::TodoList(const QDate &date, QWidget *parent) :
    QDialog(parent),
    mainLayout(new QVBoxLayout(this)), controllers(new QHBoxLayout()), okLayout(new QHBoxLayout()),
    date(date)
{
    setWindowTitle(tr("Todo List on %1").arg(date.toString("yyyy-MM-dd")));

    table = new TodoTable(this);
    mainLayout->addWidget(new QLabel(tr("Todo List"), this));
    mainLayout->addWidget(table);
    mainLayout->addLayout(controllers);
    initFileList();
    mainLayout->addLayout(okLayout);

    initControllers();
    refreshItems();
    //setLayout(mainLayout);
}

void TodoList::initFileList()
{
    mainLayout->addWidget(new QLabel(tr("Today's files"), this));

    fileListScroll = new QScrollArea(this);
    fileListScroll->setWidgetResizable(true);
    fileList = new DraggableList(this);
    fileList->show();
    fileListScroll->setWidget(fileList);
    fileListScroll->show();
    mainLayout->addWidget(fileListScroll);

    QPushButton *btnDelete = new QPushButton(tr("Delete File"), this);
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(delFile()));
    btnDelete->show();
    mainLayout->addWidget(btnDelete);
}

void TodoList::initControllers()
{
    QPushButton *btnAdd = new QPushButton(tr("Add"), this);
    connect(btnAdd, SIGNAL(clicked(bool)), this, SLOT(add()));
    btnAdd->show();
    controllers->addWidget(btnAdd);

    QPushButton *btnDelete = new QPushButton(tr("Delete"), this);
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(del()));
    btnDelete->show();
    controllers->addWidget(btnDelete);

    QPushButton *btnOK = new QPushButton(tr("OK"), this);
    btnOK->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(btnOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    btnOK->setDefault(true);
    btnOK->show();
    okLayout->addWidget(btnOK);
    okLayout->setAlignment(btnOK, Qt::AlignRight);

    setTabOrder(btnAdd, btnDelete);
    setTabOrder(btnDelete, btnOK);
    setTabOrder(btnOK, btnAdd);

    connect(table, SIGNAL(itemDoubleClicked(int)), this, SLOT(edit(int)));
    table->show();
}

TodoList::~TodoList()
{

}

void TodoList::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "TodoList" << e->pos();
    // e->ignore();
    // QWidget::mousePressEvent(e);
}

void TodoList::refreshItems()
{
    table->setTodoItems(Storage::todo()->get(date));

    // list
    files = Storage::file()->getFileList(date);
    fileList->clear();
    for (FileInfo fi : files)
    {
        auto *dl = new DraggableLabel(fi.fileName, this);
        dl->filePath = [fi] () { return Storage::file()->getFilePath(fi.id); };
        dl->userData = fi.id;
        fileList->append(dl);
    }
}

void TodoList::add()
{
    TodoEdit edit;
    edit.setWindowTitle(tr("Add"));
    edit.setItem(TodoItem(date));
    if (edit.exec())
    {
        if (!edit.item().match(date))
        {
            QMessageBox(QMessageBox::Information, tr("Note"), tr("The rule doesn't match this date, and it will show on another date.")).exec();
        }
        Storage::todo()->add(edit.item());
        refreshItems();
    }
}

void TodoList::del()
{
    QVector<int> idToDelete = table->idsSelected();

    if (idToDelete.count() == 0)
    {
        return;
    }

    if (QMessageBox(QMessageBox::Question, tr("Confirm"), tr("Are you sure to delete the todo(s) you selected?"),
                    QMessageBox::Ok | QMessageBox::Cancel).exec() != QMessageBox::Ok)
    {
        qDebug() << "cancelled";
        return;
    }

    for (int i = idToDelete.count() - 1; i >= 0; --i)
    {
        TodoItem item = Storage::todo()->get(idToDelete[i]);
        if (item.isMulti())
        {
            if (QMessageBox(QMessageBox::Question, tr("Confirm"), tr("Todo \"%1\" on %2 matches more than one day. \n"
                                                                     "Would you like to delete the whole rule or remove this day from the rule? \n"
                                                                     "Press \"Yes\" to delete the whole rule.")
                                                                    .arg(item.text, item.ruleToString()),
                            QMessageBox::Yes | QMessageBox::No).exec() == QMessageBox::Yes)
            {
                // delete the whole rule
                Storage::todo()->del(idToDelete[i]);
            }
            else
            {
                item.addExcept(date);
                Storage::todo()->set(idToDelete[i], item);
            }
        }
        else
        {
            Storage::todo()->del(idToDelete[i]);
        }
    }
    refreshItems();
}

void TodoList::edit(int id)
{
    TodoEdit edit;
    edit.setWindowTitle(tr("Edit"));
    edit.setItem(Storage::todo()->get(id));
    if (edit.exec())
    {
        if (!edit.item().match(date))
        {
            QMessageBox(QMessageBox::Information, tr("Note"), tr("The rule no longer matches this date, and will disappear from this window.")).exec();
        }
        Storage::todo()->set(id, edit.item());
        refreshItems();
    }
}

void TodoList::delFile()
{
    if (!fileList->selectedLabel)
    {
        return;
    }
    if (QMessageBox(QMessageBox::Question, tr("Confirm"), tr("Are you sure?"),
                    QMessageBox::Ok | QMessageBox::Cancel).exec() != QMessageBox::Ok)
    {
        qDebug() << "cancelled";
        return;
    }
    Storage::file()->delFile(fileList->selectedLabel->userData.toString());
    refreshItems();
}
