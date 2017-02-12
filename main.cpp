#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "database/database.h"

DataBase* DataBase::mInstance = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //还是会乱码
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
//    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    w.resize(1200, 800);
    w.setWindowTitle("泵车租赁管理软件");
    w.show();

    return a.exec();
}\
