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
INCLUDEPATH += ui/first_level/
INCLUDEPATH += ui/second_level/
INCLUDEPATH += utils/

SOURCES += main.cpp\
        ui\mainwindow.cpp \
    ui/first_level/clientmanagermentwidget.cpp \
    ui/first_level/carmanagermentwidget.cpp \
    ui/first_level/rentaldocumentwidget.cpp \
    ui/first_level/receivablewidget.cpp \
    ui/first_level/receiptwidget.cpp \
    ui/first_level/contractwidget.cpp \
    ui/second_level/clienteditdialog.cpp

HEADERS  += ui\mainwindow.h \
    ui/first_level/clientmanagermentwidget.h \
    ui/first_level/carmanagermentwidget.h \
    ui/first_level/rentaldocumentwidget.h \
    ui/first_level/receivablewidget.h \
    ui/first_level/receiptwidget.h \
    ui/first_level/contractwidget.h \
    ui/second_level/clienteditdialog.h

FORMS    += ui\mainwindow.ui \
    ui/first_level/clientmanagermentwidget.ui \
    ui/first_level/carmanagermentwidget.ui \
    ui/first_level/rentaldocumentwidget.ui \
    ui/first_level/receivablewidget.ui \
    ui/first_level/receiptwidget.ui \
    ui/first_level/contractwidget.ui \
    ui/second_level/clienteditdialog.ui

RESOURCES += \
    icon.qrc
