#include "storage.h"

TodoStorage *Storage::todoInstance = nullptr;
FileStorage *Storage::fileInstance = nullptr;

Storage::Storage()
{

}

TodoStorage *Storage::todo()
{
    if (!todoInstance)
    {
        todoInstance = new TodoStorage();
    }
    return todoInstance;
}

FileStorage *Storage::file()
{
    if (!fileInstance)
    {
        fileInstance = new FileStorage();
    }
    return fileInstance;
}

