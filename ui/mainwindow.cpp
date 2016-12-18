#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <clientmanagermentwidget.h>
#include <QList>
//#include <Tlhelp32.h>

#define LOG_TAG                         "MAIN_WINDOW"
#include "utils/Log.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mClientWidget = new ClientManagermentWidget();

    // 插入首页，首页一直存在
    mOpenTabList.insert(0, ui->homePageTab);

    // 菜单栏
    mMenuSystemSetting  = ui->menuBar->addMenu(tr("系统设置"));
    mMenuRentalManagement   = ui->menuBar->addMenu(tr("租赁管理"));
    mMenuCarManagement  = ui->menuBar->addMenu(tr("车辆管理"));
    mMenuFinanceManagement  = ui->menuBar->addMenu(tr("财务统计"));
    mMenuStatisticalReport  = ui->menuBar->addMenu(tr("统计报表"));
    mMenuUploadPicture  = ui->menuBar->addMenu(tr("上传图片"));
    mMenuUserManagerment    =ui->menuBar->addMenu(tr("用户管理"));
    mMenuAbout = ui->menuBar->addMenu(tr("关于"));
    mActExitSystem = mMenuSystemSetting->addAction(
                QIcon(":/menu/icon/exit.png"), tr("退出系统"));
    mActClient = mMenuRentalManagement->addAction(
                QIcon(":/menu/icon/client.png"), tr("客户资料"));
    mActContract = mMenuRentalManagement->addAction(
                QIcon(":/menu/icon/contract_64.ico"), tr("租赁合同"));
    mActPump = mMenuRentalManagement->addAction(
                QIcon(":/menu/icon/pump_64.ico"), tr("泵送"));
    mActCarfile = mMenuCarManagement->addAction(
                QIcon(":/menu/icon/Ford_Heavy_Wreck_Truck_128.ico"),
                tr("车辆档案"));
    mActCarMaintenance = mMenuCarManagement->addAction(
                QIcon(":/menu/icon/tools.ico"), tr("维修记录"));
    mActCarIllegal = mMenuCarManagement->addAction(
                QIcon(":/menu/icon/car_break_rules_64.ico"),
                tr("违章记录"));

    // 工具栏
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(mActClient);
    ui->mainToolBar->addAction(mActContract);
    ui->mainToolBar->addAction(mActPump);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(mActCarfile);
    ui->mainToolBar->addAction(mActCarMaintenance);
    ui->mainToolBar->addAction(mActCarIllegal);
    ui->mainToolBar->addSeparator();

    /********** signals and slots *********/
    /**
     * @brief 退出系统
     */
    connect(mActExitSystem, SIGNAL(triggered()),
            this, SLOT(exitSystem()));
    connect(mActClient, SIGNAL(triggered()),
            this, SLOT(openClientWidget()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("退出系统");
    msgBox.setText("确定要退出系统吗？");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes
                              |QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Yes, "确定");
    msgBox.setButtonText(QMessageBox::Cancel, "取消");

    if (QMessageBox::Yes == msgBox.exec())
        QApplication::exit(0);
    else
        event->ignore();
}

void
MainWindow::exitSystem()
{
    QCloseEvent *event;
    closeEvent(event);
}

void
MainWindow::openClientWidget()
{
    ALOGD("MainWindow::openClientWidget enter");
    if (mOpenTabList.isEmpty())
        return;

    int size = mOpenTabList.size();
    int index = mOpenTabList.indexOf(mClientWidget);

    if (index >= 0) {
        ui->mainTabWidget->setCurrentIndex(index);
        return;
    }
    mOpenTabList.insert(size, mClientWidget);
    ui->mainTabWidget->addTab(mClientWidget, tr("客户"));
    ui->mainTabWidget->setCurrentIndex(size);
}
