#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include "todostorage.h"
#include "filestorage.h"

#include <QSqlDatabase>
#include <QSqlResult>

class SQLiteStorage
    : public TodoStorage, public FileStorage
{
public:
    static const QString tempDirName;
    static const QString databaseFilename;
    static const QString sqlInit1;
    static const QString sqlInit2;
    static const QString sqlInit3;
    static const QString sqlInit4;
    static const QString sqlAdd;
    static const QString sqlGetByID;
    static const QString sqlGetByRowID;
    static const QString sqlGetAll;
    static const QString sqlGetLike;
    static const QString sqlGetLastInsertRowID;
    static const QString sqlGetByDate;
    static const QString sqlGetByDateFullMatch;
    static const QString sqlUpdate;
    static const QString sqlDelete;
    static const QString sqlDeleteAll;
    static const QString sqlAddFile;
    static const QString sqlGetAllFileInfo;
    static const QString sqlGetFileInfoByRowID;
    static const QString sqlGetFileInfoByID;
    static const QString sqlGetFileInfoByDate;
    static const QString sqlGetFileInfoLike;
    static const QString sqlGetFileByID;
    static const QString sqlGetFileCountByDate;
    static const QString sqlDelFile;
    QSqlDatabase db;
    SQLiteStorage();
    ~SQLiteStorage() override;

    // TodoStorage implememtation
    int add(const TodoItem &item) override;
    TodoItem get(int id) override;
    QVector<TodoItem> get(const QDate &date) override;
    QVector<TodoItem> get(const QString &query) override;
    QVector<TodoItem> getAll() override;
    void set(int id, const TodoItem &item) override;
    void del(int id) override;
    void delAll() override;

    void load() override;
    void save() override;

    // FileStorage implementation
    FileInfo putFile(const QString &source, const QDate &date) override;
    QVector<FileInfo> getAllFileList();
    QVector<FileInfo> getFileList(const QDate &date) override;
    QVector<FileInfo> getFileList(const QString &query) override;
    int getFileCount(const QDate &date) override;
    FileInfo getFileInfo(const QString &id);
    QString getFilePath(const QString &id) override;
    QByteArray getFileData(const QString &id) override;
    void delFile(const QString &id) override;

    void clearTempFiles();
private:
    void bindTodoItem(QSqlQuery &, const TodoItem &);
    TodoItem toTodoItem(QSqlQuery &);
    QVector<TodoItem> toTodoItems(QSqlQuery &);
    FileInfo toFileInfo(QSqlQuery &);
    QVector<FileInfo> toFileInfos(QSqlQuery &);
};

#endif // SQLITESTORAGE_H
