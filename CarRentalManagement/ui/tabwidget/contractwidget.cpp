#include "contractwidget.h"
#include "ui_contractwidget.h"
#include <QToolBar>
#include <tablemodel.h>
#include <contracteditdialog.h>
#include <database/database.h>
#include <QMessageBox>
#include <contract.h>
#include <client.h>
#include <car.h>
#include <QModelIndex>

#define LOG_TAG                 "CONTRACT_WIDGET_DIALOG"
#include "utils/Log.h"

ContractWidget::ContractWidget(QWidget *parent) :
    mContractEditDialog(new ContractEditDialog()),
    QWidget(parent),
    mDb(DataBase::getInstance()),
    curRow(-1),
    ui(new Ui::ContractWidget)

{
    ui->setupUi(this);
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle(TAB_TITLE_CONTRACT);

    initView();

    mActAdd = new QAction(QIcon(":/menu/icon/add_64.ico"),
                          tr("增加"), this);
    mActEdit = new QAction(QIcon(":/menu/icon/edit_64.ico"),
                          tr("修改"), this);
    mActDelete = new QAction(QIcon(":/menu/icon/delete_64.ico"),
                             tr("删除"), this);
    mActSearch = new QAction(QIcon(":/menu/icon/search_64.ico"),
                             tr("查询"), this);
    mActExport = new QAction(QIcon(":/menu/icon/export_64.ico"),
                              tr("导出"), this);

    mToolBar = new QToolBar(tr("contractToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);

    ui->toolBarHorizonLayout->addWidget(mToolBar);


    /**
     * @brief 打开编辑窗口
     */
    connect(mActAdd, SIGNAL(triggered()),
            this, SLOT(addContractSlot()));
    /**
     * @brief 工具栏，编辑按钮
     */
    connect(mActEdit, SIGNAL(triggered()),
            this, SLOT(editContractItemSlot()));
    /**
     * @brief 打开编辑窗口
     */
    connect(this,
            SIGNAL(openContractEditDialogSignal(OpenType, Contract&, QString, QString)),
            mContractEditDialog,
            SLOT(openContractEditDialogSlot(OpenType, Contract&, QString, QString)));
    /**
     * @brief 添加条目
     */
    connect(mContractEditDialog, SIGNAL(addContractItemSignal(Contract&)),
            this, SLOT(addContractItemSlot(Contract&)));
    /**
     * @brief 更新条目
     */
    connect(mContractEditDialog, SIGNAL(updateContractItemSignal(Contract&)),
            this, SLOT(updateContractItemSLot(Contract&)));
    /**
     * @brief 删除条目
     */
    connect(mActDelete, SIGNAL(triggered()),
            this, SLOT(deleteContractItemSlot()));
    connect(ui->contractTableView->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&,const QModelIndex&)),
            this,
            SLOT(contractCurrentRowChangedSlot(const QModelIndex&,const QModelIndex&)));
}

ContractWidget::~ContractWidget()
{
    delete ui;
}

void
ContractWidget::configToolBar()
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
ContractWidget::initView()
{
    initContractTableview();
    initPriceTableview();
    initClientTreeWidget();
}

void
ContractWidget::initContractTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "合同号" << "客户名称" << "工程名" << "工程地址"
               << "运距(KM)" << "结构/层数" << "计划工期开始日"
               << "计划工期结束日" << "签订日期" << "税率%"
               << "是否含税" << "租赁要求" << "补充协议" << "备注";

    mContractModel = new TableModel(0, headerList.size());
    ui->contractTableView->setModel(mContractModel);
    mContractModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->contractTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->contractTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->contractTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->contractTableView->verticalHeader()->setVisible(false);         //隐藏行表头
    ui->contractTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->contractTableView->setAlternatingRowColors(true);
    ui->contractTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->contractTableView->setColumnWidth(0, 200);
    ui->contractTableView->setColumnWidth(1, 200);
    ui->contractTableView->setColumnWidth(2, 200);
    ui->contractTableView->setColumnWidth(3, 200);
