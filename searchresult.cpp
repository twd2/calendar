#include "searchresult.h"
#include "storage.h"
#include "todoedit.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

SearchResult::SearchResult(QWidget *parent)
    : QDialog(parent), mainLayout(new QVBoxLayout(this))
{
    table = new TodoTable(this);
    connect(table, SIGNAL(itemDoubleClicked(int)), this, SLOT(edit(int)));
    table->show();
    mainLayout->addWidget(table);

    QPushButton *btnDelete = new QPushButton(tr("Delete"), this);
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(del()));
    btnDelete->show();
    mainLayout->addWidget(btnDelete);

    QPushButton *btnOK = new QPushButton(tr("OK"), this);
    connect(btnOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    btnOK->setDefault(true);
    btnOK->show();
    mainLayout->addWidget(btnOK);

    setLayout(mainLayout);
}

void SearchResult::setQuery(const QString &query)
{
    this->query = query;
    table->setTodoItems(Storage::todo()->get(query));
}

void SearchResult::updateResult()
{
    table->setTodoItems(Storage::todo()->get(query));
}

void SearchResult::del()
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
                                                                     "Press \"Yes\" to delete the whole rule.")
                                                                    .arg(item.text, item.ruleToString()),
                            QMessageBox::Yes | QMessageBox::No).exec() == QMessageBox::Yes)
            {
                // delete the whole rule
                Storage::todo()->del(idToDelete[i]);
            }
        }
        else
        {
            Storage::todo()->del(idToDelete[i]);
        }
    }
    updateResult();
}

void SearchResult::edit(int id)
{
    TodoEdit edit;
    edit.setWindowTitle(tr("Edit"));
    edit.setItem(Storage::todo()->get(id));
    if (edit.exec())
    {
        Storage::todo()->set(id, edit.item());
        updateResult();
    }
}
