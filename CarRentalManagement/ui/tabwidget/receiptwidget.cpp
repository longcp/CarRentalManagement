#include "receiptwidget.h"
#include "ui_receiptwidget.h"
#include <QToolBar>
#include <receipteditdialog.h>
#include <datatype.h>
#include <tablemodel.h>
#include <QScrollBar>
#include <contract.h>
#include <QMessageBox>
#include <rentaldocument.h>
#include <database/database.h>
#include <cartabledialog.h>
#include <clienttabledialog.h>
#include <contracttabledialog.h>
#include <rentaldoctabledialog.h>
#include <car.h>
#include <client.h>
#include <QDebug>
#include <stdio.h>

#define LOG_TAG                         "RECEIPT_WIDGET"
#include "utils/Log.h"

ReceiptWidget::ReceiptWidget(QWidget *parent) :
    QWidget(parent),
    mDb(DataBase::getInstance()),
    mCarDialog(new CarTableDialog()),
    mClientDialog(new ClientTableDialog()),
    mContractDialog(new ContractTableDialog()),
    mRentalDocDialog(new RentalDocTableDialog()),
    ui(new Ui::ReceiptWidget)
{
    ui->setupUi(this);
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    ui->screeningBtn->setStyleSheet("background-color: rgb(234, 234, 234);");
    ui->clearBtn->setStyleSheet("background-color: rgb(234, 234, 234);");
    this->setWindowTitle(TAB_TITLE_RECEIVABLE);

    mReceiptEditDialog = new ReceiptEditDialog();

    mActSearch = new QAction(QIcon(":/menu/icon/search_64.ico"),
                             tr("查询"), this);
    mActExport = new QAction(QIcon(":/menu/icon/export_64.ico"),
                             tr("导出"), this);

    mToolBar = new QToolBar(tr("receiptToolBar"), this);
    configToolBar();
//    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);
    ui->toolBarVerticalLayout->addWidget(mToolBar);

    initView();

    /**
     * @brief 单元格双击事件
     */
//    connect(ui->receiptTable, SIGNAL(cellDoubleClicked(int,int)),
//            this, SLOT(cellDoubleClickedSlot(int,int)));
    /**
     * @brief 单元格双击事件
     */
    connect(this, SIGNAL(openReceiptEditDialogSignal()),
            mReceiptEditDialog, SLOT(openReceiptEditDialogSlot()));

    /**
     * @brief 同步调整列宽
     */
    connect(ui->receiptTable->horizontalHeader(),&QHeaderView::sectionResized,
            this, &ReceiptWidget::updateReceiptSumTableSectionWidth);
    /**
     * @brief 根据进度条值同步列表位置
     */
    connect((QObject*)ui->receiptSumTable->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->receiptTable->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->receiptTable->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &ReceiptWidget::sectionClickedSlot);
    /**
     * @brief 选择car
     */
    connect(this, SIGNAL(openCarDialogSig()),
            mCarDialog, SLOT(openWindow()));
    /**
     * @brief 选择car
     */
    connect(mCarDialog, SIGNAL(selectedCar(QString)),
            this, SLOT(getCar(QString)));
    /**
     * @brief 选择contract
     */
    connect(this, SIGNAL(openContractDialogSig()),
            mContractDialog, SLOT(openWindow()));
    /**
     * @brief 选择contract
     */
    connect(mContractDialog, SIGNAL(selectedContract(QString)),
            this, SLOT(getContract(QString)));
    /**
     * @brief 选择client
     */
    connect(this, SIGNAL(openClientDialogSig()),
            mClientDialog, SLOT(openWindow()));
    /**
     * @brief 选择client
     */
    connect(mClientDialog, SIGNAL(selectedClientName(QString)),
            this, SLOT(getClientName(QString)));
    /**
     * @brief 选择doc
     */
    connect(this, SIGNAL(openRentalDocDialogSig()),
            mRentalDocDialog, SLOT(openWindow()));
    /**
     * @brief 选择doc
     */
    connect(mRentalDocDialog, SIGNAL(selectedDoc(QString)),
            this, SLOT(getDoc(QString)));
}

ReceiptWidget::~ReceiptWidget()
{
    delete ui;
}

void
ReceiptWidget::configToolBar()
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
ReceiptWidget::initView()
{
    setPumpTypeView();
    initChooseWidget();
    initReceiptTable();
    initReceiptSumTable();

    QList<RentalDocument> docs;
    if (!mDb->getAllRentalDocumentData(docs))
        reflashView(docs);
}

