#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ClientManagermentWidget;
class QWidget;

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

    // widgets
    ClientManagermentWidget *mClientWidget;

    // 用于管理各个tabwidget的list
    QList<QWidget *>        mOpenTabList;
};

#endif // MAINWINDOW_H
