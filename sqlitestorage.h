#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include "todostorage.h"
#include "filestorage.h"

class SQLiteStorage
    : public TodoStorage
{
public:
    SQLiteStorage();
};

#endif // SQLITESTORAGE_H
