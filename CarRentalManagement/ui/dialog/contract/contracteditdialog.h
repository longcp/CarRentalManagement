#ifndef CONTRACTEDITDIALOG_H
#define CONTRACTEDITDIALOG_H

#include <QDialog>
#include <datatype.h>

class TableModel;
class QToolBar;
class DataBase;
class ContractPriceDialog;
class Contract;
class QTableView;
class ClientTableDialog;

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
    void            openContractEditDialogSlot(OpenType opentype,
                                               Contract &contract);
    /**
     * @brief 工具栏编辑按钮
     */
    void            editEvent();
    /**
     * @brief 工具栏保存退出按钮
     */
    void            saveAndExitEvent();
    /**
     * @brief 工具栏保存按钮
     */
    void            saveEvent();
    /**
     * @brief 工具栏取消按钮
     */
    void            cancelEvent();
    /**
     * @brief 工具栏退出按钮
     */
    void            closeDialog();
    /**
     * @brief 添加价格
     */
    void            on_addBtn_clicked();
    /**
     * @brief 删除价格
     */
    void            on_deleteBtn_clicked();

    void            addPriceItemSlot(CONTRACT_PRICE &price);

    void            on_isIncludeTexCB_stateChanged(int state);

    void            on_priceTableView_clicked(const QModelIndex &index);

    void            getSelectedClient(QString number);

    void on_chooseClientBtn_clicked();

signals:
    void            openContractPriceWindowSignal(OpenType, QString);
    void            addContractItemSignal(Contract&);
    void            updateContractItemSignal(Contract&);
    void            openClientTableWindowSignal();

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
    /**
     * @brief 设置模式 编辑/查看
     */
    void            setMode(bool mode);
    /**
     * @brief 清理资源
     */
    void            clean();
    /**
     * @brief 清理内容
     */
    void            cleanContent();
    /**
     * @brief 清空表内容
     */
    void            clearPriceTable();
    /**
     * @brief
     */
    bool            isModified();
    /**
     * @brief 初始化相关参数
     */
    void            initParam();
    /**
     * @brief 设置界面内容
     */
    void            setView(Contract &contract);
    /**
     * @brief 重置界面相关内容
     */
    void            resetView(Contract &contract);
    /**
     * @brief 重置界面相关内容
     */
    void            resetView();
    /**
     * @brief 设置编辑模式
     */
    void            setEditMode();
    /**
     * @brief 设置查看模式
     */
    void            setViewMode();
    /**
     * @brief 设置初始数据
     */
    void            setOriginContract(Contract &contract);
    /**
     * @brief 窗口关闭事件
     */
    void            closeEvent(QCloseEvent *event);
    /**
     * @brief 保存界面数据
     */
    void            saveUiContent(Contract &contract);

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

    int             mCurRow;
    DataBase*       mDb;
    OpenType        mOpenType;
    TableModel*     mModel;
    Contract*       mOriginContract;
    ClientTableDialog*      mClientTableDialog;
    ContractPriceDialog*    mContractPriceDialog;
};

#endif // CONTRACTEDITDIALOG_H
