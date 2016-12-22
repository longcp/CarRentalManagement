#ifndef CLIENTEDITDIALOG_H
#define CLIENTEDITDIALOG_H

#include <QDialog>

class QToolBar;

namespace Ui {
class ClientEditDialog;
}

class ClientEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientEditDialog(QWidget *parent = 0);
    ~ClientEditDialog();

private slots:
    /**
     * @brief 打开窗口
     */
    void            openClientEditDialogSlot();

private:
    Ui::ClientEditDialog *ui;
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

#endif // CLIENTEDITDIALOG_H
