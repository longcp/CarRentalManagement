#ifndef RECEIPTWIDGET_H
#define RECEIPTWIDGET_H

#include <QWidget>
#include <datatype.h>

class QToolBar;
class ReceiptEditDialog;
class TableModel;
class DataBase;
class RentalDocument;
class CarTableDialog;
class ContractTableDialog;
class ClientTableDialog;
class RentalDocTableDialog;

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
    /**
     * @brief 打开收入编辑窗口
     */
    void            openReceiptEditDialogSignal();
    void            openCarDialogSig();
    void            openClientDialogSig();
    void            openContractDialogSig();
    void            openRentalDocDialogSig();

private:
    void            configToolBar();
    void            initView();
    void            setPumpTypeView();
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

    void            pumpSquareCellAddValue(double value);
    void            pumpTimeCellAddValue(double value);
    void            projectAmountCellAddValue(double value);
    void            receiptCellAddValue(double value);
    void            receivableCellAddValue(double value);

    void            pumpSquareCellDelValue(double value);
    void            pumpTimeCellDelValue(double value);
    void            projectAmountCellDelValue(double value);
    void            receiptCellDelValue(double value);
    void            receivableCellDelValue(double value);

    void            setPumpSquareCellValue(double value);
    void            setPumpTimeCellValue(double value);
    void            setProjectAmountCellValue(double value);
    void            setReceiptCellValue(double value);
    void            setReceivableValue(double value);

    RECEIPT_FILTER  getFilter();

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

    CarTableDialog*         mCarDialog;
    ContractTableDialog*    mContractDialog;
    ClientTableDialog*      mClientDialog;
    RentalDocTableDialog*   mRentalDocDialog;

    double          curPumpSquareValue;
    double          curPumpTimeValue;
    double          curProjectAmountValue;
    double          curReceiptValue;
    double          curReceivableValue;

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
