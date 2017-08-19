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
#include <QTableView>
#include <clienttabledialog.h>
#include <client.h>

#define LOG_TAG                 "CONTRACT_EDIT_DIALOG"
#include "utils/Log.h"

ContractEditDialog::ContractEditDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    mContractPriceDialog(new ContractPriceDialog()),
    mOriginContract(new Contract()),
    mCurRow(-1),
    mClientTableDialog(new ClientTableDialog()),
    mAddRows(0),
    ui(new Ui::ContractEditDialog)
{
    ui->setupUi(this);

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
    configToolBar();
    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActCancel);
    mToolBar->addAction(mActExit);
    ui->toolBarHorizontalLayout->addWidget(mToolBar);
    initView();

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
    /**
     * @brief 打开客户列表
     */
    connect(this,  SIGNAL(openClientTableWindowSignal()),
            mClientTableDialog, SLOT(openWindow()));
    /**
     * @brief 获取被选中客户
     */
    connect(mClientTableDialog, SIGNAL(selectedClient(QString)),
            this, SLOT(getSelectedClient(QString)));
}

ContractEditDialog::~ContractEditDialog()
{
    delete ui;
}

void
ContractEditDialog::initView()
{
    ALOGDTRACE();
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle("编辑合同");
    this->setFixedSize(1300, 700);

    initPriceTableView();
}

void
ContractEditDialog::configToolBar()
{
    ALOGDTRACE();
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
                                               Contract &contract,
                                               QString clientName,
                                               QString clientNumber)
{
    ALOGDTRACE();
    int count = 0;
    QList<Contract> contracts;
    Contract tmp;
    mOpenType = opentype;
    ui->clientNameLable->setText(clientName);
    ui->clientNumberLebel->setText(clientNumber);
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
        saveUiContent(tmp);
        setOriginContract(tmp);
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
    ALOGDTRACE();
    QStringList headerList;
    headerList << "编号" << "泵式" << "方量价格/方" << "台班价格/小时" << "备注";

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
    ui->priceTableView->setColumnWidth(0, 200);
    //自动适应列宽
    ui->priceTableView->horizontalHeader()
            ->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);
}

void
ContractEditDialog::setEditMode()
{
    ALOGDTRACE();
    ui->projectNameLE->setFocus();
    mActEdit->setDisabled(true);
    mActCancel->setEnabled(true);
    mActSave->setEnabled(true);
    setMode(true);
    if (ui->isIncludeTexCB->isChecked())
        ui->taxRateSB->setEnabled(true);
    else
        ui->taxRateSB->setEnabled(false);
}

void
ContractEditDialog::setViewMode()
{
    ALOGDTRACE();
    mActCancel->setDisabled(true);
    mActEdit->setEnabled(true);
    mActSave->setDisabled(true);
    setMode(false);
}

void
ContractEditDialog::setMode(bool mode)
{
    ALOGDTRACE();
//    ui->contractNumberLabel->setEnabled(mode);
    ui->signedDateDE->setEnabled(mode);
    ui->clientNameLable->setEnabled(mode);
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
    ALOGDTRACE();
    ui->contractNumberLabel->setText(contract.number);
    ui->signedDateDE->setDate(contract.signedDate);
    ui->clientNameLable->setText(contract.clientName);
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

    if (mModel->rowCount() > 0) {
        // 删除所有行并清除item内存
        mModel->removeRows(0, mModel->rowCount());
    }
    // 插入数据到价格表
    QList<QStandardItem*> items;
    for (int i = 0; i < contract.prices.size(); i++) {
        QStandardItem* num =
                new QStandardItem(contract.prices.at(i).number);
        QStandardItem* pumpType =
                new QStandardItem(Car::getPumpTypeStr(contract.prices.at(i).pumpType));
        QStandardItem* squarePrice =
                new QStandardItem(QString("%1").arg(contract.prices.at(i).squarePrice));
        QStandardItem* standardPrice =
                new QStandardItem(QString("%1").arg(contract.prices.at(i).standardPrice));
        QStandardItem* remarks = new QStandardItem(contract.prices.at(i).remarks);
        if (!items.isEmpty())
            items.clear();
        items << num << pumpType << squarePrice << standardPrice
              << remarks;
        mModel->appendRow(items);
    }
}

void
ContractEditDialog::resetView(Contract &contract)
{
    ALOGDTRACE();
    setOriginContract(contract);
    resetView();
}

void
ContractEditDialog::resetView()
{
    ALOGDTRACE();
    if (!isModified())
        return;

    setView(*mOriginContract);
    mAddRows = 0;
}

bool
ContractEditDialog::isModified()
{
    ALOGDTRACE();
    Contract tmp;
    saveUiContent(tmp);
    if (mOriginContract->isValueEqual(tmp)) {
        return false;
    }
    return true;
}

void
ContractEditDialog::closeEvent(QCloseEvent *event)
{
    ALOGDTRACE();
    if (mOpenType == CREATEITEM)
        ALOGDTRACE();
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
    ALOGDTRACE();
    ui->contractNumberLabel->setText("");
    ui->clientNameLable->setText("");
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
    mAddRows = 0;

    clearPriceTable();
}

