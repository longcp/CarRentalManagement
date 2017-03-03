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

signals:
    void            openCarEditDialogSignal(OpenType type,
                                            Car &car);

private:
    /**
     * @brief 配置toolbar
     */
    void            configToolBar();
    /**
     * @brief 初始化界面
     */
    void            initView();

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
};

#endif // CARMANAGERMENT_H
