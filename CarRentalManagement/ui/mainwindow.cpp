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
#include <versiondialog.h>
#include <datatype.h>
//#include <Tlhelp32.h>

#define LOG_TAG                         "MAIN_WINDOW"
#include "utils/Log.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mLoginDialog(new LoginDialog()),
    mUserManagerDialog(new UserManagerDialog()),
    mCarWidget(new CarManagermentWidget()),
    mClientWidget(new ClientManagermentWidget()),
    mReceiptWidget(new ReceiptWidget()),
    mContractWidget(new ContractWidget()),
    mRentalDocWidget(new RentalDocumentWidget()),
    mReceivableWidget(new ReceivableWidget()),
    mVersionDialog(new VersionDialog()),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainTabWidget->setTabIcon(0, QIcon(":/menu/icon/home_64.ico"));

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

//    mLoginDialog->exec();

    // 插入首页，首页一直存在
    mOpenTabList.insert(0, ui->homePageTab);

    // 菜单栏
    mMenuSystemSetting  = ui->menuBar->addMenu(tr(MENU_TEXT_SYSTEM_SETTING));
    mMenuRentalManagement   = ui->menuBar->addMenu(tr(MENU_TEXT_LEASING_MANAGERMENT));
    mMenuCarManagement  = ui->menuBar->addMenu(tr(MENU_TEXT_CAR_MANAGERMENT));
    mMenuFinanceManagement  = ui->menuBar->addMenu(tr(MENU_TEXT_FINANCIAL_STATISTICS));
    mMenuUploadPicture  = ui->menuBar->addMenu(tr(MENU_TEXT_UPLOAD_IMAGE));
    mMenuUserManagerment= ui->menuBar->addMenu(tr(MENU_TEXT_USER_MANAGERMENT));
    mMenuAbout = ui->menuBar->addMenu(tr(MENU_TEXT_HELP));
    mActExitSystem = mMenuSystemSetting->addAction(
                QIcon(":/menu/icon/exit.png"),
                tr(ACTION_TEXT_EXIT_SYSTEM));
    mActClient = mMenuRentalManagement->addAction(
                QIcon(":/menu/icon/client.png"),
                tr(ACTION_TEXT_CLIENT_MANAGERMENT));
    mActContract = mMenuRentalManagement->addAction(
                QIcon(":/menu/icon/contract_64.ico"),
                tr(ACTION_TEXT_CONTRACT));
    mActPump = mMenuRentalManagement->addAction(
                QIcon(":/menu/icon/pump_64.ico"),
                tr(ACTION_TEXT_RENTALDOCUMENT));
    mActCarfile = mMenuCarManagement->addAction(
                QIcon(":/menu/icon/Ford_Heavy_Wreck_Truck_128.ico"),
                tr(ACTION_TEXT_CAR_INFORMATION));
    mActReceivable = mMenuFinanceManagement->addAction(
                QIcon(":/menu/icon/custom-reports256.png"),
                tr(ACTION_TEXT_RECEIVABLE));
    mActReceipt = mMenuFinanceManagement->addAction(
                QIcon(":/menu/icon/calculator_64.ico"),
                tr(ACTION_TEXT_CONTRACT_RECEIPT));
    mActUserManager = mMenuUserManagerment->addAction(
                QIcon(":/menu/icon/user_64.ico"),
                tr(ACTION_TEXT_USER_MANAGERMENT));
    mActAbout = mMenuAbout->addAction(
                QIcon(":/menu/icon/information_64.ico"),
                tr(ACTION_TEXT_ABOUT));

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
     * @brief 打开关于窗口
     */
    connect(mActAbout, SIGNAL(triggered()),
            mVersionDialog, SLOT(openWindow()));
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
    /**
     * @brief 当前页签切换至应收账款页
     */
    connect(this, SIGNAL(tabChangeToReceivableSignal(int, QString)),
            mReceivableWidget, SLOT(tabChangeToReceivableSlot(int,QString)));
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
                              tr(TAB_TITLE_CLIENTMANAGERMENT));
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
                              tr(TAB_TITLE_CAR_INFOMATION));
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
                              tr(TAB_TITLE_RENTALDOCUMENT));
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
                              tr(TAB_TITLE_RECEIVABLE));
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
                              tr(TAB_TITLE_RECEIPT));
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
                              tr(TAB_TITLE_CONTRACT));
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
    ALOGD("%s, cur tab index = %d, text = %s", __FUNCTION__,
          index, ui->mainTabWidget->tabText(index).toStdString().data());

    if (ui->mainTabWidget->tabText(index) == ACTION_TEXT_RECEIVABLE) {
        emit tabChangeToReceivableSignal(index, ui->mainTabWidget->tabText(index));
    }
}

void MainWindow::on_mainTabWidget_tabBarClicked(int index)
{
    ALOGDTRACE();
    ALOGD("current index = %d, text = %s",
          index, ui->mainTabWidget->tabText(index).toStdString().data());
}
