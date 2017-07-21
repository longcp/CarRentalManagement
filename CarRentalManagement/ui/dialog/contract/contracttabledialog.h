#ifndef CONTRACTTABLEDIALOG_H
#define CONTRACTTABLEDIALOG_H

#include <QDialog>

class TableModel;
class DataBase;
class Contract;

namespace Ui {
class ContractTableDialog;
}

class ContractTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContractTableDialog(QWidget *parent = 0);
    ~ContractTableDialog();

private slots:
    void            openWindow(QString clientNumber);

    void            on_contractTableView_doubleClicked(const QModelIndex &index);

signals:
    void            selectedContract(QString contractNumber);

private:
    /**
     * @brief 初始化界面
     */
    void            initView();
    /**
     * @brief 初始化表，插入已有数据
     */
    void            initTableView(QString clientNumber);
    /**
     * @brief 添加条目
     */
    void            addContractItem(Contract &contract);

    TableModel*     mModel;
    DataBase*       mDb;

    Ui::ContractTableDialog *ui;
};

#endif // CONTRACTTABLEDIALOG_H
