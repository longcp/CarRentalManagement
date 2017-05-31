#include "contractpricedialog.h"
#include "ui_contractpricedialog.h"
#include "database/database.h"
#include "datatype.h"
#include "car.h"
#include <QMessageBox>

#define LOG_TAG                 "CONTRACT_PRICE_DIALOG"
#include "utils/Log.h"

ContractPriceDialog::ContractPriceDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::ContractPriceDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("合同价格");
    setPumpTypeView();
}

ContractPriceDialog::~ContractPriceDialog()
{
    delete ui;
}

void
ContractPriceDialog::openWindow(OpenType type, QString contractNumber)
{
    ALOGD("%s, contractNumber = %s", __FUNCTION__,
          contractNumber.toStdString().data());
    mContractNumber = contractNumber;
    mOpenType = type;
    QString number = QDateTime::currentDateTime().toString(DATETIME_FORMAT_STR);
    ui->numberLabel->setText(number);
    ui->pumpTypeCB->setCurrentIndex(0);
    this->exec();
}

void
ContractPriceDialog::on_ensureBtn_clicked()
{
    CONTRACT_PRICE price;
    price.number = ui->numberLabel->text();
    price.contractNumber = mContractNumber;
    price.pumpType = Car::getPumpType(ui->pumpTypeCB->currentText());
    price.remarks = ui->remarksTextEdit->toPlainText();
    price.squarePrice = ui->squrePriceDSB->value();
    price.standardPrice = ui->standardPriceDSB->value();

    ALOGD("%s, contractNumber = %s", __FUNCTION__,
          price.contractNumber.toStdString().data());
    if (mOpenType == OpenType::CREATEITEM) {
        emit addPriceItemSignal(price);
    } else {
        if (!mDb->insertContractPriceTable(price)) {
            QMessageBox::information(this,
                                     tr("温馨提示"),
                                     tr("添加成功!\n"),
                                     QMessageBox::Ok,
                                     QMessageBox::Ok);
            emit addPriceItemSignal(price);
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
ContractPriceDialog::closeEvent(QCloseEvent *event)
{
    clean();
}

void
ContractPriceDialog::on_cancelBtn_clicked()
{
    clean();
    this->close();
}

void
ContractPriceDialog::clean()
{
    ui->numberLabel->setText("");
    ui->pumpTypeCB->setCurrentIndex(0);
    ui->squrePriceDSB->setValue(0);
    ui->standardPriceDSB->setValue(0);
    ui->remarksTextEdit->setText("");
}

void
ContractPriceDialog::setPumpTypeView()
{
    ui->pumpTypeCB->insertItem(PUMP_TYPE_37M1_COLUMN,
                                  PUMP_TYPE_37M1_STR);
    ui->pumpTypeCB->insertItem(PUMP_TYPE_48M_COLUMN,
                                  PUMP_TYPE_48M_STR);
    ui->pumpTypeCB->insertItem(PUMP_TYPE_52M_COLUMN,
                                  PUMP_TYPE_52M_STR);
    ui->pumpTypeCB->insertItem(PUMP_TYPE_56M_COLUMN,
                                  PUMP_TYPE_56M_STR);
    ui->pumpTypeCB->insertItem(PUMP_TYPE_60M_COLUMN,
                                  PUMP_TYPE_60M_STR);
    ui->pumpTypeCB->insertItem(PUMP_TYPE_CAR_PUMP_COLUMN,
                                  PUMP_TYPE_CAR_PUMP_STR);
}
