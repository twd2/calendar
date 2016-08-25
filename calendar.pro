QT += core gui widgets

TEMPLATE = app
TARGET = calendar
INCLUDEPATH += .

# Input
SOURCES += main.cpp \
    todolist.cpp \
    storage.cpp \
    todoitem.cpp \
    widget.cpp \
    calendar.cpp \
    dateitem.cpp \
    todoedit.cpp \
    global.cpp \
    fileinfo.cpp \
    draggablelabel.cpp \
    draggablelist.cpp \
    todostorage.cpp \
    filestorage.cpp \
    sqlitestorage.cpp

HEADERS += \
    todolist.h \
    storage.h \
    todoitem.h \
    widget.h \
    calendar.h \
    dateitem.h \
    todoedit.h \
    global.h \
    fileinfo.h \
    draggablelabel.h \
    draggablelist.h \
    todostorage.h \
    filestorage.h \
    sqlitestorage.h

win32:LIBS += -lkernel32 -luser32

FORMS +=

TRANSLATIONS += zh_CN.ts

RESOURCES += \
    res.qrc

DISTFILES += \
    zh_CN.ts
