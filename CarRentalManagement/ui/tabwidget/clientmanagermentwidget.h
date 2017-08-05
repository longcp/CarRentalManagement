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
     * @brief ui
     */
    void            editRowEvent(int row);

    Ui::ClientManagermentWidget *ui;
    QToolBar        *mToolBar;

    // 编辑客户资料窗口
    ClientEditDialog    *mClientEditDialog;

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

    const static int      mColumnCount = 9;
};

#endif // CLIENTMANAGERMENTWIDGET_H
