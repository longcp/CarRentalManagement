#include "carannualdialog.h"
#include "ui_carannualdialog.h"
#include <QDate>
#include <database/database.h>
#include <car.h>
#include <datatype.h>
#include <QMessageBox>

#define LOG_TAG                 "ANNUAL_DIALOG"
#include "utils/Log.h"

CarAnnualDialog::CarAnnualDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CarAnnualDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("车辆年审");
    this->setFixedSize(400, 300);
    mDb = DataBase::getInstance();
}

CarAnnualDialog::~CarAnnualDialog()
{
    delete ui;
}

void
CarAnnualDialog::openWindow()
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

    QDate date = QDate::currentDate();
    int hour = QTime::currentTime().hour();
    int minute = QTime::currentTime().minute();
    int second = QTime::currentTime().second();
    QString curTime = QString::number(hour)
                        .append(QString::number(minute)
                        .append(QString::number(second)));
    ALOGD("%s, %s", date.toString("yyyyMMdd").toStdString().data(),
          curTime.toStdString().data());
    QString number = date.toString("yyyyMMdd").append(curTime);
    ui->numberLabel->setText(number);
    ui->dateDateEdit->setDate(QDate::currentDate());

    this->exec();
}

void
CarAnnualDialog::on_ensureBtn_clicked()
{
    ANNUALFEE_RECORD record;
    record.number = ui->numberLabel->text();
    record.carNumber = ui->carNumComboBox->currentText();
    record.remarks = ui->textEdit->toPlainText();
    record.annualFee = ui->annualFeeDoubleSpinBox->value();
    record.travelExpenses = ui->travelExpensesDoubleSpinBox->value();
    record.date = QDate::fromString(ui->dateDateEdit->text(), "yyyy-MM-dd");
    if (!mDb->insertAnnualTable(record)) {
        QMessageBox::information(this,
                                 tr("温馨提示"),
                                 tr("添加成功!\n"),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        emit addAnnualItemSignal(record);
    } else {
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("添加失败!未知错误.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    }
    clean();
    this->close();
}

void
CarAnnualDialog::on_cancelBtn_clicked()
{
    clean();
    this->close();
}

void
CarAnnualDialog::closeEvent(QCloseEvent *event)
{
    clean();
}

void
CarAnnualDialog::clean()
{
    ALOGD("%s", __FUNCTION__);
    ui->annualFeeDoubleSpinBox->setValue(0);
    ui->travelExpensesDoubleSpinBox->setValue(0);
    ui->textEdit->setText("");
    ui->carNumComboBox->clear();
}
