#include "contractpricedialog.h"
#include "ui_contractpricedialog.h"

ContractPriceDialog::ContractPriceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContractPriceDialog)
{
    ui->setupUi(this);
}

ContractPriceDialog::~ContractPriceDialog()
{
    delete ui;
}

void
ContractPriceDialog::openWindow(QString contractNumber)
{
    this->exec();
}

void
ContractPriceDialog::on_ensureBtn_clicked()
{

}

void
ContractPriceDialog::on_cancelBtn_clicked()
{

}
