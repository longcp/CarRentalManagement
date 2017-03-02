#include "insurancedialog.h"
#include "ui_insurancedialog.h"

InsuranceDialog::InsuranceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsuranceDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("购买保险");
    this->setFixedSize(400, 300);
}

InsuranceDialog::~InsuranceDialog()
{
    delete ui;
}

void
InsuranceDialog::openWindow()
{
    this->exec();
}

void InsuranceDialog::on_ensureBtn_clicked()
{
    this->close();
}

void InsuranceDialog::on_cancelBtn_clicked()
{
    this->close();
}
