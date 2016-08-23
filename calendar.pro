QT += core gui widgets

TEMPLATE = app
TARGET = calendar
INCLUDEPATH += .

# Input
SOURCES += main.cpp calendar.cpp \
    todolist.cpp \
    storage.cpp \
    todoitem.cpp \
    widget.cpp

HEADERS += \
    calendar.h \
    todolist.h \
    storage.h \
    todoitem.h \
    widget.h

FORMS += \
    todolist.ui
