#ifndef CONTRACTEDITDIALOG_H
#define CONTRACTEDITDIALOG_H

#include <QDialog>
#include <datatype.h>

class TableModel;
class QToolBar;
class DataBase;
class ContractPriceDialog;

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

    void            on_addBtn_clicked();

    void            on_deleteBtn_clicked();

signals:
    void            openContractPriceWindowSignal(QString);

private:
    /**
     * @brief 初始化界面
     */
    void            initView();
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    /**
     * @brief 初始化价格表
     */
    void            initPriceTableView();

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
    TableModel*     mModel;
    ContractPriceDialog*    mContractPriceDialog;
};

#endif // CONTRACTEDITDIALOG_H
