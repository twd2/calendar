#include "sqlitestorage.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

const QString SQLiteStorage::tempDirName = "temp";
const QString SQLiteStorage::databaseFilename = "cal.db3";
const QString SQLiteStorage::sqlInit1 =
    "CREATE TABLE IF NOT EXISTS `File` ( `ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `Date` INTEGER NOT NULL, `Name` TEXT, `Data` BLOB);";
const QString SQLiteStorage::sqlInit2 =
    "CREATE TABLE IF NOT EXISTS `TodoList` ( `ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `Year` INTEGER NOT NULL, `Month` INTEGER NOT NULL, `Day` INTEGER NOT NULL, `DayOfWeek` INTEGER NOT NULL, `ColorR` INTEGER NOT NULL, `ColorG` INTEGER NOT NULL, `ColorB` INTEGER NOT NULL, `Text` TEXT, `Except` TEXT );";
const QString SQLiteStorage::sqlInit3 =
    "CREATE INDEX IF NOT EXISTS `date` ON `TodoList` (`Year` ASC,`Month` ASC,`Day` ASC,`DayOfWeek` ASC);";
const QString SQLiteStorage::sqlInit4 =
    "CREATE INDEX IF NOT EXISTS `filedate` ON `File` (`Date` ASC);";
const QString SQLiteStorage::sqlAdd =
    "INSERT INTO `TodoList` (`Year`, `Month`, `Day`, `DayOfWeek`, `ColorR`, `ColorG`, `ColorB`, `Text`, `Except`) VALUES (:year, :month, :day, :dow, :r, :g, :b, :text, :except);";
const QString SQLiteStorage::sqlGetByID =
    "SELECT * FROM `TodoList` WHERE `ID` = :id;";
const QString SQLiteStorage::sqlGetByRowID =
    "SELECT * FROM `TodoList` WHERE `RowID` = :id;";
const QString SQLiteStorage::sqlGetAll =
    "SELECT * FROM `TodoList` ORDER BY `ID` ASC;";
const QString SQLiteStorage::sqlGetLike =
    "SELECT * FROM `TodoList` WHERE";
const QString SQLiteStorage::sqlGetLastInsertRowID =
    "SELECT last_insert_rowid() FROM `TodoList`;";
const QString SQLiteStorage::sqlGetByDate =
    "SELECT * FROM `TodoList` WHERE (`Year` = -1 OR `Year` = :year) AND (`Month` = -1 OR `Month` = :month) AND (`Day` = -1 OR `Day` = :day) AND (`DayOfWeek` = -1 OR `DayOfWeek` = :dow) AND INSTR(`Except`, :datestr) = 0 ORDER BY `ID` ASC;";
const QString SQLiteStorage::sqlGetByDateFullMatch =
    "SELECT * FROM `TodoList` WHERE `Year` = :year AND `Month` = :month AND `Day` = :day ORDER BY `ID` ASC;";
const QString SQLiteStorage::sqlUpdate =
    "UPDATE `TodoList` SET `Year` = :year, `Month` = :month, `Day` = :day, `DayOfWeek` = :dow, `ColorR` = :r, `ColorG` = :g, `ColorB` = :b, `Text` = :text, `Except` = :except WHERE `ID` = :id";
const QString SQLiteStorage::sqlDelete =
    "DELETE FROM `TodoList` WHERE `ID` = :id";
const QString SQLiteStorage::sqlDeleteAll =
    "DELETE FROM `TodoList`";
const QString SQLiteStorage::sqlAddFile =
    "INSERT INTO `File` (`Date`, `Name`, `Data`) VALUES (:date, :name, :data);";
const QString SQLiteStorage::sqlGetFileInfoByRowID =
    "SELECT `ID`, `Date`, `Name` FROM `File` WHERE `RowID` = :id;";
const QString SQLiteStorage::sqlGetFileInfoByID =
    "SELECT `ID`, `Date`, `Name` FROM `File` WHERE `ID` = :id;";
const QString SQLiteStorage::sqlGetFileInfoByDate =
    "SELECT `ID`, `Date`, `Name` FROM `File` WHERE `Date` = :date ORDER BY `ID` ASC;";
const QString SQLiteStorage::sqlGetFileByID =
    "SELECT * FROM `File` WHERE `ID` = :id;";
const QString SQLiteStorage::sqlGetFileCountByDate =
    "SELECT COUNT(*) FROM `File` WHERE `Date` = :date;";
const QString SQLiteStorage::sqlDelFile =
    "DELETE FROM `File` WHERE `ID` = :id;";

