#ifndef CONTRACTPRICEDIALOG_H
#define CONTRACTPRICEDIALOG_H

#include <QDialog>

namespace Ui {
class ContractPriceDialog;
}

class ContractPriceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContractPriceDialog(QWidget *parent = 0);
    ~ContractPriceDialog();

private slots:
    void            openWindow(QString contractNumber);

    void            on_ensureBtn_clicked();

    void            on_cancelBtn_clicked();

private:
    Ui::ContractPriceDialog *ui;
};

#endif // CONTRACTPRICEDIALOG_H
