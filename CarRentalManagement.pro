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
INCLUDEPATH += ui/tabwidget/
INCLUDEPATH += ui/dialog/
INCLUDEPATH += ui/dialog/user/
INCLUDEPATH += utils/
INCLUDEPATH += common/

SOURCES += main.cpp\
        ui\mainwindow.cpp \
    ui/tabwidget/clientmanagermentwidget.cpp \
    ui/tabwidget/carmanagermentwidget.cpp \
    ui/tabwidget/rentaldocumentwidget.cpp \
    ui/tabwidget/receivablewidget.cpp \
    ui/tabwidget/receiptwidget.cpp \
    ui/tabwidget/contractwidget.cpp \
    ui/dialog/clienteditdialog.cpp \
    ui/dialog/careditdialog.cpp \
    ui/dialog/receipteditdialog.cpp \
    ui/dialog/rentaldocumenteditdialog.cpp \
    common/client.cpp \
    common/regularexpression.cpp \
    database/database.cpp \
    common/car.cpp \
    ui/dialog/user/login.cpp \
    common/user.cpp

HEADERS  += ui\mainwindow.h \
    ui/tabwidget/clientmanagermentwidget.h \
    ui/tabwidget/carmanagermentwidget.h \
    ui/tabwidget/rentaldocumentwidget.h \
    ui/tabwidget/receivablewidget.h \
    ui/tabwidget/receiptwidget.h \
    ui/tabwidget/contractwidget.h \
    ui/dialog/clienteditdialog.h \
    ui/dialog/careditdialog.h \
    ui/dialog/receipteditdialog.h \
    ui/dialog/rentaldocumenteditdialog.h \
    common/client.h \
    common/regularexpression.h \
    common/datatype.h \
    database/database.h \
    common/tablemodel.h \
    common/car.h \
    ui/dialog/user/login.h \
    common/user.h

FORMS    += ui\mainwindow.ui \
    ui/tabwidget/clientmanagermentwidget.ui \
    ui/tabwidget/carmanagermentwidget.ui \
    ui/tabwidget/rentaldocumentwidget.ui \
    ui/tabwidget/receivablewidget.ui \
    ui/tabwidget/receiptwidget.ui \
    ui/tabwidget/contractwidget.ui \
    ui/dialog/clienteditdialog.ui \
    ui/dialog/careditdialog.ui \
    ui/dialog/receipteditdialog.ui \
    ui/dialog/rentaldocumenteditdialog.ui \
    ui/dialog/user/login.ui

RESOURCES += \
    icon.qrc
