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
#include <contract.h>
#include <client.h>
#include <cartabledialog.h>
#include <contracttabledialog.h>
#include <pricetabledialog.h>
#include <QMutex>
#include <user.h>
#include <util.h>

#define LOG_TAG                 "RENTALDOCUMENT_EDIT_DIALOG"
#include "utils/Log.h"

RentalDocumentEditDialog::RentalDocumentEditDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    mMutex(new QMutex(QMutex::Recursive)),
    isSetWindowSize(false),
    mCarTableDialog(new CarTableDialog),
    mRentalDocument(new RentalDocument()),
    mContractTableDialog(new ContractTableDialog),
    mPriceTableDialog(new PriceTableDialog()),
    ui(new Ui::RentalDocumentEditDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("泵送签证单");
    initView();
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
    configToolBar();
    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
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
    /**
     * @brief 选择car
     */
    connect(this, SIGNAL(openCarTableDialogSignal()),
            mCarTableDialog, SLOT(openWindow()));
    /**
     * @brief 选择car
     */
    connect(mCarTableDialog, SIGNAL(selectedCar(QString)),
            this, SLOT(getCar(QString)));
    /**
     * @brief 选择contract
     */
    connect(this, SIGNAL(openContractTableDialogSignal(QString)),
            mContractTableDialog, SLOT(openWindow(QString)));
    /**
     * @brief 选择contract
     */
    connect(mContractTableDialog, SIGNAL(selectedContract(QString)),
            this, SLOT(getContract(QString)));
    /**
     * @brief 选择价格
     */
    connect(this, SIGNAL(openPriceWindow(QString)),
            mPriceTableDialog, SLOT(openWindow(QString)));
    /**
     * @brief 获取价格
     */
    connect(mPriceTableDialog, SIGNAL(selectedPrice(CONTRACT_PRICE)),
            this, SLOT(getPrice(CONTRACT_PRICE)));
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
    setPumpTypeView();
    //pumpType值由客户选择的车辆决定
    ui->pumpTypeCB->setEnabled(false);
}

void
RentalDocumentEditDialog::setPumpTypeView()
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

int
RentalDocumentEditDialog::getPumpTypePosition(PumpType type)
{

    switch (type) {
    case TYPE_37M1:
       return PUMP_TYPE_37M1_COLUMN;

    case TYPE_48M:
       return PUMP_TYPE_48M_COLUMN;

    case TYPE_52M:
       return PUMP_TYPE_52M_COLUMN;

    case TYPE_56M:
       return PUMP_TYPE_56M_COLUMN;

    case TYPE_60M:
       return PUMP_TYPE_60M_COLUMN;

    case TYPE_CAR_PUMP:
       return PUMP_TYPE_CAR_PUMP_COLUMN;

    default:
        break;
    }

    return PUMP_TYPE_37M1_COLUMN;
}

void
RentalDocumentEditDialog::setWindowSize()
{
    ALOGD("this->height()=%d, this->width()=%d", this->height(), this->width());
    this->setFixedSize(this->width(), this->height());
}

void
RentalDocumentEditDialog::initViewWithUser(User &user)
{
    if (!user.isRoot()) {
        mActSaveExit->setEnabled(false);
        mActSave->setEnabled(false);
        mActEdit->setEnabled(false);
        mActCancel->setEnabled(false);
        mCurUserIsRoot = false;
    } else {
        mCurUserIsRoot = true;
    }
}

void
RentalDocumentEditDialog::openWindow(OpenType type,
                                     RentalDocument &doc,
                                     QString clientName,
                                     QString clientNumber)
{
    RentalDocument tmp;
    mOpenType = type;
    if (type == OpenType::CREATEITEM) {
        //已创建条目方式打开
        mActEdit->setDisabled(true);
        setEditMode();
        mActSave->setDisabled(true);
        mActCancel->setDisabled(true);
        ui->pumpTypeCB->setCurrentIndex(0);
        ui->dateDE->setDate(QDate::currentDate());
        ui->arrivalTimeDTE->setDateTime(QDateTime::currentDateTime());
        ui->leaveTimeDTE->setDateTime(QDateTime::currentDateTime());
        ui->numberLabel->setText(Util::makeNumber(DataBaseTable::RENTALDOCUMENT_TABLE));
        mClientNumber = clientNumber;
        ui->clientNameLabel->setText(clientName);
        saveUiContent(tmp);
        setOriginRentalDocument(tmp);
    } else {
        //以查看方式打开
        if (mCurUserIsRoot)
            mActSave->setEnabled(true);

        setViewMode();
        setOriginRentalDocument(doc);
        setView(doc);
        mCarNumber = doc.carNumber;
        mClientNumber = doc.clientNumber;
    }

    if (!isSetWindowSize)
        setWindowSize();
    isSetWindowSize = true;

    this->exec();
}

