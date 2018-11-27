QT += sql

QT       += core gui axcontainer

QT += widgets

CONFIG += c++11
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    csharptool.cpp \
    exceltool.cpp \
    sqlitetool.cpp \
    log.cpp \
    sqliteprocessresult.cpp \
    sqlite3generatetool.cpp \
    config.cpp \
    workthread.cpp

HEADERS += \
    csharptool.h \
    exceltool.h \
    sqlitetool.h \
    log.h \
    sqliteprocessresult.h \
    sqlite3generatetool.h \
    config.h \
    workthread.h

FORMS += \
    sqlite3generatetool.ui
