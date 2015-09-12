#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T19:14:49
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sqlite_viewer
TEMPLATE = app


SOURCES += main.cpp
SOURCES += form.cpp
SOURCES += SQLiteDB.cpp

HEADERS  += form.h
HEADERS  += SQLiteDB.h

FORMS    += form.ui
