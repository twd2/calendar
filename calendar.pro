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
    draggablelist.cpp

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
    draggablelist.h

FORMS +=

TRANSLATIONS += zh_CN.ts

RESOURCES += \
    res.qrc
