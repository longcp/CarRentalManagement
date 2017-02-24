#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <clientmanagermentwidget.h>
#include <carmanagermentwidget.h>
#include <rentaldocumentwidget.h>
#include <receivablewidget.h>
#include <receiptwidget.h>
#include <contractwidget.h>
#include <QList>
#include <QToolButton>
#include <QTabWidget>
#include <logindialog.h>
#include <usermanagerdialog.h>
//#include <Tlhelp32.h>

#define LOG_TAG                         "MAIN_WINDOW"
#include "utils/Log.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainTabWidget->setTabIcon(0, QIcon(":/menu/icon/home_64.ico"));

    mLoginDialog = new LoginDialog();
    mUserManagerDialog = new UserManagerDialog();

    /**
     * @brief 用户登陆
     */
    connect(mLoginDialog, SIGNAL(userLoginSignal(QString)),
            this, SLOT(userLoginSlot(QString)));
    /**
     * @brief 根据用户权限初始化窗口
     */
    connect(mLoginDialog, SIGNAL(userLoginSignal(QString)),
            mUserManagerDialog, SLOT(setWindow(QString)));

    mLoginDialog->exec();

    // 子tab
    mCarWidget = new CarManagermentWidget();
    mClientWidget = new ClientManagermentWidget();
    mReceiptWidget= new ReceiptWidget();
    mContractWidget = new ContractWidget();
    mRentalDocWidget = new RentalDocumentWidget();
    mReceivableWidget= new ReceivableWidget();

    // 插入首页，首页一直存在
    mOpenTabList.insert(0, ui->homePageTab);

    // 菜单栏
    mMenuSystemSetting  = ui->menuBar->addMenu(tr("系统设置"));
    mMenuRentalManagement   = ui->menuBar->addMenu(tr("租赁管理"));
    mMenuCarManagement  = ui->menuBar->addMenu(tr("车辆管理"));
    mMenuFinanceManagement  = ui->menuBar->addMenu(tr("财务统计"));
    mMenuUploadPicture  = ui->menuBar->addMenu(tr("上传图片"));
    mMenuUserManagerment= ui->menuBar->addMenu(tr("用户管理"));
    mMenuAbout = ui->menuBar->addMenu(tr("关于"));
    mActExitSystem = mMenuSystemSetting->addAction(
                QIcon(":/menu/icon/exit.png"),
                tr("退出系统"));
    mActClient = mMenuRentalManagement->addAction(
                QIcon(":/menu/icon/client.png"),
                tr("客户资料"));
    mActContract = mMenuRentalManagement->addAction(
                QIcon(":/menu/icon/contract_64.ico"),
                tr("租赁合同"));
    mActPump = mMenuRentalManagement->addAction(
                QIcon(":/menu/icon/pump_64.ico"),
                tr("泵送"));
    mActCarfile = mMenuCarManagement->addAction(
                QIcon(":/menu/icon/Ford_Heavy_Wreck_Truck_128.ico"),
                tr("车辆档案"));
    mActReceivable = mMenuFinanceManagement->addAction(
                QIcon(":/menu/icon/custom-reports256.png"),
                tr("应收"));
    mActReceipt = mMenuFinanceManagement->addAction(
                QIcon(":/menu/icon/calculator_64.ico"),
                tr("收款"));
    mActUserManager = mMenuUserManagerment->addAction(
                QIcon(":/menu/icon/user_64.ico"),
                tr("用户管理"));

    // 工具栏
    ui->mainToolBar->setMovable(false);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);                                // 工具栏不可拖动
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(mActClient);
    ui->mainToolBar->addAction(mActContract);
    ui->mainToolBar->addAction(mActPump);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(mActCarfile);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(mActReceivable);
    ui->mainToolBar->addAction(mActReceipt);
    ui->mainToolBar->addAction(mActUserManager);

    // tabwidget
    // 设置Tab标签的右上角的关闭按钮
    QToolButton *closeTabButton = new QToolButton(ui->mainTabWidget);
    closeTabButton->setAutoRaise(true);
    closeTabButton->setCursor(Qt::ArrowCursor);
    closeTabButton->setIcon(QIcon(":/menu/icon/close_64.ico"));
    closeTabButton->setToolTip("关闭当前Tab页");
    ui->mainTabWidget->setCornerWidget(closeTabButton,
                                       Qt::TopRightCorner);
    ui->mainTabWidget->setTabsClosable(true);

    /********** signals and slots *********/
    /**
     * @brief 退出系统
     */
    connect(mActExitSystem, SIGNAL(triggered()),
            this, SLOT(exitSystem()));
    /**
     * @brief 打开客户资料窗口
     */
    connect(mActClient, SIGNAL(triggered()),
            this, SLOT(openClientWidget()));
    /**
     * @brief 打开车辆档案窗口
     */
    connect(mActCarfile, SIGNAL(triggered()),
            this, SLOT(openCarWidget()));
    /**
     * @brief 打开泵送确认单窗口
     */
