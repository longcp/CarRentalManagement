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
    void            deleteClientItemSlog();
    void            editClientItemSlot();

    void on_cashRadioButton_toggled(bool checked);

    void on_monthlyRadioButton_toggled(bool checked);

    void on_paytypeTotalRadioButton_toggled(bool checked);

    void on_contractRadioButton_toggled(bool checked);

    void on_temporaryRadioButton_toggled(bool checked);

    void on_clientTypeTotalRadioButton_toggled(bool checked);

signals:
    void            openClientEditDialogSignal(OpenType type,
                                               Client &client);

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
     * @brief 初始化表，插入已有数据
     */
    void            initTableView();
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
    int             curRow;
    int             lastShowRow;
    PayTypeFilter   mCurPayTypeFilter;
    ClientTypeFilter mCurClientTypeFilter;

    const static int      mColumnCount = 9;
};

#endif // CLIENTMANAGERMENTWIDGET_H
