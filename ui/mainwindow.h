#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ClientManagermentWidget;
class CarManagermentWidget;
class RentalDocumentWidget;
class ReceivableWidget;
class ReceiptWidget;
class ContractWidget;
class QWidget;
class LoginDialog;
class UserManagerDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /**
     * @brief:退出系统
     */
    void        exitSystem();
    /**
     * @brief 窗口关闭事件
     * @param event
     */
    void        closeEvent(QCloseEvent *event);
    /**
     * @brief 打开客户管理窗口
     */
    void        openClientWidget();
    /**
     * @brief 打开车辆管理窗口
     */
    void        openCarWidget();
    /**
     * @brief 打开泵送确认单窗口
     */
    void        openRentalDocWidget();
    /**
     * @brief 打开应收账款窗口
     */
    void        openReceivableWidget();
    /**
     * @brief 打开合同收款窗口
     */
    void        openReceiptWidget();
    /**
     * @brief 打开租赁合同窗口
     */
    void        openContractWidget();
    /**
     * @brief 关闭指定tab
     */
    void        removeTab(int);
    /**
     * @brief 关闭当前页
     */
    void        removeCurTab();
    /**
     * @brief 用户登录
     */
    void        userLoginSlot(QString curUserName);
    void        on_mainTabWidget_currentChanged(int index);

signals:

private:
    Ui::MainWindow *ui;

    // 菜单栏
    QMenu       *mMenuSystemSetting;
    QMenu       *mMenuRentalManagement;
    QMenu       *mMenuCarManagement;
    QMenu       *mMenuFinanceManagement;
    QMenu       *mMenuStatisticalReport;
    QMenu       *mMenuAbout;
    QMenu       *mMenuUploadPicture;
    QMenu       *mMenuUserManagerment;

    // action
    QAction     *mActExitSystem;
    QAction     *mActClient;
    QAction     *mActContract;
    QAction     *mActPump;
    QAction     *mActCarfile;
    QAction     *mActCarMaintenance;
    QAction     *mActCarIllegal;
    QAction     *mActReceivable;
    QAction     *mActReceipt;
    QAction     *mActRevenueAndExpenditure;
    QAction     *mActUserManager;

    // widgets
    ClientManagermentWidget *mClientWidget;
    CarManagermentWidget    *mCarWidget;
    RentalDocumentWidget    *mRentalDocWidget;
    ReceivableWidget        *mReceivableWidget;
    ReceiptWidget           *mReceiptWidget;
    ContractWidget          *mContractWidget;

    // dialog
    LoginDialog             *mLoginDialog;
    UserManagerDialog       *mUserManagerDialog;

    // 用于管理各个tabwidget的list
    QList<QWidget *>        mOpenTabList;
};

#endif // MAINWINDOW_H
