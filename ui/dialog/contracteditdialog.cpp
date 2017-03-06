#include "contracteditdialog.h"
#include "ui_contracteditdialog.h"

ContractEditDialog::ContractEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContractEditDialog)
{
    ui->setupUi(this);
}

ContractEditDialog::~ContractEditDialog()
{
    delete ui;
}
