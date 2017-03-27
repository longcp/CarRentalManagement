#include "rentaldocumenteditdialog.h"
#include "ui_rentaldocumenteditdialog.h"
#include <QToolBar>
#include <QIcon>
#include <QMessageBox>
#include <QAction>
#include <database/database.h>
#include <datatype.h>
#include <rentaldocument.h>
#include <QDate>
#include <QDateTime>
#include <car.h>

#define LOG_TAG                 "RENTALDOCUMENT_EDIT_DIALOG"
#include "utils/Log.h"

RentalDocumentEditDialog::RentalDocumentEditDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::RentalDocumentEditDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("泵送签证单");
//    this->setFixedSize(800, 650);

    mActSave = new QAction(QIcon(":/menu/icon/save_64.ico"),
                          tr("保存"), this);
    mActEdit = new QAction(QIcon(":/menu/icon/edit_64.ico"),
                          tr("修改"), this);
    mActPrev = new QAction(QIcon(":/menu/icon/arrow_left_64.ico"),
                          tr("上一条"), this);
    mActNext = new QAction(QIcon(":/menu/icon/arrow_right_64.ico"),
                          tr("下一条"), this);
    mActExit = new QAction(QIcon(":/menu/icon/exit_out_64.ico"),
                          tr("退出"), this);
    mActCancel = new QAction(QIcon(":/menu/icon/cancel_64.ico"),
                          tr("取消"), this);
    mActSaveExit = new QAction(QIcon(":/menu/icon/ok_64.ico"),
                          tr("保存退出"), this);

    mToolBar = new QToolBar(tr("rentalDoasdascEditToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActPrev);
    mToolBar->addAction(mActNext);
    mToolBar->addAction(mActCancel);
    mToolBar->addAction(mActExit);

    ui->toolBarHorizonLayout->addWidget(mToolBar);

    /**
     * @brief 编辑
     */
    connect(mActEdit, SIGNAL(triggered()),
            this, SLOT(editEvent()));
    /**
     * @brief 保存
     */
    connect(mActSave, SIGNAL(triggered()),
            this, SLOT(saveEvent()));
    /**
     * @brief 取消
     */
    connect(mActCancel, SIGNAL(triggered()),
            this, SLOT(cancelEvent()));
    /**
     * @brief 保存退出
     */
    connect(mActSaveExit, SIGNAL(triggered()),
            this, SLOT(saveAndExitEvent()));
    /**
     * @brief 退出
     */
    connect(mActExit, SIGNAL(triggered()),
            this, SLOT(closeDialog()));
}

RentalDocumentEditDialog::~RentalDocumentEditDialog()
{
    delete ui;
}

void
RentalDocumentEditDialog::configToolBar()
{
    if (!mToolBar)
        return;

    mToolBar->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    mToolBar->setLayoutDirection(Qt::LayoutDirectionAuto);
    mToolBar->setIconSize(QSize(24,24));
    mToolBar->setOrientation(Qt::Horizontal);
    mToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    mToolBar->setFloatable(true);
    mToolBar->setMovable(true);
    mToolBar->setEnabled(true);
    mToolBar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    mToolBar->setFocusPolicy(Qt::NoFocus);
    mToolBar->setContextMenuPolicy(Qt::DefaultContextMenu);
    mToolBar->setInputMethodHints(Qt::ImhNone);
    mToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void
RentalDocumentEditDialog::initView()
{

}


void
RentalDocumentEditDialog::openWindow(OpenType type, RentalDocument &doc)
{
    mOpenType = type;
    if (type == OpenType::CREATEITEM) {
        //已创建条目方式打开
        mActEdit->setDisabled(true);
        mActPrev->setDisabled(true);
        mActNext->setDisabled(true);
        ui->dateDE->setDate(QDate::currentDate());
        setEditMode();
        mActSave->setDisabled(true);
        mActCancel->setDisabled(true);
        ui->numberLabel->setText(makeRentalDocNumber());
    } else {
        //以查看方式打开
        mActSave->setEnabled(true);
        mActPrev->setEnabled(true);
        mActNext->setEnabled(true);
        setViewMode();
        setOriginRentalDocument(doc);
        setView(doc);
        mCarNumber = doc.carNumber;
        mClientNumber = doc.clientNumber;
    }
    this->exec();
}

QString
RentalDocumentEditDialog::makeRentalDocNumber()
{
    ALOGD("curdatetime=%s", QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")
          .toStdString().data());
    return QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
}

void
RentalDocumentEditDialog::setEditMode()
{
    setEditModePre();
    setMode(true);
    setEditModeSupplement();
}

void
RentalDocumentEditDialog::setEditModePre()
{
    ui->clientNameCB->setFocus();
    mActEdit->setDisabled(true);
    mActCancel->setEnabled(true);
    mActSave->setEnabled(true);
}

void
RentalDocumentEditDialog::setEditModeSupplement()
{

}

void
RentalDocumentEditDialog::setViewMode()
{
    setViewModePre();
    setMode(false);
    setViewModeSupplement();
}

void
RentalDocumentEditDialog::setViewModePre()
{
    mActCancel->setDisabled(true);
    mActEdit->setEnabled(true);
    mActSave->setDisabled(true);
}

void
RentalDocumentEditDialog::setViewModeSupplement()
{

}

void
RentalDocumentEditDialog::setMode(bool mode)
{
    ui->clientNameCB->setEnabled(mode);
    ui->contractNumberCB->setEnabled(mode);
    ui->carNumberCB->setEnabled(mode);
    ui->pumpTypeCB->setEnabled(mode);
    ui->constructPlaceLE->setEnabled(mode);
    ui->concreteLableLE->setEnabled(mode);
    ui->beginFuelDSB->setEnabled(mode);
    ui->endFuelDSB->setEnabled(mode);
    ui->dateDE->setEnabled(mode);
    ui->principalLE->setEnabled(mode);
    ui->principalTelLE->setEnabled(mode);
    ui->arrivalTimeDTE->setEnabled(mode);
    ui->leaveTimeDTE->setEnabled(mode);
    ui->workingHoursDSB->setEnabled(mode);
    ui->driver1LE->setEnabled(mode);
    ui->driver2LE->setEnabled(mode);
    ui->driver3LE->setEnabled(mode);
    ui->projectAmountDSB->setEnabled(mode);
    ui->projectNameLE->setEnabled(mode);
    ui->projectAddressLE->setEnabled(mode);
    ui->pumpSquareDSB->setEnabled(mode);
    ui->squareUnitPriceDSB->setEnabled(mode);
    ui->pumpTimesDSB->setEnabled(mode);
    ui->pumpTimeUnitPriceDSB->setEnabled(mode);
    ui->reservationRB->setEnabled(mode);
    ui->confirmedRB->setEnabled(mode);
    ui->unconfirmedRB->setEnabled(mode);
    ui->remarksTE->setEnabled(mode);
}

void
RentalDocumentEditDialog::setOriginRentalDocument(RentalDocument &doc)
{
    ALOGDTRACE();
    mRentalDocument->copy(doc);
}

void
RentalDocumentEditDialog::setView(RentalDocument &doc)
{
    ui->numberLabel->setText(doc.number);
    ui->clientNameCB->setCurrentText(doc.clientName);
    ui->contractNumberCB->setCurrentText(doc.contractNumber);
    ui->carNumberCB->setCurrentText(doc.carPlateNumber);
    ui->pumpTypeCB->setCurrentText(Car::getPumpTypeStr(doc.pumpType));
    ui->constructPlaceLE->setText(doc.contructPlace);
    ui->concreteLableLE->setText(doc.concreteLable);
    ui->beginFuelDSB->setValue(doc.beginFuel);
    ui->endFuelDSB->setValue(doc.endFuel);
    ui->dateDE->setDate(doc.date);
    ui->principalLE->setText(doc.principal);
    ui->principalTelLE->setText(doc.principalTel);
    ui->arrivalTimeDTE->setDateTime(doc.arrivalDateTime);
    ui->leaveTimeDTE->setDateTime(doc.leaveDateTime);
    ui->workingHoursDSB->setValue(doc.workingHours);
    ui->driver1LE->setText(doc.driver1);
    ui->driver2LE->setText(doc.driver2);
    ui->driver3LE->setText(doc.driver3);
    ui->pumpSquareDSB->setValue(doc.pumpSquare);
    ui->squareUnitPriceDSB->setValue(doc.squareUnitPrice);
    ui->pumpTimesDSB->setValue(doc.pumpTimes);
    ui->pumpTimeUnitPriceDSB->setValue(doc.pumpTimeUnitPrice);
    ui->remarksTE->setText(doc.remarks);

    switch (doc.rentalDocState) {
    case RentalDocState::RESERVATION_STATE:
        ui->reservationRB->setChecked(true);
        ui->confirmedRB->setChecked(false);
        ui->unconfirmedRB->setChecked(false);
        break;

    case RentalDocState::CONFIRMED_STATE:
        ui->confirmedRB->setChecked(true);
        ui->reservationRB->setChecked(false);
        ui->unconfirmedRB->setChecked(false);
        break;

    case RentalDocState::UNCONFIRMED_STATE:
        ui->unconfirmedRB->setChecked(true);
        ui->confirmedRB->setChecked(false);
        ui->reservationRB->setChecked(false);
        break;

    default:
        ui->reservationRB->setChecked(true);
        ui->unconfirmedRB->setChecked(false);
        ui->confirmedRB->setChecked(false);
        break;
    }
}

void
RentalDocumentEditDialog::saveAndExitEvent()
{
    if (ui->clientNameCB->currentText() == "" ||
            ui->contractNumberCB->currentText() == "") {
        QMessageBox::warning(this, tr("温馨提示"),
                             tr("承租客户和合同号不能为空！"));
        return;
    }

    if (!isModified()) {
        closeDialog();
        return;
    }
}

void
RentalDocumentEditDialog::saveEvent()
{

}

void
RentalDocumentEditDialog::cancelEvent()
{

}

void
RentalDocumentEditDialog::editEvent()
{

}

void
RentalDocumentEditDialog::closeDialog()
{

}

bool
RentalDocumentEditDialog::isModified()
{
    if (ui->clientNameCB->isWindowModified() ||
            ui->contractNumberCB->isWindowModified() ||
            ui->carNumberCB->isWindowModified() ||
            ui->pumpTypeCB->isWindowModified() ||
            ui->constructPlaceLE->isModified() ||
            ui->concreteLableLE->isModified() ||
            ui->beginFuelDSB->isWindowModified() ||
            ui->endFuelDSB->isWindowModified() ||
            ui->dateDE->isWindowModified() ||
            ui->principalLE->isModified() ||
            ui->principalTelLE->isModified() ||
            ui->arrivalTimeDTE->isWindowModified() ||
            ui->leaveTimeDTE->isWindowModified() ||
            ui->workingHoursDSB->isWindowModified() ||
            ui->driver1LE->isModified() ||
            ui->driver2LE->isModified() ||
            ui->driver3LE->isModified() ||
            ui->projectAmountDSB->isWindowModified() ||
            ui->projectNameLE->isModified() ||
            ui->projectAddressLE->isModified() ||
            ui->pumpSquareDSB->isWindowModified() ||
            ui->squareUnitPriceDSB->isWindowModified() ||
            ui->pumpTimesDSB->isWindowModified() ||
            ui->pumpTimeUnitPriceDSB->isWindowModified() ||
            ui->reservationRB->isWindowModified() ||
            ui->confirmedRB->isWindowModified() ||
            ui->unconfirmedRB->isWindowModified() ||
            ui->remarksTE->isWindowModified()) {
        ALOGD("is modified!");
        return true;
    }

    return false;
}