SQLiteStorage::SQLiteStorage()
    : db(QSqlDatabase::addDatabase("QSQLITE"))
{
    clearTempFiles();
    db.setDatabaseName(databaseFilename);
    db.open();
    QSqlQuery q(db);
    if (!q.prepare(sqlInit1))
    {
        qDebug() << q.lastError().text();
    }
    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    if (!q.prepare(sqlInit2))
    {
        qDebug() << q.lastError().text();
    }
    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    if (!q.prepare(sqlInit3))
    {
        qDebug() << q.lastError().text();
    }
    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    if (!q.prepare(sqlInit4))
    {
        qDebug() << q.lastError().text();
    }
    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }
}

int SQLiteStorage::add(const TodoItem &item)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlAdd))
    {
        qDebug() << q.lastError().text();
    }

    bindTodoItem(q, item);

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    // get inserted ID
    int rowid = -1;
    if (!q.prepare(sqlGetLastInsertRowID))
    {
        qDebug() << q.lastError().text();
    }

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    if (q.next())
    {
        rowid = q.value(0).toInt();
        if (!q.prepare(sqlGetByRowID))
        {
            qDebug() << q.lastError().text();
        }

        q.bindValue(":id", rowid);

        if (!q.exec())
        {
            qDebug() << q.lastError().text();
        }

        if (q.next())
        {
            return q.value("ID").toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

TodoItem SQLiteStorage::get(int id)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlGetByID))
    {
        qDebug() << q.lastError().text();
    }

    q.bindValue(":id", id);

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    return toTodoItem(q);
}

QVector<TodoItem> SQLiteStorage::get(const QDate &date)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlGetByDate))
    {
        qDebug() << q.lastError().text();
    }

    q.bindValue(":year", date.year());
    q.bindValue(":month", date.month());
    q.bindValue(":day", date.day());
    q.bindValue(":dow", date.dayOfWeek());
    q.bindValue(":datestr", date.toString("yyyyMMdd"));

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    return toTodoItems(q);
}

QVector<TodoItem> SQLiteStorage::get(const QString &query)
{
    QString sql = sqlGetLike + " ";
    QStringList list = query.split(' ', QString::SkipEmptyParts);

    if (list.count() == 0)
    {
        return getAll();
    }

    for (int i = 0; i < list.count(); ++i)
    {
        sql += "(`Text` LIKE ?)";
        if (i != list.count() - 1)
        {
            sql += " AND ";
        }
    }
    sql += " ORDER BY `ID` ASC;";
    qDebug() << sql;

    QSqlQuery q(db);
    if (!q.prepare(sql))
    {
        qDebug() << q.lastError().text();
    }

    for (QString &str : list)
    {
        q.addBindValue("%" + str + "%");
    }

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    return toTodoItems(q);
}

QVector<TodoItem> SQLiteStorage::getAll()
{
    QSqlQuery q(db);
    if (!q.prepare(sqlGetAll))
    {
        qDebug() << q.lastError().text();
    }

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    return toTodoItems(q);
}

void SQLiteStorage::set(int id, const TodoItem &item)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlUpdate))
    {
        qDebug() << q.lastError().text();
    }

    q.bindValue(":id", id);
    bindTodoItem(q, item);

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }
}

void SQLiteStorage::del(int id)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlDelete))
    {
        qDebug() << q.lastError().text();
    }

    q.bindValue(":id", id);

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }
}

void SQLiteStorage::delAll()
{
    QSqlQuery q(db);
    if (!q.prepare(sqlDeleteAll))
    {
        qDebug() << q.lastError().text();
    }

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }
}

void SQLiteStorage::bindTodoItem(QSqlQuery &q, const TodoItem &i)
{
    q.bindValue(":year", i.year);
    q.bindValue(":month", i.month);
    q.bindValue(":day", i.day);
    q.bindValue(":dow", i.dayOfWeek);

    int r, g, b;
    i.color.getRgb(&r, &g, &b);
    q.bindValue(":r", r);
    q.bindValue(":g", g);
    q.bindValue(":b", b);
    q.bindValue(":text", i.text);
    q.bindValue(":except", i.except);
}

TodoItem SQLiteStorage::toTodoItem(QSqlQuery &q)
{
    if (q.next())
    {
        TodoItem i;
        i.id = q.value("ID").toInt();
        i.year = q.value("Year").toInt();
        i.month = q.value("Month").toInt();
        i.day = q.value("Day").toInt();
        i.dayOfWeek = q.value("DayOfWeek").toInt();

        int r, g, b;
        r = q.value("ColorR").toInt();
        g = q.value("ColorG").toInt();
        b = q.value("ColorB").toInt();
        i.color = QColor::fromRgb(r, g, b);

        i.text = q.value("Text").toString();
        i.except = q.value("Except").toString();
        return i;
    }
    else
    {
        return TodoItem();
    }
}