QString
RentalDocumentEditDialog::makeRentalDocNumber()
{
    ALOGD("curdatetime=%s", QDateTime::currentDateTime()
          .toString(DATETIME_SERIAL_NUM_FORMAT_STR)
          .toStdString().data());
    return QDateTime::currentDateTime().toString(DATETIME_SERIAL_NUM_FORMAT_STR);
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
//    ui->clientNameCB->setFocus();
    if (mCurUserIsRoot) {
        mActEdit->setDisabled(true);
        mActCancel->setEnabled(true);
        mActSave->setEnabled(true);
    }
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
    if (mCurUserIsRoot) {
        mActCancel->setDisabled(true);
        mActEdit->setEnabled(true);
        mActSave->setDisabled(true);
    }
}

void
RentalDocumentEditDialog::setViewModeSupplement()
{

}

void
RentalDocumentEditDialog::setMode(bool mode)
{
    ui->clientNameLabel->setEnabled(mode);
    ui->contractNumberLabel->setEnabled(mode);
    ui->carNumberLabel->setEnabled(mode);
//    ui->pumpTypeCB->setEnabled(mode);
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
    ui->receivedAccountsDSB->setEnabled(mode);
    ui->reservationRB->setEnabled(mode);
    ui->confirmedRB->setEnabled(mode);
    ui->unconfirmedRB->setEnabled(mode);
    ui->remarksTE->setEnabled(mode);
    ui->contractNumToolButton->setEnabled(mode);
    ui->carNumToolButton->setEnabled(mode);
    ui->pickPriceBtn->setEnabled(mode);
    ui->pumpSquareRb->setEnabled(mode);
    ui->pumpTimeRb->setEnabled(mode);
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
    ui->clientNameLabel->setText(doc.clientName);
    ui->contractNumberLabel->setText(doc.contractNumber);
    ui->carNumberLabel->setText(doc.carPlateNumber);
    ui->constructPlaceLE->setText(doc.constructPlace);
    ui->concreteLableLE->setText(doc.concreteLable);
    ui->beginFuelDSB->setValue(doc.beginFuel);
    ui->endFuelDSB->setValue(doc.endFuel);
    ui->dateDE->setDate(doc.date);
    ui->projectNameLE->setText(doc.projectName);
    ui->projectAddressLE->setText(doc.projectAddress);
    ui->projectAmountDSB->setValue(doc.projectAmount);
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
    ui->receivedAccountsDSB->setValue(doc.receivedAccounts);
    ui->remarksTE->setText(doc.remarks);
    ui->pumpTypeCB->setCurrentIndex(getPumpTypePosition(doc.pumpType));

    switch (doc.state) {
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

    if (doc.calculateType == CalculateType::PUMPSQUARE_CALCULATE) {
        ui->pumpSquareRb->setChecked(true);
        ui->pumpTimeRb->setChecked(false);
    } else {
        ui->pumpSquareRb->setChecked(false);
        ui->pumpTimeRb->setChecked(true);
    }
}

void
RentalDocumentEditDialog::saveAndExitEvent()
{
    if (ui->contractNumberLabel->text() == "") {
        QMessageBox::warning(this, tr("温馨提示"),
                             tr("合同号不能为空！"));
        return;
    }

    if (!isModified()) {
        closeDialog();
        return;
    }

    RentalDocument doc;
    saveUiContent(doc);
    if (mOpenType == OpenType::CREATEITEM) {
        //插入到数据库
        if (mDb->isRentalDocumentExist(doc)) {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("该签单已存在，添加失败!\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
            return;
        }

        if (!mDb->insertRentalDocumentTable(doc)) {
            setOriginRentalDocument(doc);                               //用作退出前比较
            emit addRentalDocSignal(doc);
            QMessageBox::information(this,
                                     tr("温馨提示"),
                                     tr("添加成功.\n"),
                                     QMessageBox::Ok,
                                     QMessageBox::Ok);
        } else {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("添加失败!未知错误.\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
            return;
        }
    } else {
        //更新到数据库
        if (!mDb->updateRentalDocumentData(doc)) {
            setOriginRentalDocument(doc);                               //用作退出前比较
            emit updateDocItemSignal(doc);
            QMessageBox::information(this,
                                     tr("温馨提示"),
                                     tr("已保存.\n"),
                                     QMessageBox::Ok,
                                     QMessageBox::Ok);
        } else {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("保存失败!未知错误.\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
            return;
        }
    }

    this->close();
}

void
RentalDocumentEditDialog::closeEvent(QCloseEvent *)
{

    if (mOpenType == CREATEITEM)
        ALOGD("CREATEITEM");
    else if (isModified()) {
        // 有内容发生修改
        ALOGD("isModified");
        int ret = QMessageBox::warning(this, tr("温馨提示"),
                                       tr("是否保存修改？\n"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes);
        if (ret == QMessageBox::Yes)
            saveEvent();
    }
    clean();
}

void
RentalDocumentEditDialog::saveEvent()
{
    RentalDocument doc;

    saveUiContent(doc);
    //更新数据到数据库、更新界面数据
    if (!mDb->updateRentalDocumentData(doc)) {
        setOriginRentalDocument(doc);                               //用作退出前比较
        emit updateDocItemSignal(doc);
    } else {
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("保存失败!未知错误.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    }
}

void
RentalDocumentEditDialog::cancelEvent()
{
    resetView();
    setViewMode();
}

void
RentalDocumentEditDialog::editEvent()
{
    if (mOpenType != OpenType::CREATEITEM &&
            mRentalDocument->state == RentalDocState::CONFIRMED_STATE) {
        QMessageBox::warning(this, tr("温馨提示"),
                             tr("已确认的合同无法再编辑!"));
        return;
    }

    setEditMode();
}

void
RentalDocumentEditDialog::closeDialog()
{
    ALOGDTRACE();
    this->close();
}

void
RentalDocumentEditDialog::getCar(QString number)
{
    Car car;
    int pos;
    if (!mDb->getCarInNumber(number, car)) {
        ui->carNumberLabel->setText(car.carNumber);
//        ui->carNumberLabel->setWindowModified(true);
        mCarNumber = car.number;
        pos = car.getPumpTypePosition(car.pumptype);
        ui->pumpTypeCB->setCurrentIndex(pos);
    }
}

void
RentalDocumentEditDialog::getContract(QString number)
{
    Contract contract;
    if (!mDb->getContractInNumber(number, contract)) {
        ui->contractNumberLabel->setText(contract.number);
    }
}

bool
RentalDocumentEditDialog::isModified()
{
    RentalDocument tmp;
    saveUiContent(tmp);
    if (mRentalDocument->isValueEqual(tmp)) {
        return false;
    }
    return true;
}

void
RentalDocumentEditDialog::saveUiContent(RentalDocument &doc)
{
    bool ok;

    doc.number = ui->numberLabel->text();
    doc.clientName = ui->clientNameLabel->text();
    doc.clientNumber = mClientNumber;
    doc.contractNumber = ui->contractNumberLabel->text();
    doc.carNumber = mCarNumber;
    doc.carPlateNumber = ui->carNumberLabel->text();
    doc.principal = ui->principalLE->text();
    doc.principalTel = ui->principalTelLE->text();
    doc.constructPlace = ui->constructPlaceLE->text();
    doc.concreteLable = ui->concreteLableLE->text();
    doc.driver1 = ui->driver1LE->text();
    doc.driver2 = ui->driver2LE->text();
    doc.driver3 = ui->driver3LE->text();
    doc.projectName = ui->projectNameLE->text();
    doc.projectAddress = ui->projectAddressLE->text();
    doc.remarks = ui->remarksTE->toPlainText();

    doc.beginFuel = ui->beginFuelDSB->value();
    doc.endFuel = ui->endFuelDSB->value();
    doc.projectAmount = ui->projectAmountDSB->value();
    doc.workingHours = ui->workingHoursDSB->value();
    doc.pumpSquare = ui->pumpSquareDSB->value();
    doc.squareUnitPrice = ui->squareUnitPriceDSB->value();
    doc.pumpTimes = ui->pumpTimesDSB->value();
    doc.pumpTimeUnitPrice = ui->pumpTimeUnitPriceDSB->value();
    doc.receivedAccounts = ui->receivedAccountsDSB->value();

    doc.pumpType = Car::getPumpType(ui->pumpTypeCB->currentIndex());
    doc.date = QDate::fromString(ui->dateDE->text(), DATE_FORMAT_STR);
    doc.arrivalDateTime = QDateTime::fromString(ui->arrivalTimeDTE->text(),
                                                DATETIME_FORMAT_STR);
    doc.leaveDateTime = QDateTime::fromString(ui->leaveTimeDTE->text(),
                                              DATETIME_FORMAT_STR);
    if (ui->reservationRB->isChecked())
        doc.state = RentalDocState::RESERVATION_STATE;
    else if (ui->confirmedRB->isChecked())
        doc.state = RentalDocState::CONFIRMED_STATE;
    else
        doc.state = RentalDocState::UNCONFIRMED_STATE;

    if (ui->pumpSquareRb->isChecked())
        doc.calculateType = CalculateType::PUMPSQUARE_CALCULATE;
    else
        doc.calculateType = CalculateType::PUMPTIME_CALCULATE;
}

void
RentalDocumentEditDialog::resetView(RentalDocument &doc)
{
    setOriginRentalDocument(doc);
    resetView();
}

void
RentalDocumentEditDialog::resetView()
{
    if (!isModified())
        return;

    setView(*mRentalDocument);
}

void
RentalDocumentEditDialog::clean()
{
    cleanContent();
}

void
RentalDocumentEditDialog::cleanContent()
{
    ui->numberLabel->setText("");
    ui->clientNameLabel->setText("");
    ui->contractNumberLabel->setText("");
    ui->carNumberLabel->setText("");
    ui->pumpTypeCB->setCurrentIndex(0);
    ui->constructPlaceLE->setText("");
    ui->concreteLableLE->setText("");
    ui->beginFuelDSB->setValue(0);
    ui->endFuelDSB->setValue(0);
    ui->dateDE->setDate(QDate::currentDate());
    ui->principalLE->setText("");
    ui->principalTelLE->setText("");
    ui->arrivalTimeDTE->setDateTime(QDateTime::currentDateTime());
    ui->leaveTimeDTE->setDateTime(QDateTime::currentDateTime());
    ui->workingHoursDSB->setValue(0);
    ui->driver1LE->setText("");
    ui->driver2LE->setText("");
    ui->driver3LE->setText("");
    ui->projectAmountDSB->setValue(0);
    ui->projectNameLE->setText("");
    ui->projectAddressLE->setText("");
    ui->pumpSquareDSB->setValue(0);
    ui->squareUnitPriceDSB->setValue(0);
    ui->pumpTimesDSB->setValue(0);
    ui->pumpTimeUnitPriceDSB->setValue(0);
    ui->receivedAccountsDSB->setValue(0);
    ui->reservationRB->setChecked(true);
    ui->confirmedRB->setChecked(false);
    ui->unconfirmedRB->setChecked(false);
    ui->pumpSquareRb->setChecked(true);
    ui->pumpTimeRb->setChecked(false);
    ui->remarksTE->setText("");
}

void
RentalDocumentEditDialog::on_contractNumToolButton_clicked()
{
    emit openContractTableDialogSignal(mClientNumber);
}

void
RentalDocumentEditDialog::on_carNumToolButton_clicked()
{
    emit openCarTableDialogSignal();
}

void
RentalDocumentEditDialog::getPrice(CONTRACT_PRICE price)
{
    ui->squareUnitPriceDSB->setValue(price.squarePrice);
    ui->pumpTimeUnitPriceDSB->setValue(price.standardPrice);
}

void
RentalDocumentEditDialog::on_pickPriceBtn_clicked()
{
    if (ui->contractNumberLabel->text() == "") {
        QMessageBox::warning(this, tr("温馨提示"),
                             tr("请先选择合同!"));
        return;
    }
    emit openPriceWindow(ui->contractNumberLabel->text());
}

void
RentalDocumentEditDialog::updateProjectAmountValue(double value)
{
    QMutexLocker locker(mMutex);
    ui->projectAmountDSB->setValue(value);
}

void
RentalDocumentEditDialog::on_squareUnitPriceDSB_valueChanged(double value)
{
    double num;
    if (ui->pumpTimeRb->isChecked()) {
        return;
    }

    num = ui->pumpSquareDSB->value();
    updateProjectAmountValue(num * value);
}

void
RentalDocumentEditDialog::on_pumpTimeUnitPriceDSB_valueChanged(double value)
{
    double num;
    if (ui->pumpSquareRb->isChecked()) {
        return;
    }

    num = ui->pumpTimesDSB->value();
    updateProjectAmountValue(num * value);
}

void
RentalDocumentEditDialog::on_pumpSquareDSB_valueChanged(double value)
{
    if (ui->pumpTimeRb->isChecked())
        return;

    updateProjectAmountValue(value * ui->squareUnitPriceDSB->value());
}

void
RentalDocumentEditDialog::on_pumpTimesDSB_valueChanged(double value)
{
    if (ui->pumpSquareRb->isChecked())
        return;

    updateProjectAmountValue(value * ui->pumpTimeUnitPriceDSB->value());
}

void
RentalDocumentEditDialog::on_pumpSquareRb_toggled(bool checked)
{
    if (!checked)
        return;

    updateProjectAmountValue(ui->pumpSquareDSB->value() * ui->squareUnitPriceDSB->value());
}

void
RentalDocumentEditDialog::on_pumpTimeRb_toggled(bool checked)
{
    if (!checked)
        return;

    updateProjectAmountValue(ui->pumpTimesDSB->value() * ui->pumpTimeUnitPriceDSB->value());
}
