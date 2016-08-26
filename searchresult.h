#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include "todotable.h"

#include <QDialog>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class SearchResult : public QDialog
{
    Q_OBJECT
protected:
    QString query;
    TodoTable *table;
    QVBoxLayout *mainLayout;
public:
    SearchResult(QWidget *parent = nullptr);
public slots:
    void setQuery(const QString &);
    void updateResult();
    void del();
    void edit(int);
};

#endif // SEARCHRESULT_H
