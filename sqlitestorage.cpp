#include "sqlitestorage.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

const QString SQLiteStorage::databaseFilename = "cal.db3";
const QString SQLiteStorage::sqlInit1 =
    "CREATE TABLE IF NOT EXISTS `File` ( `ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `Filename` TEXT, `Data` BLOB );";
const QString SQLiteStorage::sqlInit2 =
    "CREATE TABLE IF NOT EXISTS `TodoList` ( `ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `Year` INTEGER NOT NULL, `Month` INTEGER NOT NULL, `Day` INTEGER NOT NULL, `DayOfWeek` INTEGER NOT NULL, `ColorR` INTEGER NOT NULL, `ColorG` INTEGER NOT NULL, `ColorB` INTEGER NOT NULL, `Text` TEXT, `Except` TEXT );";
const QString SQLiteStorage::sqlInit3 =
    "CREATE INDEX IF NOT EXISTS `date` ON `TodoList` (`Year` ASC,`Month` ASC,`Day` ASC,`DayOfWeek` ASC);";
const QString SQLiteStorage::sqlAdd =
    "INSERT INTO `TodoList` (`Year`, `Month`, `Day`, `DayOfWeek`, `ColorR`, `ColorG`, `ColorB`, `Text`, `Except`) VALUES (:year, :month, :day, :dow, :r, :g, :b, :text, :except);";
const QString SQLiteStorage::sqlGetByID =
    "SELECT * FROM `TodoList` WHERE `ID` = :id;";
const QString SQLiteStorage::sqlGetByRowID =
    "SELECT * FROM `TodoList` WHERE `RowID` = :id;";
const QString SQLiteStorage::sqlGetAll =
    "SELECT * FROM `TodoList`;";
const QString SQLiteStorage::sqlGetLike =
    "SELECT * FROM `TodoList` WHERE";
const QString SQLiteStorage::sqlGetLastInsertRowID =
    "SELECT last_insert_rowid() FROM `TodoList`;";
const QString SQLiteStorage::sqlGetByDate =
    "SELECT * FROM `TodoList` WHERE (`Year` = -1 OR `Year` = :year) AND (`Month` = -1 OR `Month` = :month) AND (`Day` = -1 OR `Day` = :day) AND (`DayOfWeek` = -1 OR `DayOfWeek` = :dow) AND INSTR(`Except`, :datestr) = 0;";
const QString SQLiteStorage::sqlGetByDateFullMatch =
    "SELECT * FROM `TodoList` WHERE `Year` = :year AND `Month` = :month AND `Day` = :day;";
const QString SQLiteStorage::sqlUpdate =
    "UPDATE `TodoList` SET `Year` = :year, `Month` = :month, `Day` = :day, `DayOfWeek` = :dow, `ColorR` = :r, `ColorG` = :g, `ColorB` = :b, `Text` = :text, `Except` = :except WHERE `ID` = :id";
const QString SQLiteStorage::sqlDelete =
    "DELETE FROM `TodoList` WHERE `ID` = :id";
const QString SQLiteStorage::sqlDeleteAll =
    "DELETE FROM `TodoList`";

SQLiteStorage::SQLiteStorage()
    : db(QSqlDatabase::addDatabase("QSQLITE"))
{
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
        if (!q.prepare(sqlGetByID))
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
            return q.value(0).toInt();
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
    sql += ';';
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

SQLiteStorage::~SQLiteStorage()
{
    db.close();
}
