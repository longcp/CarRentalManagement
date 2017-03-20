#ifndef CONTRACTWIDGET_H
#define CONTRACTWIDGET_H

#include <QWidget>
#include <datatype.h>

class QToolBar;
class TableModel;
class ContractEditDialog;
class Contract;
class DataBase;

namespace Ui {
class ContractWidget;
}

class ContractWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContractWidget(QWidget *parent = 0);
    ~ContractWidget();

signals:
    void            openContractEditDialogSignal(OpenType,
                                                 Contract&);

private slots:
    /**
     * @brief 工具栏，添加按钮
     */
    void            addContractSlot();
    /**
     * @brief 工具栏，编辑按钮
     */
    void            editContractItemSlot();
    /**
     * @brief 添加条目
     */
    void            addContractItemSlot(Contract&);
    /**
     * @brief 更新条目
     */
    void            updateContractItemSLot(Contract&);
    /**
     * @brief 删除按钮
     */
    void            deleteContractItemSlot();

    void on_contractTableView_clicked(const QModelIndex &index);

    void on_contractTableView_doubleClicked(const QModelIndex &index);

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    /**
     * @brief 初始化界面
     */
    void            initView();
    /**
     * @brief 初始化合同表格式
     */
    void            initContractTableview();
    /**
     * @brief 初始化价格表格式
     */
    void            initPriceTableview();
    /**
     * @brief 编辑条目
     */
    void            editRowEvent(int row);
    /**
     * @brief 项目表添加行
     */
    void            addContractTableRow(Contract &contract);
    /**
     * @brief 价格表添加行
     */
    void            addPriceTableRows(QList<CONTRACT_PRICE> prices);
    /**
     * @brief 价格表添加行
     */
    void            addPriceTableRow(CONTRACT_PRICE price);
    /**
     * @brief 更新合同表
     */
    void            updateContractRow(Contract &contract);
    /**
     * @brief 更新价格表
     */
    void            updateContractPriceTable(QList<CONTRACT_PRICE> prices);

    Ui::ContractWidget *ui;
    ContractEditDialog* mContractEditDialog;

    // 工具栏动作
    QToolBar        *mToolBar;
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;

    TableModel*     mContractModel;
    TableModel*     mPriceModel;
    DataBase*       mDb;
    int             curRow;
};

#endif // CONTRACTWIDGET_H