void
ReceiptWidget::initChooseWidget()
{
    QDate curDate = QDate::currentDate();
    int curDateNum = curDate.toString("d").toInt();
    QDate startDay = curDate.addDays(-curDateNum+1);
    QDate endDay = curDate.addMonths(1).addDays(-curDateNum);
    ui->fromDateEdit->setDate(startDay);
    ui->toDateEdit->setDate(endDay);
    ui->fromDateCb->setChecked(false);
    ui->toDateCb->setChecked(false);
    ui->fromDateEdit->setEnabled(false);
    ui->toDateEdit->setEnabled(false);
    ui->receivableCheckBox->setChecked(true);
    ui->totalRadioButton->setChecked(true);
    ui->pumpTypeComboBox->setCurrentIndex(0);
    ui->carNumEt->setText("");
    ui->docNumEt->setText("");
    ui->contractNumEt->setText("");
    ui->clientNameEt->setText("");
}

void
ReceiptWidget::setPumpTypeView()
{
    ui->pumpTypeComboBox->insertItem(0, "");
    ui->pumpTypeComboBox->insertItem(1, PUMP_TYPE_37M1_STR);
    ui->pumpTypeComboBox->insertItem(2, PUMP_TYPE_48M_STR);
    ui->pumpTypeComboBox->insertItem(3, PUMP_TYPE_52M_STR);
    ui->pumpTypeComboBox->insertItem(4, PUMP_TYPE_56M_STR);
    ui->pumpTypeComboBox->insertItem(5, PUMP_TYPE_60M_STR);
    ui->pumpTypeComboBox->insertItem(6, PUMP_TYPE_CAR_PUMP_STR);
}

void
ReceiptWidget::initReceiptTable()
{
    //设置首行标题
    QStringList headerList;
    headerList << "日期" << "签证单号" << "合同号" << "客户名称"
               << "车号" << "泵式" << "方量单价/方"
               << "台班单价/小时" << "泵送方量" << "泵送台班"
               << "总金额" << "已收金额" << "应收金额"
               << "工程名称" << "工程地址" << "施工部位"
               << "联系人" << "联系电话" << "备注" ;

    mReceiptModel = new TableModel(0, headerList.size());
    ui->receiptTable->setModel(mReceiptModel);
    mReceiptModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->receiptTable->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->receiptTable->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->receiptTable->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->receiptTable->verticalHeader()->setVisible(false);              //隐藏行表头
    ui->receiptTable->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->receiptTable->setAlternatingRowColors(true);
    ui->receiptTable->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    //隐藏滚动条
    ui->receiptTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->receiptTable->setSortingEnabled(true);                          //点击表头排序

    ui->receiptTable->setColumnWidth(COL_DOC_NUMBER, 200);
    ui->receiptTable->setColumnWidth(COL_CONTRACT_NUMBER, 200);
    ui->receiptTable->setColumnWidth(COL_CLIENT_NAME, 200);
    ui->receiptTable->setColumnWidth(COL_CAR_PLATE_NUMBER, 200);
}

void
ReceiptWidget::initReceiptSumTable()
{
    //设置首行标题
    QStringList headerList;
    headerList << "日期" << "签证单号" << "合同号" << "客户名称"
               << "车号" << "泵式" << "方量单价/方"
               << "台班单价/小时" << "泵送方量" << "泵送台班"
               << "总金额" << "已收金额" << "应收金额"
               << "工程名称" << "工程地址" << "施工部位"
               << "联系人" << "联系电话" << "备注" ;

    ui->receiptSumTable->verticalHeader()->setDefaultSectionSize(20);
    mReceiptSumModel = new TableModel(0, headerList.size());
    ui->receiptSumTable->setModel(mReceiptSumModel);
    mReceiptSumModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->receiptSumTable->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->receiptSumTable->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->receiptSumTable->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->receiptSumTable->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->receiptSumTable->horizontalHeader()->setVisible(false);         //隐藏列表头
    ui->receiptSumTable->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->receiptSumTable->setAlternatingRowColors(true);
    ui->receiptSumTable->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->receiptSumTable->setColumnWidth(COL_DOC_NUMBER, 200);
    ui->receiptSumTable->setColumnWidth(COL_CONTRACT_NUMBER, 200);
    ui->receiptSumTable->setColumnWidth(COL_CLIENT_NAME, 200);
    ui->receiptSumTable->setColumnWidth(COL_CAR_PLATE_NUMBER, 200);

    QStandardItem* sumStrItem = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sumStrItem;
    mReceiptSumModel->appendRow(items);
    resetReceiptSumTableData();
}

