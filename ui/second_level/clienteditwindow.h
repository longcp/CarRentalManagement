#ifndef CLIENTEDITWINDOW_H
#define CLIENTEDITWINDOW_H

#include <QMainWindow>

class QToolBar;

namespace Ui {
class ClientEditWindow;
}

class ClientEditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientEditWindow(QWidget *parent = 0);
    ~ClientEditWindow();

private slots:
    /**
     * @brief 打开窗口
     */
    void            openClientEditWindowSlot();

private:
    Ui::ClientEditWindow *ui;
    // 工具栏
    QToolBar        *mToolBar;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActSaveExit;
    QAction         *mActSave;
    QAction         *mActExit;
    QAction         *mActEdit;
    QAction         *mActPrev;
    QAction         *mActNext;
    QAction         *mActCancel;
};

#endif // CLIENTEDITWINDOW_H
