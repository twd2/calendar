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
    global.cpp

HEADERS += \
    todolist.h \
    storage.h \
    todoitem.h \
    widget.h \
    calendar.h \
    dateitem.h \
    todoedit.h \
    global.h

FORMS +=
