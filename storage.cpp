#include "storage.h"

SQLiteStorage *Storage::sqlInstance = nullptr;

Storage::Storage()
{

}

TodoStorage *Storage::todo()
{
    if (!sqlInstance)
    {
        sqlInstance = new SQLiteStorage();
    }
    return sqlInstance;
}

FileStorage *Storage::file()
{
    if (!sqlInstance)
    {
        sqlInstance = new SQLiteStorage();
    }
    return sqlInstance;
}

void Storage::end()
{
    if (sqlInstance)
    {
        delete sqlInstance;
        sqlInstance = nullptr;
    }
}
