#ifndef STORAGE_H
#define STORAGE_H

#include "todostorage.h"
#include "filestorage.h"

class Storage
{
private:
    static TodoStorage *todoInstance;
    static FileStorage *fileInstance;
public:
    static TodoStorage *todo();
    static FileStorage *file();
    Storage();
};

#endif // STORAGE_H
