#ifndef CAREDITDIALOG_H
#define CAREDITDIALOG_H

#include <QDialog>
#include <datatype.h>

class QToolBar;
class Car;
class TableModel;
class DataBase;
class RentalDocument;
class QMenu;
class QAction;
class User;

namespace Ui {
class CarEditDialog;
}

class CarEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarEditDialog(QWidget *parent = 0);
    ~CarEditDialog();

private slots:
    /**
     * @brief 打开窗口
     */
    void            openCarEditDialogSlot(OpenType type,
                                          Car &car);
    void            updateProjectSumSectionWidth(int logicalIndex,
                                                int /* oldSize */,
                                                int newSize);
    void            updateAnnualSumSectionWidth(int logicalIndex,
                                                int /* oldSize */,
                                                int newSize);
    void            updateBusinessSumSectionWidth(int logicalIndex,
                                                int /* oldSize */,
                                                int newSize);
    void            updatePaymentSumSectionWidth(int logicalIndex,
                                                int /* oldSize */,
                                                int newSize);

    void            updateAnnualTableviewScrollBar(int to);
    /**
     * @brief 关闭窗口
     */
    void            closeDialog();
    /**
     * @brief 保存退出
     */
    void            saveAndExitEvent();
    /**
     * @brief 保存
     */
    void            saveEvent();
    /**
     * @brief 窗口关闭事件
     */
    void            closeEvent(QCloseEvent *event);
    /**
     * @brief 编辑事件
     */
    void            editEvent();
    /**
     * @brief 取消事件
     */
    void            cancelEvent();
    /**
     * @brief initViewWithUser
     */
    void            initViewWithUser(User &user);
    /**
     * @brief 添加年费条目
     */
    void            addAnnualItemSlot(ANNUALFEE_RECORD &record);
    void            addInsuranceItemSlot(INSURANCE_RECORD &record);
    void            addBusinessInsuranceItemSlot(INSURANCE_RECORD &record);
    void            addProjectItemSlot(PROJECT_RECORD &record);

    void            delAnnualTableItem();
    void            delBusinessTableItem();
    void            delPaymentTableItem();

    void            on_annualTableview_clicked(const QModelIndex &index);
    void            on_businessTableView_clicked(const QModelIndex &index);
    void            on_paymentTableView_clicked(const QModelIndex &index);
    void            on_annualTableview_customContextMenuRequested(const QPoint &pos);
    void            on_businessTableView_customContextMenuRequested(const QPoint &pos);
    void            on_paymentTableView_customContextMenuRequested(const QPoint &pos);

signals:
    void            addCarItemSignal(Car &car);
    void            updateCarItemSignal(Car &car);

