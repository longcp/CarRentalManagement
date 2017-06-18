#ifndef RECEIVABLEWIDGET_H
#define RECEIVABLEWIDGET_H

#include <QWidget>

class QToolBar;
class TableModel;
class RentalDocument;
class DataBase;

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

private:
    void            configToolBar();
    void            initView();
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

    enum DetailTableColumn {
        DETAILTABLE_COLUMN_DATE,
        DETAILTABLE_COLUMN_DOC_NUMBER,
        DETAILTABLE_COLUMN_CONTRACT_NUMBER,
        DETAILTABLE_COLUMN_CLIENT_NAME,
        DETAILTABLE_COLUMN_CAR_PLATE_NUMBER,
        DETAILTABLE_COLUMN_PUMPTYPE,
        DETAILTABLE_COLUMN_CONCRETE_LABLE,
        DETAILTABLE_COLUMN_SQUARE_UNIT_PRICE,
        DETAILTABLE_COLUMN_PUMPTIME_UNIT_PRICE,
        DETAILTABLE_COLUMN_PUMP_SQUARE,
        DETAILTABLE_COLUMN_PUMP_TIME,
        DETAILTABLE_COLUMN_PROJECT_AMOUNT,
        DETAILTABLE_COLUMN_RECEIPT,
        DETAILTABLE_COLUMN_RECEIVABLE,
        DETAILTABLE_COLUMN_PROJECT_NAME,
        DETAILTABLE_COLUMN_PROJECT_ADDRESS,
        DETAILTABLE_COLUMN_CONSTRUCT_PLACE,
        DETAILTABLE_COLUMN_PRINCIPLE,
        DETAILTABLE_COLUMN_PRINCIPLE_TEL,
        DETAILTABLE_COLUMN_REMARKS
    };

    enum SumTableColumn {
        SUMTABLE_COLUMN_CONTRACT_NUMBER,
        SUMTABLE_COLUMN_CLIENT_NAME,
        SUMTABLE_COLUMN_PUMP_SQUARE,
        SUMTABLE_COLUMN_PUMP_TIME,
        SUMTABLE_COLUMN_PROJECT_AMOUNT,
        SUMTABLE_COLUMN_RECEIPT,
        SUMTABLE_COLUMN_RECEIVABLE,
    };
};

#endif // RECEIVABLEWIDGET_H
