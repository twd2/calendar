#include "todolist.h"
#include "todoitem.h"
#include "storage.h"

#include <QPushButton>
#include <QSpacerItem>
#include <QDebug>

TodoList::TodoList(const QDate &date, QWidget *parent) :
    QDialog(parent), date(date),
    mainLayout(new QVBoxLayout(this)), controllers(new QHBoxLayout(this)), okLayout(new QHBoxLayout(this))
{
    setWindowTitle(tr("Todo List on %1").arg(date.toString("yyyy-MM-dd")));
    QStringList header;
    header << tr("On") << tr("Things");
    table = new QTableWidget(0, 2, this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setColumnWidth(0, 250);
    table->setColumnWidth(1, 500);
    table->setHorizontalHeaderLabels(header);
    table->show();
    mainLayout->addWidget(table);
    mainLayout->addLayout(controllers);
    mainLayout->addLayout(okLayout);

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

    refreshTable();
}

TodoList::~TodoList()
{

}

void TodoList::refreshTable()
{
    auto list = Storage::i()->get(date);
    table->clearSelection();
    table->setRowCount(list.count());
    indexToID.clear();
    for (int i = 0; i < list.count(); ++i)
    {
        const auto &item = list[i];
        table->setItem(i, 0, new QTableWidgetItem(item.matchToString()));
        table->setItem(i, 1, new QTableWidgetItem(item.thing));
        indexToID[i] = item.id;
    }
}

void TodoList::add()
{
    // TODO
    Storage::i()->add(TodoItem());
    refreshTable();
}

void TodoList::del()
{
    // TODO
    QVector<int> indexToDelete;
    for (auto &range : table->selectedRanges())
    {
        for (int index = range.topRow(); index <= range.bottomRow(); ++index)
        {
            qDebug() << index << indexToID[index];
            if (!indexToID.contains(index))
            {
                qDebug() << "no index";
                continue;
            }
            indexToDelete.append(indexToID[index]);
        }
    }

    for (int i = indexToDelete.count() - 1; i >= 0; --i)
    {
        Storage::i()->del(indexToDelete[i]);
    }
    refreshTable();
}
