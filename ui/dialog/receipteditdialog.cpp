#include "receipteditdialog.h"
#include "ui_receipteditdialog.h"

ReceiptEditDialog::ReceiptEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReceiptEditDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("收款");
    this->setFixedSize(800, 600);
}

ReceiptEditDialog::~ReceiptEditDialog()
{
    delete ui;
}

void
ReceiptEditDialog::openReceiptEditDialogSlot()
{
    this->exec();
}
