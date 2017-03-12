#ifndef CONTRACTEDITDIALOG_H
#define CONTRACTEDITDIALOG_H

#include <QDialog>
#include <datatype.h>

class TableModel;
class QToolBar;
class DataBase;
class ContractPriceDialog;
class Contract;

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
     * @brief 添加价格
     */
    void            on_addBtn_clicked();
    /**
     * @brief 删除价格
     */
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
    Contract*       mOriginContract;
    ContractPriceDialog*    mContractPriceDialog;
};

#endif // CONTRACTEDITDIALOG_H
