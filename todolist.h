#ifndef TODOLIST_H
#define TODOLIST_H

#include "fileinfo.h"

#include <QDialog>
#include <QDate>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QListWidget>
#include <QMap>
#include <QMouseEvent>
#include <QVector>

class TodoList : public QDialog
{
    Q_OBJECT
protected:
    QTableWidget *table;
    QListWidget *fileList;
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
private:
    QMap<int, int> indexToID;
    QVector<FileInfo> files;
    void initControllers();
    void refreshItems();
};

#endif // TODOLIST_H
