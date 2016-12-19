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
        ui\mainwindow.cpp \
    ui/clientmanagermentwidget.cpp \
    ui/carmanagermentwidget.cpp

HEADERS  += ui\mainwindow.h \
    ui/clientmanagermentwidget.h \
    ui/carmanagermentwidget.h

FORMS    += ui\mainwindow.ui \
    ui/clientmanagermentwidget.ui \
    ui/carmanagermentwidget.ui

RESOURCES += \
    icon.qrc
