#ifndef CLIENTTABLEDIALOG_H
#define CLIENTTABLEDIALOG_H

#include <QDialog>

class TableModel;
class DataBase;
class Client;

namespace Ui {
class ClientTableDialog;
}

class ClientTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientTableDialog(QWidget *parent = 0);
    ~ClientTableDialog();

private slots:
    void            openWindow();

    void on_clientTableView_doubleClicked(const QModelIndex &index);

signals:
    void            selectedClient(QString number);
    void            selectedClientName(QString Name);

private:
    /**
     * @brief 初始化界面
     */
    void            initView();
    /**
     * @brief 初始化表，插入已有数据
     */
    void            initTableView();
    /**
     * @brief 添加条目
     */
    void            addClientItem(Client &client);

    Ui::ClientTableDialog *ui;
    TableModel*     mModel;
    DataBase*       mDb;
};

#endif // CLIENTTABLEDIALOG_H
