#-------------------------------------------------
#
# Project created by QtCreator 2014-08-10T20:54:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app

INCLUDEPATH += ../Algorithm\
                /usr/local/include/boost/

LIBS += -L/usr/local/lib -lboost_system\
        -L/usr/local/lib -lboost_filesystem\
        ../Algorithm/GE.o\
        ../Algorithm/Maker.o

SOURCES += main.cpp\
        mainwindow.cpp\
    newproject.cpp \
    openproject.cpp

HEADERS  += mainwindow.h\
        ../Algorithm/Maker.h\
        ../Algorithm/GE.h \
    newproject.h \
    openproject.h

FORMS    += mainwindow.ui \
    newproject.ui \
    openproject.ui

CONFIG += -stdlib=libc++\
           c++11\
QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
QMAKE_LFLAGS += -mmacosx-version-min=10.7


