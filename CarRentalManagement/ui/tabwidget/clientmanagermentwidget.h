#ifndef CLIENTMANAGERMENTWIDGET_H
#define CLIENTMANAGERMENTWIDGET_H

#include <QWidget>
#include <datatype.h>

class QToolBar;
class QTableWidget;
class ClientEditDialog;
class Client;
class DataBase;
class TableModel;
class User;

namespace Ui {
class ClientManagermentWidget;
}

class ClientManagermentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientManagermentWidget(QWidget *parent = 0);
    ~ClientManagermentWidget();

private slots:
    void            cellDoubleClickedSlot(const QModelIndex &index);
    void            cellClickedSlot(const QModelIndex &index);
    void            addClientSlot();
    void            addClientItemSlot(Client &client);
    void            updateClientItemSlot(Client &client);
    void            deleteClientItemSlot();
    void            editClientItemSlot();

    void            on_cashRadioButton_toggled(bool checked);
    void            on_monthlyRadioButton_toggled(bool checked);
    void            on_paytypeTotalRadioButton_toggled(bool checked);
    void            on_contractRadioButton_toggled(bool checked);
    void            on_temporaryRadioButton_toggled(bool checked);
    void            on_clientTypeTotalRadioButton_toggled(bool checked);

    void            updateSumTabSectionWidth(int logicalIndex, int /*oldSize*/, int newSize);
    void            initViewWithUser(User &user);

signals:
    void            openClientEditDialogSignal(OpenType type, Client &client);
    void            initViewWithUserSig(User &user);

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    /**
     * @brief 初始化界面
     */
    void            initView();
    /**
     * @brief initClientTableView
     */
    void            initClientTableView();
    /**
     * @brief intSumTableView
     */
    void            initSumTableView();
    /**
     * @brief 初始化表，插入已有数据
     */
    void            initTableViewData();
    /**
     * @brief removeAllRows
     */
    void            removeAllRows();
    /**
     * @brief 更新表格
     */
    void            updateTableView();
    /**
     * @brief ui
     */
    void            editRowEvent(int row);

    void            disableFilterView();
    void            enableFilterView();
    void            setFilterViewState(bool state);
    void            clearSumTableData();

    void            setSumAmountCellValue(double value);
    void            setSumPaidCellValue(double value);
    void            setSumBalanceCellValue(double value);

    void            sumAmountCellAddValue(double value);
    void            sumPaidCellAddValue(double value);
    void            sumBalanceCellAddValue(double value);

    void            sumAmountCellDelValue(double value);
    void            sumPaidCellDelValue(double value);
    void            sumBalanceCellDelValue(double value);


    Ui::ClientManagermentWidget *ui;
    QToolBar        *mToolBar;

    // 编辑客户资料窗口
    ClientEditDialog    *mClientEditDialog;

    enum PayTypeFilter {CASH, MONTHLY, PAYTYPE_TOTAL};
    enum ClientTypeFilter {CONTRACT, TEMPORARY, CLIENTTYPE_TOTAL};

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;
    DataBase        *mDb;
    TableModel      *mModel;
    TableModel      *mSumModel;
    int             curRow;
    int             lastShowRow;
    double          mCurSumAmountValue;
    double          mCurSumPaidValue;
    double          mCurSumBalanceValue;
    PayTypeFilter   mCurPayTypeFilter;
    ClientTypeFilter mCurClientTypeFilter;

    enum ClientTableCol {
        CLIENT_COL_NUM,
        CLIENT_COL_TYPE,
        CLIENT_COL_NAME,
        CLIENT_COL_ADDRESS,
        CLIENT_COL_TEL,
        CLIENT_COL_FAX,
        CLIENT_COL_CONTRACT,
        CLIENT_COL_PAYTYPE,
        CLIENT_COL_MONTHLY,
        CLIENT_COL_AMOUNT,
        CLIENT_COL_PAID,
        CLIENT_COL_BALANCE,
        CLIENT_COL_REMARKS
    };
};

#endif // CLIENTMANAGERMENTWIDGET_H
