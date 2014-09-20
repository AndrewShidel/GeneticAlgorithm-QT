#-------------------------------------------------
#
# Project created by QtCreator 2014-08-10T20:54:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app

INCLUDEPATH += ../Algorithm

RESOURCES = application.qrc \
    application.qrc


SOURCES += main.cpp\
        mainwindow.cpp\
    newproject.cpp \
    openproject.cpp \
    rundialog.cpp \
    tracker.cpp

HEADERS  += mainwindow.h\
        ../Algorithm/Maker.h\
        ../Algorithm/GE.h \
    newproject.h \
    openproject.h \
    rundialog.h \
    tracker.h

FORMS    += mainwindow.ui \
    newproject.ui \
    openproject.ui \
    rundialog.ui

OTHER_FILES += \
    ../Algorithm/statTracker.o\
    ../Algorithm/GE.o