void
ReceiptWidget::sectionClickedSlot(int index)
{
    ALOGD("index = %d", index);
//    mReceiptModel->sort(index, Qt::AscendingOrder);
}

void
ReceiptWidget::updateReceiptSumTableSectionWidth(int logicalIndex,
                                                 int /*oldSize*/,
                                                 int newSize)
{
    ui->receiptSumTable->setColumnWidth(logicalIndex, newSize);
}

void
ReceiptWidget::cellDoubleClickedSlot(int a,int b)
{
    ALOGD("%s, a = %d, b = %d", __FUNCTION__, a, b);
    emit openReceiptEditDialogSignal();
}

void
ReceiptWidget::setPumpSquareCellValue(double value)
{
    char buf[64];
    sprintf(buf, "%0.2lf", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_PUMP_SQUARE),
                             QString(buf));
    curPumpSquareValue = value;
}

void
ReceiptWidget::setPumpTimeCellValue(double value)
{
    char buf[64];
    sprintf(buf, "%0.2lf", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_PUMP_TIME),
                             QString(buf));
    curPumpTimeValue = value;
}

void
ReceiptWidget::setProjectAmountCellValue(double value)
{
    char buf[64];
    sprintf(buf, "%0.2lf", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_PROJECT_AMOUNT),
                             QString(buf));
    curProjectAmountValue = value;
}

void
ReceiptWidget::setReceiptCellValue(double value)
{
    char buf[64];
    sprintf(buf, "%0.2lf", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_RECEIPT),
                             QString(buf));
    curReceiptValue = value;
}

void
ReceiptWidget::setReceivableValue(double value)
{
    char buf[64];
    sprintf(buf, "%0.2lf", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_RECEIVABLE),
                             QString(buf));
    curReceivableValue = value;
}

void
ReceiptWidget::resetReceiptSumTableData()
{
    setPumpSquareCellValue(0);
    setPumpTimeCellValue(0);
    setProjectAmountCellValue(0);
    setReceiptCellValue(0);
    setReceivableValue(0);
}

void
ReceiptWidget::addTableRow(RentalDocument &doc)
{
    char buf[64];

    QStandardItem *date = new QStandardItem(doc.date.toString(DATE_FORMAT_STR));
    QStandardItem *docNum = new QStandardItem(doc.number);
    QStandardItem *contractNumber = new QStandardItem(doc.contractNumber);
    QStandardItem *clientName = new QStandardItem(doc.clientName);
    QStandardItem *carPlateNumber = new QStandardItem(doc.carPlateNumber);
    QStandardItem *pumpType = new QStandardItem(QString("%1").arg(doc.pumpType));
    QStandardItem *projectName = new QStandardItem(doc.projectName);
    QStandardItem *projectAddr = new QStandardItem(doc.projectAddress);
    QStandardItem *constructPlace = new QStandardItem(doc.constructPlace);
    QStandardItem *principal = new QStandardItem(doc.principal);
    QStandardItem *principalTel = new QStandardItem(doc.principalTel);
    QStandardItem *remarks = new QStandardItem(doc.remarks);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", doc.squareUnitPrice);
    QStandardItem *squareUnitPrice = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", doc.pumpTimeUnitPrice);
    QStandardItem *pumpTimeUnitPrice = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", doc.pumpSquare);
    QStandardItem *pumpSquare = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", doc.pumpTimes);
    QStandardItem *pumpTimes = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", doc.projectAmount);
    QStandardItem *projectAmounts =  new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", doc.receivedAccounts);
    QStandardItem *receivedAccounts = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", doc.projectAmount - doc.receivedAccounts);
    QStandardItem *receivable = new QStandardItem(QString(buf));

    QList<QStandardItem*> items;
    items << date << docNum << contractNumber << clientName << carPlateNumber
          << pumpType << squareUnitPrice << pumpTimeUnitPrice
          << pumpSquare << pumpTimes << projectAmounts << receivedAccounts
          << receivable << projectName << projectAddr << constructPlace
          << principal << principalTel << remarks;
    mReceiptModel->appendRow(items);
}

