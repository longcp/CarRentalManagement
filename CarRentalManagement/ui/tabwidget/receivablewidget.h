#ifndef RECEIVABLEWIDGET_H
#define RECEIVABLEWIDGET_H

#include <QWidget>
#include <datatype.h>

class QToolBar;
class TableModel;
class RentalDocument;
class DataBase;
class CarTableDialog;
class ContractTableDialog;
class ClientTableDialog;
class RentalDocTableDialog;

namespace Ui {
class ReceivableWidget;
}

class ReceivableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReceivableWidget(QWidget *parent = 0);
    ~ReceivableWidget();

    class Receivable {
        RentalDocument* rentalDoc;
        double totalAmount;
        double receivedAmount;
    };

private slots:
    void            updateDetailSumTableviewSectionWidth(int logicalIndex, int,
                                                         int newSize);
    void            updateTotalSumTableviewSectionWidth(int logicalIndex, int,
                                                        int newSize);
    void            tabChangeToReceivableSlot(int index, QString tabText);

    void            deleteRentalDocumentSlot(QString docNum);
    void            addRentalDocumentSlot(RentalDocument &doc);
    void            getContract(QString number);
    void            getClientName(QString name);
    void            getCar(QString number);
    void            getDoc(QString number);

    void            on_screeningBtn_clicked();
    void            on_clearBtn_clicked();
    void            on_fromDateCb_toggled(bool checked);
    void            on_toDateCb_toggled(bool checked);
    void            on_docNumTb_clicked();
    void            on_contractNumTb_clicked();
    void            on_clientNameTb_clicked();
    void            on_carNumTb_clicked();

    void            onDetailTabHeaderClicked(int column);
    void            onTotalTabHeaderClicked(int column);

signals:
    void            openCarDialogSig();
    void            openClientDialogSig();
    void            openContractDialogSig();
    void            openRentalDocDialogSig();

private:
    void            configToolBar();
    void            initView();
    void            initChooseWidget();
    void            setPumpTypeView();
    void            initDetailTableview();
    void            initDetailSumTableview();
    void            initTotalTableview();
    void            initTotalSumTableview();

    void            clearDetailTableview();
    void            clearSumTableview();

    void            reflashDetailTableview(QList<RentalDocument> &docs);
    void            reflashSumTableview(QList<RentalDocument> &docs);

    void            addDetailTableRows(QList<RentalDocument> &docs);
    void            addDetailTableRow(RentalDocument &doc);
    void            addSumTableRows(QList<RentalDocument> &docs);
    void            addSumTableRow(RentalDocument &doc);
    void            addRowAndUpdateDetailSumTable(RentalDocument &doc);

    void            setDetailPumpSquareCellValue(double value);
    void            setDetailPumpTimeCellValue(double value);
    void            setDetailProjectAmountCellValue(double value);
    void            setDetailReceiptCellValue(double value);
    void            setDetailReceivableValue(double value);

    void            setSumPumpSquareCellValue(double value);
    void            setSumPumpTimeCellValue(double value);
    void            setSumProjectAmountCellValue(double value);
    void            setSumReceiptCellValue(double value);
    void            setSumReceivableValue(double value);

    void            detailPumpSquareCellAddValue(double value);
    void            detailPumpTimeCellAddValue(double value);
    void            detailProjectAmountCellAddValue(double value);
    void            detailReceiptCellAddValue(double value);
    void            detailReceivableCellAddValue(double value);

    void            sumPumpSquareCellAddValue(double value);
    void            sumPumpTimeCellAddValue(double value);
    void            sumProjectAmountCellAddValue(double value);
    void            sumReceiptCellAddValue(double value);
    void            sumReceivableCellAddValue(double value);

    void            resetDetailSumTableData();
    void            resetTotalSumTableData();

    RECEIPT_FILTER  getFilter();

    Ui::ReceivableWidget *ui;
    QToolBar        *mToolBar;
    DataBase*       mDb;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;

    TableModel*     mDetailModel;
    TableModel*     mDetailSumModel;
    TableModel*     mTotalModel;
    TableModel*     mTotalSumModel;

    CarTableDialog*         mCarDialog;
    ContractTableDialog*    mContractDialog;
    ClientTableDialog*      mClientDialog;
    RentalDocTableDialog*   mRentalDocDialog;

    double          curPumpSquareValue;
    double          curPumpTimeValue;
    double          curProjectAmountValue;
    double          curReceiptValue;
    double          curReceivableValue;
    double          curPumpSquareSumValue;
    double          curPumpTimeSumValue;
    double          curProjectAmountSumValue;
    double          curReceiptSumValue;
    double          curReceivableSumValue;

    int             mCurDetailSortCol;
    int             mCurTotalSortCol;
    bool            mIsDetailSortAscending;
    bool            mIsTotalSortAscending;
    bool            mIsScreening;

    enum DetailTableColumn {
        DETAIL_COL_DATE,
        DETAIL_COL_DOC_NUMBER,
        DETAIL_COL_CONTRACT_NUMBER,
        DETAIL_COL_CLIENT_NAME,
        DETAIL_COL_CAR_PLATE_NUMBER,
        DETAIL_COL_PUMPTYPE,
        DETAIL_COL_CONCRETE_LABLE,
        DETAIL_COL_SQUARE_UNIT_PRICE,
        DETAIL_COL_PUMPTIME_UNIT_PRICE,
        DETAIL_COL_PUMP_SQUARE,
        DETAIL_COL_PUMP_TIME,
        DETAIL_COL_PROJECT_AMOUNT,
        DETAIL_COL_RECEIPT,
        DETAIL_COL_RECEIVABLE,
        DETAIL_COL_PROJECT_NAME,
        DETAIL_COL_PROJECT_ADDRESS,
        DETAIL_COL_CONSTRUCT_PLACE,
        DETAIL_COL_PRINCIPLE,
        DETAIL_COL_PRINCIPLE_TEL,
        DETAIL_COL_REMARKS
    };

    enum SumTableColumn {
        SUM_COL_CONTRACT_NUMBER,
        SUM_COL_CLIENT_NAME,
        SUM_COL_PUMP_SQUARE,
        SUM_COL_PUMP_TIME,
        SUM_COL_PROJECT_AMOUNT,
        SUM_COL_RECEIPT,
        SUM_COL_RECEIVABLE,
    };
};

#endif // RECEIVABLEWIDGET_H
