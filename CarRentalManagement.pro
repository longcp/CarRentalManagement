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
INCLUDEPATH += ui/dialog/car/
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
    ui/dialog/receipteditdialog.cpp \
    ui/dialog/rentaldocumenteditdialog.cpp \
    common/client.cpp \
    common/regularexpression.cpp \
    database/database.cpp \
    common/car.cpp \
    common/user.cpp \
    ui/dialog/user/logindialog.cpp \
    ui/dialog/user/usermanagerdialog.cpp \
    ui/dialog/user/modifypasswarddialog.cpp \
    ui/dialog/user/adduserdialog.cpp \
    ui/dialog/versiondialog.cpp \
    ui/dialog/car/carannualdialog.cpp \
    ui/dialog/car/careditdialog.cpp \
    ui/dialog/car/insurancedialog.cpp

HEADERS  += ui\mainwindow.h \
    ui/tabwidget/clientmanagermentwidget.h \
    ui/tabwidget/carmanagermentwidget.h \
    ui/tabwidget/rentaldocumentwidget.h \
    ui/tabwidget/receivablewidget.h \
    ui/tabwidget/receiptwidget.h \
    ui/tabwidget/contractwidget.h \
    ui/dialog/clienteditdialog.h \
    ui/dialog/receipteditdialog.h \
    ui/dialog/rentaldocumenteditdialog.h \
    common/client.h \
    common/regularexpression.h \
    common/datatype.h \
    database/database.h \
    common/tablemodel.h \
    common/car.h \
    common/user.h \
    ui/dialog/user/logindialog.h \
    ui/dialog/user/usermanagerdialog.h \
    ui/dialog/user/modifypasswarddialog.h \
    ui/dialog/user/adduserdialog.h \
    ui/dialog/versiondialog.h \
    ui/dialog/car/carannualdialog.h \
    ui/dialog/car/careditdialog.h \
    ui/dialog/car/insurancedialog.h \
    utils/Log.h

FORMS    += ui\mainwindow.ui \
    ui/tabwidget/clientmanagermentwidget.ui \
    ui/tabwidget/carmanagermentwidget.ui \
    ui/tabwidget/rentaldocumentwidget.ui \
    ui/tabwidget/receivablewidget.ui \
    ui/tabwidget/receiptwidget.ui \
    ui/tabwidget/contractwidget.ui \
    ui/dialog/clienteditdialog.ui \
    ui/dialog/receipteditdialog.ui \
    ui/dialog/rentaldocumenteditdialog.ui \
    ui/dialog/user/logindialog.ui \
    ui/dialog/user/usermanagerdialog.ui \
    ui/dialog/user/modifypasswarddialog.ui \
    ui/dialog/user/adduserdialog.ui \
    ui/dialog/versiondialog.ui \
    ui/dialog/car/carannualdialog.ui \
    ui/dialog/car/careditdialog.ui \
    ui/dialog/car/insurancedialog.ui

RESOURCES += \
    icon.qrc
