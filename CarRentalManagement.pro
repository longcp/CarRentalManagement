#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T10:33:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarRentalManagement
TEMPLATE = app

INCLUDEPATH += ui/
INCLUDEPATH += utils/

SOURCES += main.cpp\
        ui\mainwindow.cpp

HEADERS  += ui\mainwindow.h

FORMS    += ui\mainwindow.ui

RESOURCES += \
    icon.qrc