void
ContractEditDialog::clearPriceTable()
{
    ALOGDTRACE();
    if (mModel->rowCount() > 0) {
        // 删除所有行并清除item内存
        mModel->removeRows(0, mModel->rowCount());
    }
}

void
ContractEditDialog::saveUiContent(Contract &contract)
{
    ALOGDTRACE();
    bool ok;

    contract.number = ui->contractNumberLabel->text();
    contract.clientName = ui->clientNameLable->text();
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
                ui->signedDateDE->text(), DATE_FORMAT_STR);
    contract.beginDate = QDate::fromString(
                ui->startDateDE->text(), DATE_FORMAT_STR);
    contract.endDate = QDate::fromString(
                ui->endDateDE->text(), DATE_FORMAT_STR);
    contract.creatDate = QDate::fromString(
                ui->createDateDE->text(), DATE_FORMAT_STR);

    contract.isIncludeTax = ui->isIncludeTexCB->isChecked() ? true : false;

    CONTRACT_PRICE price;
    int rowSize = mModel->rowCount();
    contract.prices.clear();
    for (int i = 0; i < rowSize; i++) {
        price.number = mModel->index(i, 0).data().toString();
        price.contractNumber = ui->contractNumberLabel->text();
        price.pumpType = Car::getPumpType(
                    mModel->index(i, 1).data().toString());
        price.squarePrice = mModel->index(i, 2).data().toDouble(&ok);
        price.standardPrice = mModel->index(i, 3).data().toDouble(&ok);
        price.remarks = mModel->index(i, 4).data().toString();

        contract.prices.push_back(price);
    }
}

void
ContractEditDialog::editEvent()
{
    ALOGDTRACE();
    setEditMode();
    //已存在的合同，其合同编码不可再次编辑
//    ui->contractNumberLabel->setDisabled(true);
}

void
ContractEditDialog::saveAndExitEvent()
{
    ALOGDTRACE();
    int ret;

    if (mModel->rowCount() <= 0) {
        QMessageBox::warning(this, tr("温馨提示"),
                             tr("价格表不能为空！\n"),
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
    ALOGDTRACE();
    Contract contract;

    saveUiContent(contract);
    if (!mDb->updateContractTableData(contract)) {
        //先把旧数据都删除，再重新插入数据
        mDb->delContractPriceInContractNumber(contract.number);
        for (int i = 0; i < contract.prices.size(); i++) {
            mDb->insertContractPriceTable(contract.prices.at(i));
        }
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
    ALOGDTRACE();
    resetView();
    setViewMode();
}

void
ContractEditDialog::addPriceItemSlot(CONTRACT_PRICE &price)
{
    ALOGDTRACE();
    QStandardItem *num
            = new QStandardItem(price.number);
    QStandardItem *pumpType
            = new QStandardItem(Car::getPumpTypeStr(price.pumpType));
    QStandardItem* squarePrice
            = new QStandardItem(QString("%1").arg(price.squarePrice));
    QStandardItem* standardPrice
            = new QStandardItem(QString("%1").arg(price.standardPrice));
    QStandardItem *remarks
            = new QStandardItem(price.remarks);

    QList<QStandardItem*> items;
    items << num << pumpType <<squarePrice <<standardPrice <<remarks;
    mModel->appendRow(items);
    mAddRows++;
}

void
ContractEditDialog::getSelectedClient(QString number)
{
    ALOGDTRACE();

    Client client;
    if (!mDb->getClientInNumber(number, client)) {
        ui->clientNameLable->setText(client.name);
        ui->clientNumberLebel->setText(client.number);
    }
}

void
ContractEditDialog::on_addBtn_clicked()
{
    ALOGDTRACE();
    emit openContractPriceWindowSignal(mOpenType, ui->contractNumberLabel->text());
}

void
ContractEditDialog::on_deleteBtn_clicked()
{
    ALOGDTRACE();
    if (mCurRow < 0) {
        QMessageBox::warning(this,
                             tr("温馨提示"),
                             tr("请选择要删除条目.\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    int ret = QMessageBox::warning(this,
                                   tr("温馨提示"),
                                   tr("确定要删除该条目吗？.\n"),
                                   QMessageBox::Yes |
                                   QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    QString number = "";
    number = mModel->index(mCurRow, 0).data().toString();
    if (mOpenType == OpenType::CREATEITEM) {
        //FIXME:创建条目方式，不删除数据库条目，因为条目尚未插入数据库
        mModel->removeRow(mCurRow);
        mAddRows--;
    } else {
        /*
         * 修改条目方式才删除数据库条目，因为在此种方式下，添加price条目时已经
         * 把price插入数据库
         */
        if (!mDb->delProjectDataInNumber(number)) {
            ALOGD("%s, delete ok", __FUNCTION__);
            mModel->removeRow(mCurRow);
            mAddRows--;
        }
    }
}

void ContractEditDialog::on_isIncludeTexCB_stateChanged(int state)
{
    ALOGDTRACE();
    if (state == Qt::Checked)
        ui->taxRateSB->setEnabled(true);
    else
        ui->taxRateSB->setDisabled(true);
}

void
ContractEditDialog::on_priceTableView_clicked(const QModelIndex &index)
{
    ALOGDTRACE();
    mCurRow = index.row();
}