//    connect(mActPump, SIGNAL(triggered()),
//            this, SLOT(openRentalDocWindow()));
    /**
     * @brief 打开泵送确认单窗口
     */
    connect(mActPump, SIGNAL(triggered()),
            this, SLOT(openRentalDocWidget()));
    /**
     * @brief 打开应收窗口
     */
    connect(mActReceivable, SIGNAL(triggered()),
            this, SLOT(openReceivableWidget()));
    /**
     * @brief 打开合同收款窗口
     */
    connect(mActReceipt, SIGNAL(triggered()),
            this, SLOT(openReceiptWidget()));
    /**
     * @brief 打开租赁合同窗口
     */
    connect(mActContract, SIGNAL(triggered()),
            this, SLOT(openContractWidget()));
    /**
     * @brief 打开用户管理窗口
     */
    connect(mActUserManager, SIGNAL(triggered()),
            mUserManagerDialog, SLOT(openWindow()));
    /**
     * @brief tab关闭事件
     */
    connect(ui->mainTabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(removeTab(int)));
    /**
     * @brief 关闭当前页
     */
    connect(closeTabButton, SIGNAL(clicked()),
            this, SLOT(removeCurTab()));
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
    if (mOpenTabList.isEmpty())
        return;

    int size = mOpenTabList.size();
    int index = mOpenTabList.indexOf(mClientWidget);

    if (index >= 0) {
        ui->mainTabWidget->setCurrentIndex(index);
        return;
    }
    mOpenTabList.insert(size, mClientWidget);
    ui->mainTabWidget->addTab(mClientWidget,
                              QIcon(":/menu/icon/client.png"),
                              tr("客户资料"));
    ui->mainTabWidget->setCurrentIndex(size);
}

void
MainWindow::openCarWidget()
{
    if (mOpenTabList.isEmpty())
        return;

    int size = mOpenTabList.size();
    int index = mOpenTabList.indexOf(mCarWidget);

    if (index >= 0) {
        ui->mainTabWidget->setCurrentIndex(index);
        return;
    }
    mOpenTabList.insert(size, mCarWidget);
    ui->mainTabWidget->addTab(mCarWidget,
                              QIcon(":/menu/icon/Ford_Heavy_Wreck_Truck_128.ico"),
                              tr("车辆档案"));
    ui->mainTabWidget->setCurrentIndex(size);
}

void
MainWindow::openRentalDocWidget()
{
    if (mOpenTabList.isEmpty())
        return;

    int size = mOpenTabList.size();
    int index = mOpenTabList.indexOf(mRentalDocWidget);

    if (index >= 0) {
        ui->mainTabWidget->setCurrentIndex(index);
        return;
    }
    mOpenTabList.insert(size, mRentalDocWidget);
    ui->mainTabWidget->addTab(mRentalDocWidget,
                              QIcon(":/menu/icon/pump_64.ico"),
                              tr("泵送签证单"));
    ui->mainTabWidget->setCurrentIndex(size);
}

void
MainWindow::openReceivableWidget()
{
    if (mOpenTabList.isEmpty())
        return;

    int size = mOpenTabList.size();
    int index = mOpenTabList.indexOf(mReceivableWidget);

    if (index >= 0) {
        ui->mainTabWidget->setCurrentIndex(index);
        return;
    }
    mOpenTabList.insert(size, mReceivableWidget);
    ui->mainTabWidget->addTab(mReceivableWidget,
                              QIcon(":/menu/icon/custom-reports256.png"),
                              tr("应收"));
    ui->mainTabWidget->setCurrentIndex(size);
}

void
MainWindow::openReceiptWidget()
{
    if (mOpenTabList.isEmpty())
        return;

    int size = mOpenTabList.size();
    int index = mOpenTabList.indexOf(mReceiptWidget);

    if (index >= 0) {
        ui->mainTabWidget->setCurrentIndex(index);
        return;
    }
    mOpenTabList.insert(size, mReceiptWidget);
    ui->mainTabWidget->addTab(mReceiptWidget,
                              QIcon(":/menu/icon/calculator_64.ico"),
                              tr("收款"));
    ui->mainTabWidget->setCurrentIndex(size);
}

void
MainWindow::openContractWidget()
{
    if (mOpenTabList.isEmpty())
        return;

    int size = mOpenTabList.size();
    int index = mOpenTabList.indexOf(mContractWidget);

    if (index >= 0) {
        ui->mainTabWidget->setCurrentIndex(index);
        return;
    }
    mOpenTabList.insert(size, mContractWidget);
    ui->mainTabWidget->addTab(mContractWidget,
                              QIcon(":/menu/icon/contract_64.ico"),
                              tr("租赁合同"));
    ui->mainTabWidget->setCurrentIndex(size);
}

void
MainWindow::removeTab(int index)
{
    ALOGD("index = %d", index);
    if (index == 0)
        return;

    mOpenTabList.removeAt(index);
    ui->mainTabWidget->removeTab(index);
}

void
MainWindow::removeCurTab()
{
    int index = ui->mainTabWidget->currentIndex();
    if (index == 0)
        return;

    mOpenTabList.removeAt(index);
    QWidget *widget = ui->mainTabWidget->widget(index);
    ui->mainTabWidget->removeTab(index);
    widget->close();
}

void
MainWindow::userLoginSlot(QString curUserName)
{
    // do something according to current user
    ALOGD("%s enter.", __FUNCTION__);
}

void
MainWindow::on_mainTabWidget_currentChanged(int index)
{
    ALOGD("%s, cur tab index = %d", __FUNCTION__, index);
    if (index == 0) {
        // 主页
    } else {
        // 其他页
    }
}
