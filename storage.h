#ifndef STORAGE_H
#define STORAGE_H

#include "todostorage.h"
#include "filestorage.h"
#include "sqlitestorage.h"

class Storage
{
private:
    static SQLiteStorage *sqlInstance;
public:
    static TodoStorage *todo();
    static FileStorage *file();
    static void end();
    Storage();
};

#endif // STORAGE_H
