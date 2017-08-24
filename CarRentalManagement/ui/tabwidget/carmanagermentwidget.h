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
class User;

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
    void            openCarEditDialogSignal(OpenType type, Car &car);
    void            initViewWithUserSig(User &user);
private slots:
    void            updateSumTabSectionWidth(int logicalIndex, int, int newSize);
    void            initViewWithUser(User &user);

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
     * @brief initCarTableView
     */
    void            initCarTableView();
    /**
     * @brief initSumTableView
     */
    void            initSumTableView();
    /**
     * @brief clearSumTableData
     */
    void            clearSumTableData();
    /**
     * @brief 编辑条目
     */
    void            editRowEvent(int row);
    /**
     * @brief 初始化数据到表中
     */
    void            initCarTableViewData();

    void            setSumPumpedSquareCellValue(double value);
    void            setSumPumpedTimeCellValue(double value);
    void            setSumMilageCellValue(double value);

    void            sumPumpedSquareCellAddValue(double value);
    void            sumPumpedTimeCellAddValue(double value);
    void            sumMilageCellAddValue(double value);

    void            sumPumpedSquareCellDelValue(double value);
    void            sumPumpedTimeCellDelValue(double value);
    void            sumMilageCellDelValue(double value);

    void            sumUpdateRowCount();

    Ui::CarManagermentWidget *ui;
    QToolBar        *mToolBar;

    // 编辑车辆档案窗口
    CarEditDialog    *mCarEditDialog;
    InsuranceDialog  *mInsuranceDialog;
    CarAnnualDialog  *mCarAnnualDialog;

    double          mCurSumPumpedSquareValue;
    double          mCurSumPumpedTimeValue;
    double          mCurSumMilageValue;

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
    TableModel*     mSumModel;
    DataBase*       mDb;

    int             curRow;

    enum CarTableCol {
        CAR_COL_NUM,
        CAR_COL_PUMPTYPE,
        CAR_COL_PUMPEDSQUARE,
        CAR_COL_PUMPEDTIMES,
        CAR_COL_MILAGE,
        CAR_COL_CARBRAND,
        CAR_COL_CHASSISBRAND,
        CAR_COL_DRIVINGLICENSEDATE,
        CAR_COL_FUELCARNUMBER,
        CAR_COL_FRAMENUMBER,
        CAR_COL_IDENTIFICATIONNUMBER,
        CAR_COL_PRODUCTNUMBER,
        CAR_COL_INSURANCECARDNUMBER,
        CAR_COL_ENGINENUMBER,
        CAR_COL_WORTH,
        CAR_COL_ENGINEPOWER,
        CAR_COL_MAXDELIVERYSIZES,
        CAR_COL_MAXOUTPUTPRESSURE,
        CAR_COL_DIMENSIONS,
        CAR_COL_BOOMVERTICALLEN,
        CAR_COL_BOOMHORIZONTALLEN,
        CAR_COL_TOTALWEIGHT,
        CAR_COL_EQUIPMENTWEIGHT,
        CAR_COL_PRODUCTIONDATE,
        CAR_COL_FACTORYCODE,
        CAR_COL_OPERATOR1,
        CAR_COL_OPERATOR2,
        CAR_COL_OPERATOR3,
        CAR_COL_OPERATOR4,
        CAR_COL_OPERATOR5,
        CAR_COL_REMARKS
    };
};

#endif // CARMANAGERMENT_H
