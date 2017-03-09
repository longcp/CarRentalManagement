#ifndef CONTRACTWIDGET_H
#define CONTRACTWIDGET_H

#include <QWidget>

class QToolBar;
class TableModel;
class ContractEditDialog;

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
    void            openContractEditDialogSignal();

private slots:
    /**
     * @brief 工具栏，添加按钮
     */
    void            addContractSlot();

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
};

#endif // CONTRACTWIDGET_H
