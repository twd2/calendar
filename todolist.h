#ifndef TODOLIST_H
#define TODOLIST_H

#include "fileinfo.h"
#include "draggablelist.h"
#include "draggablelabel.h"

#include <QDialog>
#include <QDate>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QListWidget>
#include <QMap>
#include <QMouseEvent>
#include <QVector>
#include <QScrollArea>

class TodoList : public QDialog
{
    Q_OBJECT
protected:
    QTableWidget *table;
    DraggableList *fileList;
    QScrollArea *fileListScroll;
    QVBoxLayout *mainLayout;
    QHBoxLayout *controllers;
    QHBoxLayout *okLayout;
public:
    const QDate date;
    explicit TodoList(const QDate &date, QWidget *parent = nullptr);
    ~TodoList();
protected:
    void mousePressEvent(QMouseEvent *) override;
public slots:
    void add();
    void del();
    void edit(int);
    void delFile();
private:
    QMap<int, int> indexToID;
    QVector<FileInfo> files;
    void initControllers();
    void initFileList();
    void refreshItems();
};

#endif // TODOLIST_H
