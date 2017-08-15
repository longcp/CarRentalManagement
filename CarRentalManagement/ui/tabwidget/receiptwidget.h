#ifndef RECEIPTWIDGET_H
#define RECEIPTWIDGET_H

#include <QWidget>

class QToolBar;
class ReceiptEditDialog;
class TableModel;
class DataBase;
class RentalDocument;

namespace Ui {
class ReceiptWidget;
}

class ReceiptWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptWidget(QWidget *parent = 0);
    ~ReceiptWidget();

private slots:
    /**
     * @brief 双击列表响应
     * @param a
     * @param b
     */
    void            cellDoubleClickedSlot(int a,int b);
    void            updateReceiptSumTableSectionWidth(int logicalIndex,
                                                      int /*oldSize*/,
                                                      int newSize);
    void            tabChangeToReceiptSlot(int index, QString tabText);
    void            sectionClickedSlot(int index);
    void            deleteRentalDocumentSlot(QString docNum);
    void            addRentalDocumentSlot(RentalDocument &doc);

    void on_screeningBtn_clicked();

    void on_clearBtn_clicked();

    void on_fromDateCb_toggled(bool checked);

    void on_toDateCb_toggled(bool checked);

signals:
    /**
     * @brief 打开收入编辑窗口
     */
    void            openReceiptEditDialogSignal();

private:
    void            configToolBar();
    void            initView();
    void            initChooseWidget();
    void            initReceiptTable();
    void            initReceiptSumTable();
    void            reflashView(QList<RentalDocument> docs);   
    void            clearTableview();
    void            addTableRows(QList<RentalDocument> &docs);
    void            addTableRow(RentalDocument &doc);
    void            addRowAndUpdateSumTable(RentalDocument &doc);
    void            delRowAndUpdateSumTable(int row, RentalDocument &doc);
    void            resetReceiptSumTableData();

    void            pumpSquareCellAddValue(float value);
    void            pumpTimeCellAddValue(float value);
    void            projectAmountCellAddValue(float value);
    void            receiptCellAddValue(float value);
    void            receivableCellAddValue(float value);

    void            pumpSquareCellDelValue(float value);
    void            pumpTimeCellDelValue(float value);
    void            projectAmountCellDelValue(float value);
    void            receiptCellDelValue(float value);
    void            receivableCellDelValue(float value);

    void            setPumpSquareCellValue(float value);
    void            setPumpTimeCellValue(float value);
    void            setProjectAmountCellValue(float value);
    void            setReceiptCellValue(float value);
    void            setReceivableValue(float value);

    Ui::ReceiptWidget *ui;
    QToolBar        *mToolBar;

    // 编辑客户资料窗口
    ReceiptEditDialog    *mReceiptEditDialog;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;

    TableModel*     mReceiptModel;
    TableModel*     mReceiptSumModel;

    DataBase*       mDb;

    enum ReceiptTableColumn {
        COL_DATE,
        COL_DOC_NUMBER,
        COL_CONTRACT_NUMBER,
        COL_CLIENT_NAME,
        COL_CAR_PLATE_NUMBER,
        COL_PUMPTYPE,
        COL_SQUARE_UNIT_PRICE,
        COL_PUMPTIME_UNIT_PRICE,
        COL_PUMP_SQUARE,
        COL_PUMP_TIME,
        COL_PROJECT_AMOUNT,
        COL_RECEIPT,
        COL_RECEIVABLE,
        COL_PROJECT_NAME,
        COL_PROJECT_ADDRESS,
        COL_CONSTRUCT_PLACE,
        COL_PRINCIPLE,
        COL_PRINCIPLE_TEL,
        COL_REMARKS
    };
};

#endif // RECEIPTWIDGET_H
