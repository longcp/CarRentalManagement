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
INCLUDEPATH += ui/dialog/contract/
INCLUDEPATH += ui/dialog/client/
INCLUDEPATH += ui/dialog/rentaldocument/
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
    ui/dialog/receipteditdialog.cpp \
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
    ui/dialog/car/insurancedialog.cpp \
    common/contract.cpp \
    ui/dialog/contract/contractpricedialog.cpp \
    ui/dialog/contract/contracteditdialog.cpp \
    common/rentaldocument.cpp \
    ui/dialog/rentaldocument/rentaldocumenteditdialog.cpp \
    ui/dialog/contract/contracttabledialog.cpp \
    ui/dialog/client/clienteditdialog.cpp \
    ui/dialog/client/clienttabledialog.cpp \
    ui/dialog/car/cartabledialog.cpp \
    ui/dialog/rentaldocument/rentaldoctabledialog.cpp \
    utils/floatoperator.cpp \
    ui/dialog/contract/pricetabledialog.cpp \
    utils/util.cpp

HEADERS  += ui\mainwindow.h \
    ui/tabwidget/clientmanagermentwidget.h \
    ui/tabwidget/carmanagermentwidget.h \
    ui/tabwidget/rentaldocumentwidget.h \
    ui/tabwidget/receivablewidget.h \
    ui/tabwidget/receiptwidget.h \
    ui/tabwidget/contractwidget.h \
    ui/dialog/receipteditdialog.h \
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
    utils/Log.h \
    common/contract.h \
    ui/dialog/contract/contractpricedialog.h \
    ui/dialog/contract/contracteditdialog.h \
    common/rentaldocument.h \
    ui/dialog/rentaldocument/rentaldocumenteditdialog.h \
    ui/dialog/contract/contracttabledialog.h \
    ui/dialog/client/clienteditdialog.h \
    ui/dialog/client/clienttabledialog.h \
    ui/dialog/car/cartabledialog.h \
    ui/dialog/rentaldocument/rentaldoctabledialog.h \
    utils/floatoperator.h \
    ui/dialog/contract/pricetabledialog.h \
    utils/util.h

FORMS    += ui\mainwindow.ui \
    ui/tabwidget/clientmanagermentwidget.ui \
    ui/tabwidget/carmanagermentwidget.ui \
    ui/tabwidget/rentaldocumentwidget.ui \
    ui/tabwidget/receivablewidget.ui \
    ui/tabwidget/receiptwidget.ui \
    ui/tabwidget/contractwidget.ui \
    ui/dialog/receipteditdialog.ui \
    ui/dialog/user/logindialog.ui \
    ui/dialog/user/usermanagerdialog.ui \
    ui/dialog/user/modifypasswarddialog.ui \
    ui/dialog/user/adduserdialog.ui \
    ui/dialog/versiondialog.ui \
    ui/dialog/car/carannualdialog.ui \
    ui/dialog/car/careditdialog.ui \
    ui/dialog/car/insurancedialog.ui \
    ui/dialog/contract/contractpricedialog.ui \
    ui/dialog/contract/contracteditdialog.ui \
    ui/dialog/rentaldocument/rentaldocumenteditdialog.ui \
    ui/dialog/contract/contracttabledialog.ui \
    ui/dialog/client/clienteditdialog.ui \
    ui/dialog/client/clienttabledialog.ui \
    ui/dialog/car/cartabledialog.ui \
    ui/dialog/rentaldocument/rentaldoctabledialog.ui \
    ui/dialog/contract/pricetabledialog.ui

RESOURCES += \
    icon.qrc
