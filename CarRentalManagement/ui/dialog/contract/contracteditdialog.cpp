#include "contracteditdialog.h"
#include "ui_contracteditdialog.h"
#include <database/database.h>
#include <QAction>
#include <QToolBar>
#include <tablemodel.h>
#include <contractpricedialog.h>
#include <contract.h>
#include <QMessageBox>
#include <car.h>

#define LOG_TAG                 "CONTRACT_EDIT_DIALOG"
#include "utils/Log.h"

ContractEditDialog::ContractEditDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    mContractPriceDialog(new ContractPriceDialog()),
    mOriginContract(new Contract()),
    ui(new Ui::ContractEditDialog)
{
    ui->setupUi(this);

    initView();

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

    mToolBar = new QToolBar(tr("contractEditToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActPrev);
    mToolBar->addAction(mActNext);
    mToolBar->addAction(mActCancel);
    mToolBar->addAction(mActExit);
    ui->toolBarHorizontalLayout->addWidget(mToolBar);

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
     * @brief 打开价格窗口
     */
    connect(this, SIGNAL(openContractPriceWindowSignal(OpenType, QString)),
            mContractPriceDialog, SLOT(openWindow(OpenType, QString)));
    /**
     * @brief 添加价格
     */
    connect(mContractPriceDialog, SIGNAL(addPriceItemSignal(CONTRACT_PRICE &)),
            this, SLOT(addPriceItemSlot(CONTRACT_PRICE &)));
}

ContractEditDialog::~ContractEditDialog()
{
    delete ui;
}

void
ContractEditDialog::initView()
{
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle("编辑合同");
    this->setFixedSize(1300, 700);

    initPriceTableView();
}

void
ContractEditDialog::configToolBar()
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
ContractEditDialog::openContractEditDialogSlot(OpenType opentype,
                                               Contract &contract)
{
    int count = 0;
    QList<Contract> contracts;
    mOpenType = opentype;
    if (opentype == OpenType::CREATEITEM) {
        //以创建条目方式打开
        mActEdit->setDisabled(true);
        mActPrev->setDisabled(true);
        mActNext->setDisabled(true);
        ui->signedDateDE->setDate(QDate::currentDate());
        ui->startDateDE->setDate(QDate::currentDate());
        ui->endDateDE->setDate(QDate::currentDate());
        ui->createDateDE->setDate(QDate::currentDate());
        setEditMode();
        mActSave->setDisabled(true);
        mActCancel->setDisabled(true);
        if (!mDb->getAllContractData(contracts))
            count = contracts.size();
        ui->contractNumberLabel->setText(Contract::makeNewestContractNumber(count));
    } else {
        //以查看内容方式打开
        mActSave->setEnabled(true);
        mActPrev->setEnabled(true);
        mActNext->setEnabled(true);
        setViewMode();
        setOriginContract(contract);
        setView(contract);
    }
    this->exec();
}

void
ContractEditDialog::initPriceTableView()
{
    QStringList headerList;
    headerList << "编号" << "泵式" << "方量价格" << "标准台班价格"
               << "2.5小时内台班价格" << "4小时内台班价格" << "备注";

    mModel = new TableModel(0, headerList.size());
    ui->priceTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑
    ui->priceTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->priceTableView->verticalHeader()->setVisible(false);            //隐藏行表头
    ui->priceTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->priceTableView->setAlternatingRowColors(true);
    ui->priceTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->priceTableView->resizeColumnToContents(4);                      //自动适应列宽
    ui->priceTableView->resizeColumnToContents(5);
    ui->priceTableView->horizontalHeader()
            ->setSectionResizeMode(6, QHeaderView::Stretch);            //自动适应列宽
}

void
ContractEditDialog::setEditMode()
{
    ui->projectNameLE->setFocus();
    mActEdit->setDisabled(true);
    mActCancel->setEnabled(true);
    mActSave->setEnabled(true);
    setMode(true);
}

void
ContractEditDialog::setViewMode()
{
    mActCancel->setDisabled(true);
    mActEdit->setEnabled(true);
    mActSave->setDisabled(true);
    setMode(false);
}

void
ContractEditDialog::setMode(bool mode)
{
//    ui->contractNumberLabel->setEnabled(mode);
    ui->signedDateDE->setEnabled(mode);
    ui->clientNameCB->setEnabled(mode);
    ui->clientNumberLebel->setEnabled(mode);
    ui->projectNameLE->setEnabled(mode);
    ui->projectAddrLE->setEnabled(mode);
    ui->deliveryDSB->setEnabled(mode);
    ui->structureLevelDSB->setEnabled(mode);
    ui->startDateDE->setEnabled(mode);
    ui->endDateDE->setEnabled(mode);
    ui->requireTE->setEnabled(mode);
    ui->supplementTE->setEnabled(mode);
    ui->remarksTE->setEnabled(mode);
    ui->isIncludeTexCB->setEnabled(mode);
    ui->taxRateSB->setEnabled(mode);
    ui->creatorLE->setEnabled(mode);
    ui->createDateDE->setEnabled(mode);
    ui->addBtn->setEnabled(mode);
    ui->deleteBtn->setEnabled(mode);
}

void
ContractEditDialog::setOriginContract(Contract &contract)
{
    ALOGDTRACE();
    mOriginContract->copy(contract);
}

void
ContractEditDialog::setView(Contract &contract)
{
    ui->contractNumberLabel->setText(contract.number);
    ui->signedDateDE->setDate(contract.signedDate);
    ui->clientNameCB->setCurrentText(contract.clientName);
    ui->clientNumberLebel->setText(contract.clientNumber);
    ui->projectNameLE->setText(contract.projectName);
    ui->projectAddrLE->setText(contract.projectAddress);
    ui->deliveryDSB->setValue(contract.deliverySizes);
    ui->structureLevelDSB->setValue(contract.structureLevel);
    ui->startDateDE->setDate(contract.beginDate);
    ui->endDateDE->setDate(contract.endDate);
    ui->requireTE->setText(contract.requirement);
    ui->supplementTE->setText(contract.supplement);
    ui->remarksTE->setText(contract.remarks);
    ui->isIncludeTexCB->setChecked(contract.isIncludeTax ? true : false);
    ui->taxRateSB->setValue(contract.taxRate);
    ui->creatorLE->setText(contract.creator);
    ui->createDateDE->setDate(contract.creatDate);

    if (mModel->rowCount()) {
        // 删除所有行并清除item内存
        mModel->removeRows(0, mModel->rowCount());
    }
    // 插入数据到价格表
    QList<QStandardItem*> items;
    for (int i = 0; i < contract.prices.size(); i++) {
        QStandardItem* num =
                new QStandardItem(contract.prices.at(i).number);
        QStandardItem* pumpType =
                new QStandardItem(Car::getPumpTypeStr(contract.prices.at(i)
                                                      .pumpType));
        QStandardItem* squarePrice =
                new QStandardItem(QString("%1").arg(contract.prices.at(i)
                                                    .squarePrice));
        QStandardItem* standardPrice =
                new QStandardItem(QString("%1").arg(contract.prices.at(i)
                                                    .standardPrice));
        QStandardItem* within150MinPrice =
                new QStandardItem(QString("%1").arg(contract.prices.at(i)
                                                    .within150MinPrice));
        QStandardItem* within240MinPrice =
                new QStandardItem(QString("%1").arg(contract.prices.at(i)
                                                    .within240MinPrice));
        QStandardItem* remarks =
                new QStandardItem(contract.prices.at(i).remarks);
        if (!items.isEmpty())
            items.clear();
        items << num << pumpType << squarePrice << standardPrice
              << within150MinPrice << within240MinPrice << remarks;
        mModel->appendRow(items);
    }
}

void
ContractEditDialog::resetView(Contract &contract)
{
    setOriginContract(contract);
    resetView();
}

void
ContractEditDialog::resetView()
{
    if (!isModified())
        return;

    setView(*mOriginContract);
}

bool
ContractEditDialog::isModified()
{
    if (/*ui->contractNumberLabel->isModified() ||*/
            ui->signedDateDE->isWindowModified() ||
            ui->clientNameCB->isWindowModified() ||
            ui->clientNumberLebel->isWindowModified() ||
            ui->projectNameLE->isModified() ||
            ui->projectAddrLE->isModified() ||
            ui->deliveryDSB->isWindowModified() ||
            ui->structureLevelDSB->isWindowModified() ||
            ui->startDateDE->isWindowModified() ||
            ui->endDateDE->isWindowModified() ||
            ui->requireTE->isWindowModified() ||
            ui->supplementTE->isWindowModified() ||
            ui->remarksTE->isWindowModified() ||
            ui->isIncludeTexCB->isWindowModified() ||
            ui->taxRateSB->isWindowModified() ||
            ui->creatorLE->isModified() ||
            ui->createDateDE->isWindowModified()) {
        ALOGD("is modified!");
        return true;
    }

    return false;
}

void
ContractEditDialog::closeEvent(QCloseEvent *event)
{
    if (mOpenType == CREATEITEM)
        ALOGDTRACE();
    else if (isModified()) {
        // 有内容发生修改
        ALOGD("isModified");
        int ret = QMessageBox::warning(this, tr("温馨提示"),
                                       tr("是否保存修改？\n"),
                                       QMessageBox::Yes |
                                       QMessageBox::No |
                                       QMessageBox::Cancel,
                                       QMessageBox::Yes);
        if (ret == QMessageBox::Cancel)
            return;
        else if (ret == QMessageBox::Yes)
            saveEvent();
    }
    clean();
}

void
ContractEditDialog::closeDialog()
{
    ALOGDTRACE();
    this->close();
}

void
ContractEditDialog::clean()
{
    ALOGDTRACE();
    cleanContent();
}

void
ContractEditDialog::cleanContent()
{
    ui->contractNumberLabel->setText("");
    ui->clientNameCB->setCurrentText("");
    ui->clientNumberLebel->setText("");
    ui->projectNameLE->setText("");
    ui->projectAddrLE->setText("");
    ui->deliveryDSB->setValue(0);
    ui->structureLevelDSB->setValue(0);
    ui->taxRateSB->setValue(0);
    ui->isIncludeTexCB->setChecked(false);
    ui->requireTE->setText("");
    ui->supplementTE->setText("");
    ui->remarksTE->setText("");
    ui->creatorLE->setText("");

    clearPriceTable();
}

void
ContractEditDialog::clearPriceTable()
{
    if (mModel->rowCount()) {
        // 删除所有行并清除item内存
        mModel->removeRows(0, mModel->rowCount());
    }
}

void
ContractEditDialog::saveUiContent(Contract &contract)
{
    bool ok;

    contract.number = ui->contractNumberLabel->text();
    contract.clientName = ui->clientNameCB->currentText();
    contract.clientNumber = ui->clientNumberLebel->text();
    contract.projectName = ui->projectNameLE->text();
    contract.projectAddress = ui->projectAddrLE->text();
    contract.requirement = ui->requireTE->toPlainText();
    contract.supplement = ui->supplementTE->toPlainText();
    contract.remarks = ui->remarksTE->toPlainText();
    contract.creator = ui->creatorLE->text();

    contract.deliverySizes = ui->deliveryDSB->value();
    contract.structureLevel = ui->structureLevelDSB->value();
    contract.taxRate = ui->taxRateSB->value();

    contract.signedDate = QDate::fromString(
                ui->signedDateDE->text(), "yyyy-MM-dd");
    contract.beginDate = QDate::fromString(
                ui->startDateDE->text(), "yyyy-MM-dd");
    contract.endDate = QDate::fromString(
                ui->endDateDE->text(), "yyyy-MM-dd");
    contract.creatDate = QDate::fromString(
                ui->createDateDE->text(), "yyyy-MM-dd");

    contract.isIncludeTax = ui->isIncludeTexCB->isChecked() ? true : false;

    CONTRACT_PRICE price;
    int rowSize = mModel->rowCount();
    contract.prices.clear();
    for (int i = 0; i < rowSize; i++) {
        price.number = mModel->index(i, 0).data().toString();
        price.pumpType = Car::getPumpType(
                    mModel->index(i, 1).data().toString());
        price.squarePrice = mModel->index(i, 2).data().toFloat(&ok);
        price.standardPrice = mModel->index(i, 3).data().toFloat(&ok);
        price.within150MinPrice = mModel->index(i, 4).data().toFloat(&ok);
        price.within240MinPrice = mModel->index(i, 5).data().toFloat(&ok);
        price.remarks = mModel->index(i, 7).data().toString();
    }
}

void
ContractEditDialog::editEvent()
{
    setEditMode();
    //已存在的合同，其合同编码不可再次编辑
//    ui->contractNumberLabel->setDisabled(true);
}

void
ContractEditDialog::saveAndExitEvent()
{
    int ret;

    if (ui->clientNameCB->currentText().isEmpty() ||
            mModel->rowCount() <= 0) {
        QMessageBox::warning(this, tr("温馨提示"),
                             tr("客户名称和价格表不能为空！\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    if (!isModified()) {
        closeDialog();
        return;
    }

    Contract contract;
    saveUiContent(contract);
    contract.dump();

    if (mOpenType == CREATEITEM) {
        // 插入数据库,更新到界面
        if (mDb->isContractExist(contract)) {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("该合同已存在，添加失败!\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
            return;
        }

        if(!mDb->insertContractTable(contract)) {
            if (!contract.prices.isEmpty()) {
                for (int i = 0; i < contract.prices.size(); i++) {
                    // 插入价格
                    mDb->insertContractPriceTable(contract.prices.at(i));
                }
            }
            resetView(contract);
            emit addContractItemSignal(contract);
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
        // 更新到数据库
        if (!mDb->updateContractTableData(contract)) {
            resetView(contract);
            updateContractItemSignal(contract);
            ret = QMessageBox::information(this,
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
ContractEditDialog::saveEvent()
{
    Contract contract;

    saveUiContent(contract);
    if (!mDb->updateContractTableData(contract)) {
        resetView(contract);
        emit updateContractItemSignal(contract);
    } else {
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("保存失败!未知错误.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    }
}

void
ContractEditDialog::cancelEvent()
{
    resetView();
    setViewMode();
}

void
ContractEditDialog::addPriceItemSlot(CONTRACT_PRICE &price)
{
    ALOGDTRACE();
}

void
ContractEditDialog::on_addBtn_clicked()
{
    ALOGDTRACE();
    emit openContractPriceWindowSignal(mOpenType,
                                       ui->contractNumberLabel->text());
}

void
ContractEditDialog::on_deleteBtn_clicked()
{

}

void ContractEditDialog::on_isIncludeTexCB_stateChanged(int state)
{
    if (state == Qt::Checked)
        ui->taxRateSB->setEnabled(true);
    else
        ui->taxRateSB->setDisabled(true);
}
