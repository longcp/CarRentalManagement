#ifndef PRICETABLEDIALOG_H
#define PRICETABLEDIALOG_H

#include <QDialog>
#include <datatype.h>

class TableModel;
class DataBase;

namespace Ui {
class PriceTableDialog;
}

class PriceTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PriceTableDialog(QWidget *parent = 0);
    ~PriceTableDialog();

private slots:
    void            openWindow(QString contractNumber);

    void            on_tableView_doubleClicked(const QModelIndex &index);

signals:
    void            selectedPrice(CONTRACT_PRICE price);

private:
    void            initView();
    void            initTableView();
    void            addPriceItem(CONTRACT_PRICE price);

    QString         mContractNumber;
    Ui::PriceTableDialog *ui;
    TableModel*     mModel;
    DataBase*       mDb;
};

#endif // PRICETABLEDIALOG_H