void
ReceiptWidget::pumpSquareCellAddValue(double value)
{
    setPumpSquareCellValue(curPumpSquareValue+value);
}

void
ReceiptWidget::pumpTimeCellAddValue(double value)
{
    setPumpTimeCellValue(curPumpTimeValue+value);
}

void
ReceiptWidget::projectAmountCellAddValue(double value)
{
    setProjectAmountCellValue(curProjectAmountValue+value);
}

void
ReceiptWidget::receiptCellAddValue(double value)
{
    setReceiptCellValue(curReceiptValue+value);
}

void
ReceiptWidget::receivableCellAddValue(double value)
{
    setReceivableValue(curReceivableValue+value);
}

void
ReceiptWidget::pumpSquareCellDelValue(double value)
{
    setPumpSquareCellValue(curPumpSquareValue-value);
}

void
ReceiptWidget::pumpTimeCellDelValue(double value)
{
    setPumpTimeCellValue(curPumpTimeValue-value);
}

void
ReceiptWidget::projectAmountCellDelValue(double value)
{
    setProjectAmountCellValue(curProjectAmountValue-value);
}

void
ReceiptWidget::receiptCellDelValue(double value)
{
    setReceiptCellValue(curReceiptValue-value);
}

void
ReceiptWidget::receivableCellDelValue(double value)
{
    setReceivableValue(curReceivableValue-value);
}

void
ReceiptWidget::deleteRentalDocumentSlot(QString docNum)
{
    ALOGDTRACE();
    RentalDocument doc;
    for (int i = 0; i < mReceiptModel->rowCount(); i++) {
        if (docNum != mReceiptModel->index(i, COL_DOC_NUMBER).data().toString())
            continue;
        if (!mDb->getRentalDocumentDataInNumber(docNum, doc))
            delRowAndUpdateSumTable(i, doc);
    }
}

void
ReceiptWidget::delRowAndUpdateSumTable(int row, RentalDocument &doc)
{
    pumpSquareCellDelValue(doc.pumpSquare);
    pumpTimeCellDelValue(doc.pumpTimes);
    projectAmountCellDelValue(doc.projectAmount);
    receiptCellDelValue(doc.receivedAccounts);
    receivableCellDelValue(doc.projectAmount-doc.receivedAccounts);
    mReceiptModel->removeRow(row);
}

void
ReceiptWidget::addRentalDocumentSlot(RentalDocument &doc)
{
    addRowAndUpdateSumTable(doc);
}

void
ReceiptWidget::addRowAndUpdateSumTable(RentalDocument &doc)
{
    addTableRow(doc);
    pumpSquareCellAddValue(doc.pumpSquare);
    pumpTimeCellAddValue(doc.pumpTimes);
    projectAmountCellAddValue(doc.projectAmount);
    receiptCellAddValue(doc.receivedAccounts);
    receivableCellAddValue(doc.projectAmount-doc.receivedAccounts);
}

void
ReceiptWidget::addTableRows(QList<RentalDocument> &docs)
{
    RentalDocument doc;
    for (int i = 0; i < docs.size(); i++) {
        doc = docs.at(i);
        addRowAndUpdateSumTable(doc);
    }
}

void
ReceiptWidget::clearTableview()
{
    if (mReceiptModel->rowCount() > 0)
        mReceiptModel->removeRows(0, mReceiptModel->rowCount());

    resetReceiptSumTableData();
}

void
ReceiptWidget::reflashView(QList<RentalDocument> docs)
{
    clearTableview();
    addTableRows(docs);
}

void
ReceiptWidget::tabChangeToReceiptSlot(int index, QString tabText)
{
    ALOGDTRACE();
#if 0
    QList<RentalDocument> docs;

    if (tabText != TAB_TITLE_RECEIPT ||
            mDb->getAllRentalDocumentData(docs))
        return;

    reflashView(docs);
#endif
}

