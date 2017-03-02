#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "database/database.h"
#include <QDebug>

DataBase* DataBase::mInstance = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));     //设置gbd编码

    w.resize(1200, 800);
    w.setWindowTitle("泵车租赁管理软件");
    w.setWindowState(Qt::WindowMaximized);                              //窗口最大化
    w.show();

    return a.exec();
}
