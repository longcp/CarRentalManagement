#ifndef CONTRACTEDITDIALOG_H
#define CONTRACTEDITDIALOG_H

#include <QDialog>
#include <datatype.h>

class QToolBar;
class DataBase;

namespace Ui {
class ContractEditDialog;
}

class ContractEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContractEditDialog(QWidget *parent = 0);
    ~ContractEditDialog();

private slots:
    void            openContractEditDialogSlot();

private:
    /**
     * @brief 初始化界面
     */
    void            initView();
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();

    Ui::ContractEditDialog *ui;
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

    DataBase*       mDb;
    OpenType        mOpenType;
};

#endif // CONTRACTEDITDIALOG_H
