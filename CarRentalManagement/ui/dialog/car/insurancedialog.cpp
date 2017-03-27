#include "insurancedialog.h"
#include "ui_insurancedialog.h"
#include <datatype.h>
#include <database/database.h>
#include <QMessageBox>
#include <car.h>
#include <QDateTime>

#define LOG_TAG                 "INSURANCE_DIALOG"
#include "utils/Log.h"

InsuranceDialog::InsuranceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsuranceDialog)
{
    ui->setupUi(this);
    mDb = DataBase::getInstance();
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
    Car car;
    QList<Car> cars;
    if (mDb->getAllCarData(cars)) {
        ALOGD("no cars");
        return;
    }

    if (cars.isEmpty())
        return;

    int size = cars.size();
    for (int i = 0; i < size; i++) {
        car = cars.at(i);
        ui->carNumComboBox->insertItem(i, car.number);
    }

    QString number = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
    ui->numberLabel->setText(number);
    ui->dateDateEdit->setDate(QDate::currentDate());
    ui->insuranceRadioBtn->setChecked(true);
    ui->businessInsuranceRadioBtn->setChecked(false);

    this->exec();
}

void
InsuranceDialog::on_ensureBtn_clicked()
{
    INSURANCE_RECORD record;
    record.number = ui->numberLabel->text();
    record.company = ui->companyLE->text();
    record.carNumber = ui->carNumComboBox->currentText();
    record.date = QDate::fromString(ui->dateDateEdit->text(), "yyyy-MM-dd");
    record.remarks = ui->textEdit->toPlainText();
    record.fee = ui->feeDoubleSpinBox->value();
    if (ui->businessInsuranceRadioBtn->isChecked()) {
        if (!mDb->insertBusinessInsuranceTable(record)) {
            QMessageBox::information(this,
                                     tr("温馨提示"),
                                     tr("添加成功!\n"),
                                     QMessageBox::Ok,
                                     QMessageBox::Ok);
            emit addInsuranceItemSignal(record);
        } else {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("添加失败!未知错误.\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
        }
    } else if (ui->insuranceRadioBtn->isChecked()) {
        if (!mDb->insertInsuranceTable(record)) {
            QMessageBox::information(this,
                                     tr("温馨提示"),
                                     tr("添加成功!\n"),
                                     QMessageBox::Ok,
                                     QMessageBox::Ok);
            emit addBusinessInsuranceItemSignal(record);
        } else {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("添加失败!未知错误.\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
        }
    }
    clean();
    this->close();
}

void
InsuranceDialog::closeEvent(QCloseEvent *)
{
    clean();
}

void
InsuranceDialog::on_cancelBtn_clicked()
{
    clean();
    this->close();
}

void
InsuranceDialog::clean()
{
    ui->insuranceRadioBtn->setChecked(true);
    ui->businessInsuranceRadioBtn->setChecked(false);
    ui->feeDoubleSpinBox->setValue(0);
    ui->textEdit->setText("");
    ui->carNumComboBox->clear();
}