RECEIPT_FILTER
ReceiptWidget::getFilter()
{
    RECEIPT_FILTER filter;
    if (ui->fromDateCb->isChecked())
        filter.fromDate = QDate::fromString(ui->fromDateEdit->text(), DATE_FORMAT_STR);
    if (ui->toDateCb->isChecked())
        filter.toDate = QDate::fromString(ui->toDateEdit->text(), DATE_FORMAT_STR);

    if (ui->contractRadioButton->isChecked())
        filter.clientType = ClientType::CONTRACT;
    else if (ui->tempRadioButton->isChecked())
        filter.clientType = ClientType::TEMPORARY;

    if (ui->pumpTypeComboBox->currentText() != NULL)
        filter.pumpType = Car::getPumpType(ui->pumpTypeComboBox->currentText());
    else
        filter.pumpType = PumpType::UNKNOWN_PUMPTYPE;

    if (ui->receivableCheckBox->isChecked())
        filter.isAccountPositive = true;
    else
        filter.isAccountPositive = false;

    filter.carNumber = ui->carNumEt->text();
    filter.clientName = ui->clientNameEt->text();
    filter.contractNumber = ui->contractNumEt->text();
    filter.rentalDocNumber = ui->docNumEt->text();

//    ALOGDTRACE();
//    ALOGD("fromDate = %s", filter.fromDate.toString(DATE_FORMAT_STR).toStdString().data());
//    ALOGD("toDate = %s", filter.toDate.toString(DATE_FORMAT_STR).toStdString().data());
//    ALOGD("clientType = %d",int(filter.clientType));
//    ALOGD("pumpType = %d", int(filter.pumpType));
//    qDebug()<< "isAccountPositive = " << filter.isAccountPositive;
//    ALOGD("carNumber = %s", filter.carNumber.toStdString().data());
//    ALOGD("clientName = %s", filter.clientName.toStdString().data());
//    ALOGD("contractNumber = %s", filter.contractNumber.toStdString().data());
//    ALOGD("rentalDocNumber = %s", filter.rentalDocNumber.toStdString().data());

    return filter;
}

void
ReceiptWidget::on_screeningBtn_clicked()
{
    int ret;
    Client client;
    RentalDocument doc;
    QList<RentalDocument> docs;
    RECEIPT_FILTER filter = getFilter();

    if (!filter.isAccountPositive &&
            filter.fromDate.toString(DATETIME_FORMAT_STR) == NULL &&
            filter.toDate.toString(DATETIME_FORMAT_STR) == NULL &&
            filter.rentalDocNumber == NULL &&
            filter.contractNumber == NULL &&
            filter.clientName == NULL &&
            filter.pumpType == PumpType::UNKNOWN_PUMPTYPE &&
            filter.carNumber == NULL) {
        ret = mDb->getAllRentalDocumentData(docs);
    } else {
        ret = mDb->getRentalDocInFilter(filter, docs);
    }
    if (ret) {
        QMessageBox::warning(this, tr("温馨提示"),
                                       tr("未知错误.\n"),
                                       QMessageBox::Ok);
        return;
    }

    ui->screeningBtn->setStyleSheet("background-color: rgb(70, 130, 180);");
    if (!ui->totalRadioButton->isChecked()) {
        int size = docs.size();
        for (int i = 0; i < size; i++) {
            doc = docs.at(i);
            if (!mDb->getClientInNumber(doc.clientNumber, client)) {
                if (client.clienttype != filter.clientType) {
                    docs.removeAt(i);
                    size--;
                    i--;
                }
            }
        }
    }

    reflashView(docs);
}

void
ReceiptWidget::on_clearBtn_clicked()
{
    QList<RentalDocument> docs;
    ui->screeningBtn->setStyleSheet("background-color: rgb(234, 234, 234);");
    initChooseWidget();
    if (!mDb->getAllRentalDocumentData(docs))
        reflashView(docs);
}

void ReceiptWidget::on_fromDateCb_toggled(bool checked)
{
    ui->fromDateEdit->setEnabled(checked);
}

void ReceiptWidget::on_toDateCb_toggled(bool checked)
{
    ui->toDateEdit->setEnabled(checked);
}

void
ReceiptWidget::getContract(QString number)
{
    ui->contractNumEt->setText(number);
}

void
ReceiptWidget::getCar(QString number)
{
    ui->carNumEt->setText(number);
}

void
ReceiptWidget::getClientName(QString name)
{
    ui->clientNameEt->setText(name);
}

void
ReceiptWidget::getDoc(QString number)
{
    ui->docNumEt->setText(number);
}

void ReceiptWidget::on_docNumTb_clicked()
{
    emit openRentalDocDialogSig();
}

void ReceiptWidget::on_contractNumTb_clicked()
{
    emit openContractDialogSig();
}

void ReceiptWidget::on_clientNameTb_clicked()
{
    emit openClientDialogSig();
}

void ReceiptWidget::on_carNumTb_clicked()
{
    emit openCarDialogSig();
}