private:
    void            initView();
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    /**
     * @brief initProjectTableView
     */
    void            initProjectTableview();
    /**
     * @brief initAnnualTableview
     */
    void            initAnnualTableview();
    /**
     * @brief initPaymentTableview
     */
    void            initPaymentTableview();
    /**
     * @brief initBusinessTableview
     */
    void            initBusinessTableview();
    /**
     * @brief initAnnualSumTableview
     */
    void            initAnnualSumTableview();
    /**
     * @brief initBusinessSumTableview
     */
    void            initBusinessSumTableview();
    /**
     * @brief initPaymentSumTableview
     */
    void            initPaymentSumTableview();
    /**
     * @brief initProjectSumTableview
     */
    void            initProjectSumTableview();

    /* 清除各合计表数据 */
    void            clearAnnualSumTabData();
    void            clearBusSumTabData();
    void            clearPaySumTabData();
    void            clearProSumTabData();

    /* 设置各合计表格数值 */
    void            setAnnSumAnnFeeCellVal(double value);
    void            setAnnSumTraFeeCellVal(double value);
    void            setBusSumFeeCellVal(double value);
    void            setPaySumFeeCellVal(double value);
    void            setProSumAmountCellVal(double value);

    /* 各合计表格添加数值 */
    void            annSumAnnFeeCellAddVal(double value);
    void            annSumTraFeeCellAddVal(double value);
    void            busSumFeeCellAddVal(double value);
    void            paySumFeeCellAddVal(double value);
    void            proSumAmountCellAddVal(double value);

    /* 各合计表格减去数值 */
    void            annSumAnnFeeCellDelVal(double value);
    void            annSumTraFeeCellDelVal(double value);
    void            busSumFeeCellDelVal(double value);
    void            paySumFeeCellDelVal(double value);
    void            proSumAmountCellDelVal(double value);

    /* 更新各合计表格的条目数 */
    void            annSumUpdateRowCount();
    void            busSumUpdateRowCount();
    void            paySumUpdateRowCount();
    void            proSumUpdateRowCount();
    /**
     * @brief 设置编辑模式
     */
    void            setEditMode();
    /**
     * @brief 设置查看模式
     */
    void            setViewMode();
    /**
     * @brief 重置界面
     */
    void            resetView(Car &car);
    /**
     * @brief 重置界面
     */
    void            resetView();
    /**
     * @brief 设置模式 编辑/查看
     */
    void            setMode(bool mode);

    void            setPumpTypeView();
    /**
     * @brief setOriginCar
     */
    void            setOriginCar(Car &car);
    /**
     * @brief setView
     */
    void            setView(Car &car);
    /**
     * @brief getPumpTypePosition
     */
    int             getPumpTypePosition(PumpType type);
    /**
     * @brief isModified
     */
    bool            isModified();
    /**
     * @brief 清理资源
     */
    void            clean();
    /**
     * @brief 清理内容
     */
    void            cleanContent();
    /**
     * @brief 保存界面数据到client
     */
    void            saveUiContent(Car &car);
    /**
     * @brief 打印car信息
     */
    void            dumpCar(Car &car);
    /**
     * @brief 更新所有表数据
     */
    void            updateAllTableView();
    void            updateAnnualTableView();
    void            updateBusinessInsuanceTableView();
    void            updateInsuranceTableView();
    void            updateProjectTableView();

    PROJECT_RECORD  getProjectFromRentaldoc(const RentalDocument &doc);

    Ui::CarEditDialog *ui;
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

    TableModel*     mProjectModel;
    TableModel*     mProjectSumModel;
    TableModel*     mAnnualModel;
    TableModel*     mAnnualSumModel;
    TableModel*     mBusinessModel;
    TableModel*     mBusinessSumModel;
    TableModel*     mPaymentModel;
    TableModel*     mPaymentSumModel;

    double          mCurAnnSumAnnFee;
    double          mCurAnnSumTraFee;
    double          mCurBusSumFee;
    double          mCurPaySumFee;
    double          mCurProSumAmount;

    OpenType        mOpenType;
    Car*            mOriginCar;
    QString         mCarNumber;
    DataBase*       mDb;
    QMenu*          mAnnualMenu;
    QMenu*          mBusinessMenu;
    QMenu*          mPaymentMenu;
    QAction*        mAnnualDelAct;
    QAction*        mBusinessDelAct;
    QAction*        mPaymentDelAct;
    int             mCurRow;
    bool            mIsModifing;

    bool            mCurUserIsRoot;

    enum AnnualRecordTableCol {
        ANN_REC_COL_NUM,
        ANN_REC_COL_DATE,
        ANN_REC_COL_ANNFEE,
        ANN_REC_COL_TRAVELFEE,
        ANN_REC_COL_REMARKS
    };

    enum InsuranceRecordTableCol {
        INSURANCE_REC_COL_NUM,
        INSURANCE_REC_COL_DATE,
        INSURANCE_REC_COL_FEE,
        INSURANCE_REC_COL_COMPANY,
        INSURANCE_REC_COL_REMARKS
    };

    enum ProjectRecordTableCol {
        PRO_REC_COL_NUM,
        PRO_REC_COL_DATE,
        PRO_REC_COL_CONTRACT_NUM,
        PRO_REC_COL_CLIENT_NUM,
        PRO_REC_COL_CLIENT_NAME,
        PRO_REC_COL_AMOUNT,
        PRO_REC_COL_REMARKS,
        PRO_REC_COL_DOC_NUM
    };
};

#endif // CAREDITDIALOG_H