//    setContractTableviewData();
}

void
ContractWidget::initPriceTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "泵式" << "方量价格/方" << "台班价格/小时"  << "备注";

    mPriceModel = new TableModel(0, headerList.size());
    ui->priceTableView->setModel(mPriceModel);
    mPriceModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->priceTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->priceTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->priceTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

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
//    setPriceTableViewData();
}

void
ContractWidget::initClientTreeWidget()
{
    ui->clientTreeWidget->setColumnWidth(0, 220);
    mRootItem = new QTreeWidgetItem(ui->clientTreeWidget,
                                    QStringList("所有客户"));
    mRootItem->setIcon(0, QIcon(":/menu/icon/client.png"));
    addAllClientItem();
}

void
ContractWidget::addAllClientItem()
{
    int size;
    QList<Client> clients;
    if (!mDb->getAllClientData(clients)) {
        size = clients.size();
        for (int i = 0; i < size; i++) {
            QStringList itemList;
            itemList << clients.at(i).name << clients.at(i).number;
            QTreeWidgetItem *newItem =
                    new QTreeWidgetItem(mRootItem, itemList);
            if (mDb->isClientHasContract(clients.at(i).number))
                newItem->setIcon(0, QIcon(":/menu/icon/contract_64.ico"));
            else
                newItem->setIcon(0, QIcon(":/menu/icon/empty_64.ico"));
            mRootItem->addChild(newItem);
        }
    }
}

