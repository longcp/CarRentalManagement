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

signals:
    /**
     * @brief 打开收入编辑窗口
     */
    void            openReceiptEditDialogSignal();

private:
    void            configToolBar();
    void            initView();
    void            initReceiptTable();
    void            initReceiptSumTable();
    void            reflashView(QList<RentalDocument> docs);   
    void            clearTableview();
    void            addTableRows(QList<RentalDocument> &docs);
    void            addTableRow(RentalDocument &doc);
    void            addRowAndUpdateSumTable(RentalDocument &doc);
    void            resetReceiptSumTableData();
    void            pumpSquareCellAddValue(float value);
    void            pumpTimeCellAddValue(float value);
    void            projectAmountCellAddValue(float value);
    void            receiptCellAddValue(float value);
    void            receivableCellAddValue(float value);
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
        RECEIPTTABLE_COLUMN_DATE,
        RECEIPTTABLE_COLUMN_DOC_NUMBER,
        RECEIPTTABLE_COLUMN_CONTRACT_NUMBER,
        RECEIPTTABLE_COLUMN_CLIENT_NAME,
        RECEIPTTABLE_COLUMN_CAR_PLATE_NUMBER,
        RECEIPTTABLE_COLUMN_PUMPTYPE,
        RECEIPTTABLE_COLUMN_SQUARE_UNIT_PRICE,
        RECEIPTTABLE_COLUMN_PUMPTIME_UNIT_PRICE,
        RECEIPTTABLE_COLUMN_PUMP_SQUARE,
        RECEIPTTABLE_COLUMN_PUMP_TIME,
        RECEIPTTABLE_COLUMN_PROJECT_AMOUNT,
        RECEIPTTABLE_COLUMN_RECEIPT,
        RECEIPTTABLE_COLUMN_RECEIVABLE,
        RECEIPTTABLE_COLUMN_PROJECT_NAME,
        RECEIPTTABLE_COLUMN_PROJECT_ADDRESS,
        RECEIPTTABLE_COLUMN_CONSTRUCT_PLACE,
        RECEIPTTABLE_COLUMN_PRINCIPLE,
        RECEIPTTABLE_COLUMN_PRINCIPLE_TEL,
        RECEIPTTABLE_COLUMN_REMARKS
    };
};

#endif // RECEIPTWIDGET_H