QVector<TodoItem> SQLiteStorage::toTodoItems(QSqlQuery &q)
{
    QVector<TodoItem> items;
    while (q.next())
    {
        TodoItem i;
        i.id = q.value("ID").toInt();
        i.year = q.value("Year").toInt();
        i.month = q.value("Month").toInt();
        i.day = q.value("Day").toInt();
        i.dayOfWeek = q.value("DayOfWeek").toInt();

        int r, g, b;
        r = q.value("ColorR").toInt();
        g = q.value("ColorG").toInt();
        b = q.value("ColorB").toInt();
        i.color = QColor::fromRgb(r, g, b);

        i.text = q.value("Text").toString();
        i.except = q.value("Except").toString();
        items.append(i);
    }
    return items;
}

void SQLiteStorage::load()
{
    // nothing to do
}

void SQLiteStorage::save()
{
    // nothing to do
}

FileInfo SQLiteStorage::putFile(const QString &source, const QDate &date)
{
    FileInfo fi;
    QFileInfo file(source);
    QFile f(source);

    QSqlQuery q(db);
    if (!q.prepare(sqlAddFile))
    {
        qDebug() << q.lastError().text();
    }

    q.bindValue(":date", date.toString("yyyyMMdd").toInt());
    q.bindValue(":name", file.fileName());
    fi.fileName = file.fileName();

    f.open(QFile::ReadOnly);
    q.bindValue(":data", f.readAll());

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    // get inserted ID
    int rowid = -1;
    if (!q.prepare(sqlGetLastInsertRowID))
    {
        qDebug() << q.lastError().text();
    }

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    if (q.next())
    {
        rowid = q.value(0).toInt();
        if (!q.prepare(sqlGetFileInfoByRowID))
        {
            qDebug() << q.lastError().text();
        }

        q.bindValue(":id", rowid);

        if (!q.exec())
        {
            qDebug() << q.lastError().text();
        }

        if (q.next())
        {
            fi.id = QString::number(q.value("ID").toInt());
        }
    }
    qDebug() << fi.id << fi.fileName;
    return fi;
}

QVector<FileInfo> SQLiteStorage::getFileList(const QDate &date)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlGetFileInfoByDate))
    {
        qDebug() << q.lastError().text();
    }

    q.bindValue(":date", date.toString("yyyyMMdd").toInt());

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    return toFileInfos(q);
}

FileInfo SQLiteStorage::getFileInfo(const QString &id)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlGetFileInfoByID))
    {
        qDebug() << q.lastError().text();
    }

    q.bindValue(":id", id.toInt());

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    return toFileInfo(q);
}

int SQLiteStorage::getFileCount(const QDate &date)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlGetFileCountByDate))
    {
        qDebug() << q.lastError().text();
    }

    q.bindValue(":date", date.toString("yyyyMMdd").toInt());

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    if (q.next())
    {
        return q.value(0).toInt();
    }
    return 0;
}

QString SQLiteStorage::getFilePath(const QString &id)
{
    // 1. get filename
    FileInfo fi = getFileInfo(id);
    qDebug() << fi.id << fi.fileName;
    QDir dir(tempDirName);
    // TODO progress callback
    if (!dir.exists())
    {
        QDir::root().mkdir(dir.absolutePath());
    }
    QString relFilePath = dir.filePath(fi.fileName);
    QFile f(relFilePath);
    f.open(QFile::WriteOnly);
    f.write(getFileData(id));
    qDebug() << QFileInfo(relFilePath).absoluteFilePath();
    return QFileInfo(relFilePath).absoluteFilePath();
}

QByteArray SQLiteStorage::getFileData(const QString &id)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlGetFileByID))
    {
        qDebug() << q.lastError().text();
    }

    q.bindValue(":id", id.toInt());

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }

    if (q.next())
    {
        return q.value("Data").toByteArray();
    }
    else
    {
        return QByteArray();
    }
}

void SQLiteStorage::delFile(const QString &id)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlDelFile))
    {
        qDebug() << q.lastError().text();
    }

    qDebug() << "deleting" << id;
    q.bindValue(":id", id.toInt());

    if (!q.exec())
    {
        qDebug() << q.lastError().text();
    }
}

FileInfo SQLiteStorage::toFileInfo(QSqlQuery &q)
{
    if (q.next())
    {
        FileInfo i;
        i.id = QString::number(q.value("ID").toInt());
        i.fileName = q.value("Name").toString();
        return i;
    }
    else
    {
        return FileInfo();
    }
}

QVector<FileInfo> SQLiteStorage::toFileInfos(QSqlQuery &q)
{
    QVector<FileInfo> items;
    while (q.next())
    {
        FileInfo i;
        i.id = QString::number(q.value("ID").toInt());
        i.fileName = q.value("Name").toString();
        items.append(i);
    }
    return items;
}

void SQLiteStorage::clearTempFiles()
{
    QDir dir(tempDirName);
    if (dir.exists())
    {
        for (QFileInfo file : dir.entryInfoList())
        {
            QFile(file.absoluteFilePath()).remove();
        }
    }
}

SQLiteStorage::~SQLiteStorage()
{
    db.close();
    clearTempFiles();
}
