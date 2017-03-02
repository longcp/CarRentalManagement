#include "carannualdialog.h"
#include "ui_carannualdialog.h"

CarAnnualDialog::CarAnnualDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CarAnnualDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("车辆年审");
    this->setFixedSize(400, 300);
}

CarAnnualDialog::~CarAnnualDialog()
{
    delete ui;
}

void
CarAnnualDialog::openWindow()
{
    this->exec();
}

void CarAnnualDialog::on_ensureBtn_clicked()
{
    this->close();
}

void CarAnnualDialog::on_cancelBtn_clicked()
{
    this->close();
}
