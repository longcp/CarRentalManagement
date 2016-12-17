#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //菜单栏
    mSystemSettingMenu  = ui->menuBar->addMenu(tr("系统设置"));
    mRentalManagementMenu   = ui->menuBar->addMenu(tr("租赁管理"));
    mFinanceManagementMenu  = ui->menuBar->addMenu(tr("财务统计"));
    mStatisticalReportMenu  = ui->menuBar->addMenu(tr("统计报表"));
    mUploadPictureMenu  = ui->menuBar->addMenu(tr("上传图片"));
    mUserManagermentMenu    =ui->menuBar->addMenu(tr("用户管理"));
    mAboutMenu = ui->menuBar->addMenu(tr("关于"));

}

MainWindow::~MainWindow()
{
    delete ui;
}
