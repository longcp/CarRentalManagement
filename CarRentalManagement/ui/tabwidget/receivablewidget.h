#ifndef RECEIVABLEWIDGET_H
#define RECEIVABLEWIDGET_H

#include <QWidget>

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
        float totalAmount;
        float receivedAmount;
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

signals:
    void            openCarDialogSig();
    void            openClientDialogSig();
    void            openContractDialogSig();
    void            openRentalDocDialogSig();

private:
    void            configToolBar();
    void            initView();
    void            initChooseWidget();
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

    void            setDetailPumpSquareCellValue(float value);
    void            setDetailPumpTimeCellValue(float value);
    void            setDetailProjectAmountCellValue(float value);
    void            setDetailReceiptCellValue(float value);
    void            setDetailReceivableValue(float value);

    void            setSumPumpSquareCellValue(float value);
    void            setSumPumpTimeCellValue(float value);
    void            setSumProjectAmountCellValue(float value);
    void            setSumReceiptCellValue(float value);
    void            setSumReceivableValue(float value);

    void            detailPumpSquareCellAddValue(float value);
    void            detailPumpTimeCellAddValue(float value);
    void            detailProjectAmountCellAddValue(float value);
    void            detailReceiptCellAddValue(float value);
    void            detailReceivableCellAddValue(float value);

    void            sumPumpSquareCellAddValue(float value);
    void            sumPumpTimeCellAddValue(float value);
    void            sumProjectAmountCellAddValue(float value);
    void            sumReceiptCellAddValue(float value);
    void            sumReceivableCellAddValue(float value);

    void            resetDetailSumTableData();
    void            resetTotalSumTableData();

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
