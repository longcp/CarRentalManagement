#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.resize(1200, 800);
    w.setWindowTitle("泵车租赁管理软件");
    w.show();

    return a.exec();
}
