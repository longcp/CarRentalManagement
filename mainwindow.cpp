#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //菜单栏
    mSystemSettingMenu  = ui->menuBar->addMenu(tr("系统设置"));
    mInformationMenu    = ui->menuBar->addMenu(tr("基本资料"));
    mRentalManagementMenu   = ui->menuBar->addMenu(tr("租赁管理"));
    mFinanceManagementMenu  = ui->menuBar->addMenu(tr("财务统计"));
    mStatisticalReportMenu  = ui->menuBar->addMenu(tr("统计报表"));
    mAboutMenu = ui->menuBar->addMenu(tr("关于"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
