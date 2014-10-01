#-------------------------------------------------
#
# Project created by QtCreator 2011-07-12T01:24:02
#
#-------------------------------------------------

QT       += core gui

TARGET = ballgame
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainthread.cpp \
    Actor.cpp \
    Ship.cpp \
    Bullet.cpp \
    Meteorite.cpp \
    globals.cpp

HEADERS  += mainwindow.h \
    mainthread.h \
    Actor.h \
    Ship.h \
    Bullet.h \
    Meteorite.h \
    globals.h

FORMS    += mainwindow.ui
