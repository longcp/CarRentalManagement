#ifndef CARMANAGERMENT_H
#define CARMANAGERMENT_H

#include <QWidget>
#include <datatype.h>

class QToolBar;
class QTableWidget;
class CarEditDialog;
class InsuranceDialog;
class CarAnnualDialog;
class TableModel;
class Car;
class DataBase;

namespace Ui {
class CarManagermentWidget;
}

class CarManagermentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarManagermentWidget(QWidget *parent = 0);
    ~CarManagermentWidget();

private slots:
    void            cellDoubleClickedSlot(const QModelIndex &index);
    /**
     * @brief 工具栏，添加按钮
     */
    void            addCarSlot();
    /**
     * @brief 添加条目到表格中
     */
    void            addCarItemSlot(Car &car);
    /**
     * @brief 更新表格条目
     */
    void            updateCarItemSlot(Car &car);
    /**
     * @brief 单击表格
     */
    void            on_carTableView_clicked(const QModelIndex &index);
    /**
     * @brief 编辑条目
     */
    void            editCarItemSlot();
    /**
     * @brief 删除条目
     */
    void            deleteCarItemSlot();

signals:
    void            openCarEditDialogSignal(OpenType type,
                                            Car &car);
    void            addAnnualItemSig(ANNUALFEE_RECORD &);

private:
    /**
     * @brief 配置toolbar
     */
    void            configToolBar();
    /**
     * @brief 初始化界面
     */
    void            initView();
    /**
     * @brief 编辑条目
     */
    void            editRowEvent(int row);
    /**
     * @brief 初始化数据到表中
     */
    void            initTableView();

    void            initAnnualTableView();
    void            initInsuranceTableView();
    void            initBusinessInsuanceTableView();
    void            initProjectTableView();

    Ui::CarManagermentWidget *ui;
    QToolBar        *mToolBar;

    // 编辑车辆档案窗口
    CarEditDialog    *mCarEditDialog;
    InsuranceDialog  *mInsuranceDialog;
    CarAnnualDialog  *mCarAnnualDialog;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActInsurance;
    QAction         *mActAnnual;

    TableModel*     mModel;
    DataBase*       mDb;

    int             curRow;
};

#endif // CARMANAGERMENT_H
