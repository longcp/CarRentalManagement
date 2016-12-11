#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // 菜单栏
    QMenu       *mSystemSettingMenu;
    QMenu       *mInformationMenu;
    QMenu       *mRentalManagementMenu;
    QMenu       *mFinanceManagementMenu;
    QMenu       *mStatisticalReportMenu;
    QMenu       *mAboutMenu;
};

#endif // MAINWINDOW_H
