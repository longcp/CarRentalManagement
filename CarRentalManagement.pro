#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T10:33:25
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT	 += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarRentalManagement
TEMPLATE = app

INCLUDEPATH += ui/
INCLUDEPATH += ui/first_level/
INCLUDEPATH += ui/second_level/
INCLUDEPATH += utils/
INCLUDEPATH += common/

SOURCES += main.cpp\
        ui\mainwindow.cpp \
    ui/first_level/clientmanagermentwidget.cpp \
    ui/first_level/carmanagermentwidget.cpp \
    ui/first_level/rentaldocumentwidget.cpp \
    ui/first_level/receivablewidget.cpp \
    ui/first_level/receiptwidget.cpp \
    ui/first_level/contractwidget.cpp \
    ui/second_level/clienteditdialog.cpp \
    ui/second_level/careditdialog.cpp \
    ui/second_level/receipteditdialog.cpp \
    ui/second_level/rentaldocumenteditdialog.cpp \
    common/client.cpp \
    common/regularexpression.cpp \
    database/database.cpp \
    common/car.cpp

HEADERS  += ui\mainwindow.h \
    ui/first_level/clientmanagermentwidget.h \
    ui/first_level/carmanagermentwidget.h \
    ui/first_level/rentaldocumentwidget.h \
    ui/first_level/receivablewidget.h \
    ui/first_level/receiptwidget.h \
    ui/first_level/contractwidget.h \
    ui/second_level/clienteditdialog.h \
    ui/second_level/careditdialog.h \
    ui/second_level/receipteditdialog.h \
    ui/second_level/rentaldocumenteditdialog.h \
    common/client.h \
    common/regularexpression.h \
    common/datatype.h \
    database/database.h \
    common/tablemodel.h \
    common/car.h

FORMS    += ui\mainwindow.ui \
    ui/first_level/clientmanagermentwidget.ui \
    ui/first_level/carmanagermentwidget.ui \
    ui/first_level/rentaldocumentwidget.ui \
    ui/first_level/receivablewidget.ui \
    ui/first_level/receiptwidget.ui \
    ui/first_level/contractwidget.ui \
    ui/second_level/clienteditdialog.ui \
    ui/second_level/careditdialog.ui \
    ui/second_level/receipteditdialog.ui \
    ui/second_level/rentaldocumenteditdialog.ui

RESOURCES += \
    icon.qrc
