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
    QDialog(parent), date(date),
    mainLayout(new QVBoxLayout(this)), controllers(new QHBoxLayout()), okLayout(new QHBoxLayout())
{
    setWindowTitle(tr("Todo List on %1").arg(date.toString("yyyy-MM-dd")));

    table = new QTableWidget(0, 2, this);
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

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setColumnWidth(0, 250);
    table->setColumnWidth(1, 500);
    QStringList header;
    header << tr("On") << tr("Things");
    table->setHorizontalHeaderLabels(header);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(edit(int)));
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
    // table
    auto list = Storage::i()->get(date);
    table->clearSelection();
    table->setRowCount(list.count());
    indexToID.clear();
    for (int i = 0; i < list.count(); ++i)
    {
        const auto &item = list[i];
        QTableWidgetItem *onItem = new QTableWidgetItem(item.matchToString()),
                         *textItem = new QTableWidgetItem(item.text);
        onItem->setBackgroundColor(item.color);
        onItem->setTextColor(Global::getTextColor(item.color));
        textItem->setBackgroundColor(item.color);
        textItem->setTextColor(Global::getTextColor(item.color));
        table->setItem(i, 0, onItem);
        table->setItem(i, 1, textItem);

        indexToID[i] = item.id;
    }

    // list
    files = Storage::i()->getFileList(date);
    fileList->clear();
    for (FileInfo fi : files)
    {
        auto *dl = new DraggableLabel(fi.fileName, this);
        dl->filePath = Storage::i()->getFilePath(fi.id);
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
        Storage::i()->add(edit.item());
        refreshItems();
    }
}

void TodoList::del()
{
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
    refreshItems();
}

void TodoList::edit(int index)
{
    TodoEdit edit;
    edit.setWindowTitle(tr("Edit"));
    edit.setItem(Storage::i()->get(indexToID[index]));
    if (edit.exec())
    {
        if (!edit.item().match(date))
        {
            QMessageBox(QMessageBox::Information, tr("Note"), tr("The rule no longer matches this date, and will disappear from this window.")).exec();
        }
        Storage::i()->set(indexToID[index], edit.item());
        refreshItems();
    }
}

void TodoList::delFile()
{
    Storage::i()->delFile(fileList->selectedLabel->userData.toString());
    refreshItems();
}