void
ContractWidget::addContractSlot()
{
    ALOGD("ui->clientTreeWidget->currentIndex().row()=%d",
          ui->clientTreeWidget->currentIndex().row());
    Contract contract;
    if (!isSelectedClient()) {
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("请先选好客户.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    int curTreeRow = ui->clientTreeWidget->currentIndex().row();
    QString clientName = ui->clientTreeWidget->currentItem()
                            ->text(mClientNameColumn);
    QString clientNumber = ui->clientTreeWidget->currentItem()
                            ->text(mClientNumberColumn);
    emit openContractEditDialogSignal(OpenType::CREATEITEM, contract,
                                      clientName, clientNumber);
}

bool
ContractWidget::isSelectedClient()
{
    if (ui->clientTreeWidget->currentItem() == NULL ||
            ui->clientTreeWidget->currentItem()->parent() == NULL)
        return false;

    return true;
}

void
ContractWidget::editRowEvent(int row)
{
    ALOGDTRACE();
    Contract contract;
    QString number = mContractModel->index(row, 0).data().toString();
    if (mDb->getContractInNumber(number, contract)) {
        ALOGE("%s getContractInNumber failed, sql err = ",
              mDb->lastError().toStdString().data());
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("未知错误,无法查看该项.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    mDb->getAllContractPriceData(contract.number, contract.prices);
    emit openContractEditDialogSignal(OpenType::SHOWITEM, contract,
                                      contract.clientName, contract.clientNumber);
}

void
ContractWidget::editContractItemSlot()
{
    if (curRow < 0) {
        QMessageBox::information(this,
                                 tr("温馨提示"),
                                 tr("请选择要编辑条目.\n"),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        return;
    }

    editRowEvent(curRow);
}

void
ContractWidget::clearContractTable()
{
    if (mContractModel->rowCount())
        mContractModel->removeRows(0, mContractModel->rowCount());
}

void
ContractWidget::clearPriceTable()
{
    if (mPriceModel->rowCount())
        mPriceModel->removeRows(0, mPriceModel->rowCount());
}

void
ContractWidget::addContractItemSlot(Contract &contract)
{
    ALOGDTRACE();
    addContractTableRow(contract);
    ui->contractTableView->selectRow(mContractModel->rowCount()-1);
    clearPriceTable();
    addPriceTableRows(contract.prices);
}

void
ContractWidget::addContractTableRows(QList<Contract> &contracts)
{
    Contract contract;
    for (int i = 0; i < contracts.size(); i++) {
        contract = contracts.at(i);
        addContractTableRow(contract);
    }
}

void
ContractWidget::addContractTableRow(Contract &contract)
{
    ALOGDTRACE();
    QStandardItem *num
            = new QStandardItem(contract.number);
    QStandardItem *clientName
            = new QStandardItem(contract.clientName);
    QStandardItem *projectName
            = new QStandardItem(contract.projectName);
    QStandardItem *projectAddr
            = new QStandardItem(contract.projectAddress);
    QStandardItem* deliverySizes
            = new QStandardItem(QString("%1")
                                .arg(contract.deliverySizes));
    QStandardItem* structureLevel
            = new QStandardItem(QString("%1")
                                .arg(contract.structureLevel));
    QStandardItem* beginDate
            = new QStandardItem(contract
                                .beginDate.toString(DATE_FORMAT_STR));
    QStandardItem* endDate
            = new QStandardItem(contract
                                .endDate.toString(DATE_FORMAT_STR));
    QStandardItem* signedDate
            = new QStandardItem(contract
                                .signedDate.toString(DATE_FORMAT_STR));
    QStandardItem* taxRate
            = new QStandardItem(QString("%1").arg(contract.taxRate));
    QStandardItem* isIncludeTax
            = new QStandardItem(contract.isIncludeTax ? "是" : "否");
    QStandardItem *requirement
            = new QStandardItem(contract.requirement);
    QStandardItem *supplement
            = new QStandardItem(contract.supplement);
    QStandardItem *remarks
            = new QStandardItem(contract.remarks);

    QList<QStandardItem*> items;
    items << num << clientName << projectName << projectAddr
          << deliverySizes << structureLevel << beginDate
          << endDate << signedDate << taxRate << isIncludeTax
          << requirement << supplement << remarks;
    mContractModel->appendRow(items);
}

void
ContractWidget::addPriceTableRows(QList<CONTRACT_PRICE> prices)
{
    ALOGDTRACE();
    int size = prices.size();
    for (int i = 0; i < size; i++) {
        addPriceTableRow(prices.at(i));
    }
}

void
ContractWidget::addPriceTableRow(CONTRACT_PRICE price)
{
    ALOGDTRACE();
    QStandardItem *num
            = new QStandardItem(price.number);
    QStandardItem *pumyType
            = new QStandardItem(Car::getPumpTypeStr(price.pumpType));
    QStandardItem *squarePrice
            = new QStandardItem(QString("%1").arg(price.squarePrice));
    QStandardItem *standardPrice
            = new QStandardItem(QString("%1").arg(price.standardPrice));
    QStandardItem *remarks
            = new QStandardItem(price.remarks);

    QList<QStandardItem*> items;
    items << num << pumyType << squarePrice << standardPrice << remarks;
    mPriceModel->appendRow(items);
}

void
ContractWidget::updateContractItemSLot(Contract &contract)
{
    ALOGDTRACE();
    updateContractRow(contract);
    updateContractPriceTable(contract.prices);
}

void
ContractWidget::updateContractRow(Contract &contract)
{
    ALOGDTRACE();
    mContractModel->setData(mContractModel->index(curRow, 0),
                         contract.number);
    mContractModel->setData(mContractModel->index(curRow, 1),
                         contract.clientName);
    mContractModel->setData(mContractModel->index(curRow, 2),
                         contract.projectName);
    mContractModel->setData(mContractModel->index(curRow, 3),
                         contract.projectAddress);
    mContractModel->setData(mContractModel->index(curRow, 4),
                         QString("%1").arg(contract.deliverySizes));
    mContractModel->setData(mContractModel->index(curRow, 5),
                         QString("%1").arg(contract.structureLevel));
    mContractModel->setData(mContractModel->index(curRow, 6),
                         contract.beginDate.toString(DATE_FORMAT_STR));
    mContractModel->setData(mContractModel->index(curRow, 7),
                         contract.endDate.toString(DATE_FORMAT_STR));
    mContractModel->setData(mContractModel->index(curRow, 8),
                         contract.signedDate.toString(DATE_FORMAT_STR));
    mContractModel->setData(mContractModel->index(curRow, 9),
                         QString("%1").arg(contract.taxRate));
    mContractModel->setData(mContractModel->index(curRow, 10),
                         contract.isIncludeTax ? "是" : "否");
    mContractModel->setData(mContractModel->index(curRow, 11),
                         contract.requirement);
    mContractModel->setData(mContractModel->index(curRow, 12),
                         contract.supplement);
    mContractModel->setData(mContractModel->index(curRow, 13),
                         contract.remarks);
}

void
ContractWidget::updateContractPriceTable(QList<CONTRACT_PRICE> prices)
{
    if (mPriceModel->rowCount()) {
        // 删除所有行并清除item内存
        mPriceModel->removeRows(0, mPriceModel->rowCount());
    }

    for (int i = 0; i < prices.size(); i++) {
        addPriceTableRow(prices.at(i));
    }
}

void
ContractWidget::deleteContractItemSlot()
{
    if (curRow < 0) {
        QMessageBox::warning(this,
                             tr("温馨提示"),
                             tr("请选择要删除条目.\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    int ret = QMessageBox::warning(this,
                                   tr("温馨提示"),
                                   tr("确定要删除该条目吗？\n"),
                                   QMessageBox::Yes |
                                   QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    QString number = "";
    number = mContractModel->index(curRow, 0).data().toString();
    if (!mDb->deleteContractDataInNumber(number)) {
        ALOGD("%s, delete ok", __FUNCTION__);
        mContractModel->removeRow(curRow);
    }
}

void
ContractWidget::contractCurrentRowChangedSlot(const QModelIndex &current,const QModelIndex &prev)
{
    ALOGDTRACE();
    curRow = current.row();
    Contract contract;
    QString number = mContractModel->index(curRow, 0).data().toString();
    if (!mDb->getContractInNumber(number, contract)) {
        mDb->getAllContractPriceData(contract.number, contract.prices);
        updateContractPriceTable(contract.prices);
    }
}

void
ContractWidget::on_contractTableView_clicked(const QModelIndex &index)
{
    ALOGDTRACE();
    if (index.row() == curRow)
        return;

    curRow = index.row();
    Contract contract;
    QString number = mContractModel->index(curRow, 0).data().toString();
    if (!mDb->getContractInNumber(number, contract)) {
        mDb->getAllContractPriceData(contract.number, contract.prices);
        updateContractPriceTable(contract.prices);
    }
}

void 
ContractWidget::on_contractTableView_doubleClicked(const QModelIndex &index)
{
    ALOGDTRACE();
    editRowEvent(index.row());
}

void ContractWidget::on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    ALOGDTRACE();
    ALOGD("%s name=%s, number=%s, column=%d",
          __FUNCTION__, item->text(mClientNameColumn).toStdString().data(),
          item->text(mClientNumberColumn).toStdString().data(), column);

    QList<Contract> contracts;
    clearContractTable();
    clearPriceTable();
    if (item->parent() == NULL) {
        //根节点
        if (mDb->getAllContractData(contracts))
            return;
    } else {
        //子节点
        if (mDb->getContractInClientNumber(item->text(mClientNumberColumn),
                                           contracts))
            return;
    }

    addContractTableRows(contracts);
    if (mContractModel->rowCount() > 0) {
        //默认显示第一行的数据
        ui->contractTableView->selectRow(0);
        QString number = mContractModel->index(0, 0).data().toString();
        QList<CONTRACT_PRICE> prices;
        if (mDb->getAllContractPriceData(number, prices))
            return;
        addPriceTableRows(prices);
    }
}
