#include "receiptwidget.h"
#include "ui_receiptwidget.h"
#include <QToolBar>
#include <receipteditdialog.h>
#include <datatype.h>
#include <tablemodel.h>
#include <QScrollBar>
#include <contract.h>
#include <rentaldocument.h>
#include <database/database.h>

#define LOG_TAG                         "RECEIPT_WIDGET"
#include "utils/Log.h"

ReceiptWidget::ReceiptWidget(QWidget *parent) :
    QWidget(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::ReceiptWidget)
{
    ui->setupUi(this);
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle(TAB_TITLE_RECEIVABLE);

    mReceiptEditDialog = new ReceiptEditDialog();

    mActSearch = new QAction(QIcon(":/menu/icon/search_64.ico"),
                             tr("查询"), this);
    mActExport = new QAction(QIcon(":/menu/icon/export_64.ico"),
                             tr("导出"), this);

    mToolBar = new QToolBar(tr("receiptToolBar"), this);
    configToolBar();
    mToolBar->addAction(mActSearch);
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
    initReceiptTable();
    initReceiptSumTable();

    QList<RentalDocument> docs;
    if (!mDb->getAllRentalDocumentData(docs))
        reflashView(docs);
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
ReceiptWidget::setPumpSquareCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_PUMP_SQUARE),
                             QString(buf));
}

void
ReceiptWidget::setPumpTimeCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_PUMP_TIME),
                             QString(buf));
}

void
ReceiptWidget::setProjectAmountCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_PROJECT_AMOUNT),
                             QString(buf));
}

void
ReceiptWidget::setReceiptCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_RECEIPT),
                             QString(buf));
}

void
ReceiptWidget::setReceivableValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mReceiptSumModel->setData(mReceiptSumModel->index(0, COL_RECEIVABLE),
                             QString(buf));
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
    sprintf(buf, "%0.2f", doc.squareUnitPrice);
    QStandardItem *squareUnitPrice = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.pumpTimeUnitPrice);
    QStandardItem *pumpTimeUnitPrice = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.pumpSquare);
    QStandardItem *pumpSquare = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.pumpTimes);
    QStandardItem *pumpTimes = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.projectAmount);
    QStandardItem *projectAmounts =  new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.receivedAccounts);
    QStandardItem *receivedAccounts = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.projectAmount - doc.receivedAccounts);
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
ReceiptWidget::pumpSquareCellAddValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_PUMP_SQUARE).data().toFloat();
    setPumpSquareCellValue(curValue+value);
}

void
ReceiptWidget::pumpTimeCellAddValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_PUMP_TIME).data().toFloat();
    setPumpTimeCellValue(curValue+value);
}

void
ReceiptWidget::projectAmountCellAddValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_PROJECT_AMOUNT).data().toFloat();
    setProjectAmountCellValue(curValue+value);
}

void
ReceiptWidget::receiptCellAddValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_RECEIPT).data().toFloat();
    setReceiptCellValue(curValue+value);
}

void
ReceiptWidget::receivableCellAddValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_RECEIVABLE).data().toFloat();
    setReceivableValue(curValue+value);
}

void
ReceiptWidget::pumpSquareCellDelValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_PUMP_SQUARE).data().toFloat();
    setPumpSquareCellValue(curValue-value);
}

void
ReceiptWidget::pumpTimeCellDelValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_PUMP_TIME).data().toFloat();
    setPumpTimeCellValue(curValue-value);
}

void
ReceiptWidget::projectAmountCellDelValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_PROJECT_AMOUNT).data().toFloat();
    setProjectAmountCellValue(curValue-value);
}

void
ReceiptWidget::receiptCellDelValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_RECEIPT).data().toFloat();
    setReceiptCellValue(curValue-value);
}

void
ReceiptWidget::receivableCellDelValue(float value)
{
    float curValue = mReceiptSumModel->index(0, COL_RECEIVABLE).data().toFloat();
    setReceivableValue(curValue-value);
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
    if (mReceiptModel->rowCount())
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
