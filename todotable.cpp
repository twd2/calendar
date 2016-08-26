#include "todotable.h"
#include "global.h"

#include <QDebug>

TodoTable::TodoTable(QWidget *parent) : QTableWidget(0, 2, parent)
{
    qDebug() << width() << height();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList header;
    header << tr("On") << tr("Todo");
    setHorizontalHeaderLabels(header);
    connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableRowDoubleClicked(int)));
}

void TodoTable::resizeEvent(QResizeEvent *e)
{
    setColumnWidth(0, width() / 3);
    setColumnWidth(1, width() * 2 / 3);
}

QVector<int> TodoTable::idsSelected()
{
    QVector<int> ids;
    for (auto &range : selectedRanges())
    {
        for (int index = range.topRow(); index <= range.bottomRow(); ++index)
        {
            qDebug() << index << indexToID[index];
            if (!indexToID.contains(index))
            {
                qDebug() << "no index";
                continue;
            }
            ids.append(indexToID[index]);
        }
    }
    return ids;
}

void TodoTable::setTodoItems(const QVector<TodoItem> &items)
{
    clearSelection();
    setRowCount(items.count());
    indexToID.clear();
    for (int i = 0; i < items.count(); ++i)
    {
        const auto &item = items[i];
        QTableWidgetItem *onItem = new QTableWidgetItem(item.ruleToString()),
                         *textItem = new QTableWidgetItem(item.text);
        onItem->setBackgroundColor(item.color);
        onItem->setTextColor(Global::getTextColor(item.color));
        textItem->setBackgroundColor(item.color);
        textItem->setTextColor(Global::getTextColor(item.color));
        setItem(i, 0, onItem);
        setItem(i, 1, textItem);
        indexToID[i] = item.id;
    }
}

void TodoTable::tableRowDoubleClicked(int row)
{
    emit itemDoubleClicked(indexToID[row]);
}
