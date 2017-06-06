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
    void            setPumpSquareCellValue(int value);
    void            setPumpTimeCellValue(int value);
    void            setProjectAmountCellValue(int value);
    void            setReceiptCellValue(int value);
    void            setReceivableValue(int value);
    void            pumpSquareCellAddValue(int value);
    void            pumpTimeCellAddValue(int value);
    void            projectAmountCellAddValue(int value);
    void            receiptCellAddValue(int value);
    void            receivableCellAddValue(int value);
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
        COLUMN_DATE,
        COLUMN_DOC_NUMBER,
        COLUMN_CONTRACT_NUMBER,
        COLUMN_CLIENT_NAME,
        COLUMN_CAR_PLATE_NUMBER,
        COLUMN_PUMPTYPE,
        COLUMN_CONCRETE_LABLE,
        COLUMN_SQUARE_UNIT_PRICE,
        COLUMN_PUMPTIME_UNIT_PRICE,
        COLUMN_PUMP_SQUARE,
        COLUMN_PUMP_TIME,
        COLUMN_PROJECT_AMOUNT,
        COLUMN_RECEIPT,
        COLUMN_RECEIVABLE,
        COLUMN_PROJECT_NAME,
        COLUMN_PROJECT_ADDRESS,
        COLUMN_CONSTRUCT_PLACE,
        COLUMN_PRINCIPLE,
        COLUMN_PRINCIPLE_TEL,
        COLUMN_REMARKS
    };
};

#endif // RECEIVABLEWIDGET_H
