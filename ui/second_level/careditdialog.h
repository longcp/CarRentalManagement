#ifndef CAREDITDIALOG_H
#define CAREDITDIALOG_H

#include <QDialog>

class QToolBar;

namespace Ui {
class CarEditDialog;
}

class CarEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarEditDialog(QWidget *parent = 0);
    ~CarEditDialog();

private slots:
    /**
     * @brief 打开窗口
     */
    void            openCarEditDialogSlot();

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    Ui::CarEditDialog *ui;
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

#endif // CAREDITDIALOG_H
