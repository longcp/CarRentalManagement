#include "receivablewidget.h"
#include "ui_receivablewidget.h"
#include <QToolBar>
#include <tablemodel.h>
#include <QScrollBar>
#include <contract.h>
#include <rentaldocument.h>
#include <database/database.h>
#include <stdio.h>

#define LOG_TAG                         "RECEIVABLE_WIDGET"
#include "utils/Log.h"

// FIXME: 统计有误待修正

ReceivableWidget::ReceivableWidget(QWidget *parent) :
    QWidget(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::ReceivableWidget)
{
    ui->setupUi(this);
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    ui->screeningBtn->setStyleSheet("background-color: rgb(234, 234, 234);");
    ui->clearBtn->setStyleSheet("background-color: rgb(234, 234, 234);");
    this->setWindowTitle(TAB_TITLE_RECEIVABLE);

    mActSearch = new QAction(QIcon(":/menu/icon/search_64.ico"),
                             tr("查询"), this);
    mActExport = new QAction(QIcon(":/menu/icon/export_64.ico"),
                             tr("导出"), this);

    mToolBar = new QToolBar(tr("receivableToolBar"), this);
    configToolBar();
//    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);
    ui->toolBarVerticalLayout->addWidget(mToolBar);

    initView();

    /**
     * @brief 同步调整列宽
     */
    connect(ui->detailTableview->horizontalHeader(),&QHeaderView::sectionResized,
            this, &ReceivableWidget::updateDetailSumTableviewSectionWidth);

    connect(ui->totalTableview->horizontalHeader(),&QHeaderView::sectionResized,
            this, &ReceivableWidget::updateTotalSumTableviewSectionWidth);
    /**
     * @brief 根据进度条值同步列表位置
     */
    connect((QObject*)ui->detailSummaryTablview->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->detailTableview->horizontalScrollBar(), SLOT(setValue(int)));

    connect((QObject*)ui->totalSummaryTableview->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->totalTableview->horizontalScrollBar(), SLOT(setValue(int)));
}

ReceivableWidget::~ReceivableWidget()
{
    delete ui;
}

void
ReceivableWidget::configToolBar()
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
ReceivableWidget::initView()
{
    initChooseWidget();
    initDetailTableview();
    initDetailSumTableview();
    initTotalTableview();
    initTotalSumTableview();
}

void
ReceivableWidget::initChooseWidget()
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
}

void
ReceivableWidget::initDetailTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "日期" << "签证单号" << "合同号" << "客户名称"
               << "车号" << "泵式" << "混凝土标号"
               << "方量单价/方" << "台班单价/小时" << "泵送方量"
               << "泵送台班" /*<< "泵送台班时间"*/ << "总金额"
               << "已收金额" << "应收金额" << "工程名称"
               << "工程地址" << "施工部位" << "联系人"
               << "联系电话" << "备注" ;

    mDetailModel = new TableModel(0, headerList.size());
    ui->detailTableview->setModel(mDetailModel);
    mDetailModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->detailTableview->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->detailTableview->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->detailTableview->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->detailTableview->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->detailTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->detailTableview->setAlternatingRowColors(true);
    ui->detailTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    //隐藏滚动条
    ui->detailTableview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->detailTableview->setColumnWidth(DETAIL_COL_DOC_NUMBER, 200);
    ui->detailTableview->setColumnWidth(DETAIL_COL_CONTRACT_NUMBER, 200);
    ui->detailTableview->setColumnWidth(DETAIL_COL_CLIENT_NAME, 200);
    ui->detailTableview->setColumnWidth(DETAIL_COL_CAR_PLATE_NUMBER, 200);
    ui->detailTableview->setSortingEnabled(true);
}

void
ReceivableWidget::initDetailSumTableview()
{
    //设置首行标题
    // TODO:确定好台班单价、台班数、泵送台班时间的关系
    QStringList headerList;
    headerList << "日期" << "签证单号" << "合同号" << "客户名称"
               << "车号" << "泵式" << "混凝土标号"
               << "方量单价/方" << "台班单价/小时" << "泵送方量"
               << "泵送台班" /*<< "泵送台班时间"*/ << "总金额"
               << "已收金额" << "应收金额" << "工程名称"
               << "工程地址" << "施工部位" << "联系人"
               << "联系电话" << "备注" ;

    ui->detailSummaryTablview->verticalHeader()->setDefaultSectionSize(20);
    mDetailSumModel = new TableModel(0, headerList.size());
    ui->detailSummaryTablview->setModel(mDetailSumModel);
    mDetailSumModel->setHorizontalHeaderLabels(headerList);
    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->detailSummaryTablview->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->detailSummaryTablview->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->detailSummaryTablview->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->detailSummaryTablview->verticalHeader()->setVisible(false);     //隐藏行表头
    ui->detailSummaryTablview->horizontalHeader()->setVisible(false);   //隐藏列表头
    ui->detailSummaryTablview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->detailSummaryTablview->setAlternatingRowColors(true);
    ui->detailSummaryTablview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->detailSummaryTablview->setColumnWidth(DETAIL_COL_DOC_NUMBER, 200);
    ui->detailSummaryTablview->setColumnWidth(DETAIL_COL_CONTRACT_NUMBER, 200);
    ui->detailSummaryTablview->setColumnWidth(DETAIL_COL_CLIENT_NAME, 200);
    ui->detailSummaryTablview->setColumnWidth(DETAIL_COL_CAR_PLATE_NUMBER, 200);

    QStandardItem* sumStrItem = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sumStrItem;
    mDetailSumModel->appendRow(items);
    resetDetailSumTableData();
}

void
ReceivableWidget::initTotalTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "合同号" << "客户名称" << "泵送方量" << "泵送台班"
               << "总金额" << "已收金额" << "应收金额";

    mTotalModel = new TableModel(0, headerList.size());
    ui->totalTableview->setModel(mTotalModel);
    mTotalModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->totalTableview->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->totalTableview->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->totalTableview->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->totalTableview->verticalHeader()->setVisible(false);            //隐藏行表头
    ui->totalTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->totalTableview->setAlternatingRowColors(true);
    ui->totalTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    //最后一列自适应
    ui->totalTableview->horizontalHeader()
            ->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);

    ui->totalTableview->setColumnWidth(SUM_COL_CONTRACT_NUMBER, 180);
    ui->totalTableview->setColumnWidth(SUM_COL_CLIENT_NAME, 180);
    ui->totalTableview->setColumnWidth(SUM_COL_PUMP_SQUARE, 180);
    ui->totalTableview->setColumnWidth(SUM_COL_PUMP_TIME, 180);
    ui->totalTableview->setColumnWidth(SUM_COL_PROJECT_AMOUNT, 180);
    ui->totalTableview->setColumnWidth(SUM_COL_RECEIPT, 180);
    ui->totalTableview->setColumnWidth(SUM_COL_RECEIVABLE, 180);

    //隐藏滚动条
    ui->totalTableview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->totalTableview->setSortingEnabled(true);                        //点击表头排序
}

void
ReceivableWidget::initTotalSumTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "合同号" << "客户名称" << "泵送方量" << "泵送台班"
               << "总金额" << "已收金额" << "应收金额";

    ui->totalSummaryTableview->verticalHeader()->setDefaultSectionSize(20);
    mTotalSumModel = new TableModel(0, headerList.size());
    ui->totalSummaryTableview->setModel(mTotalSumModel);
    mTotalSumModel->setHorizontalHeaderLabels(headerList);
    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->totalSummaryTableview->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->totalSummaryTableview->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->totalSummaryTableview->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->totalSummaryTableview->verticalHeader()->setVisible(false);     //隐藏行表头
    ui->totalSummaryTableview->horizontalHeader()->setVisible(false);   //隐藏列表头
    ui->totalSummaryTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->totalSummaryTableview->setAlternatingRowColors(true);
    ui->totalSummaryTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->totalSummaryTableview->setColumnWidth(SUM_COL_CONTRACT_NUMBER, 180);
    ui->totalSummaryTableview->setColumnWidth(SUM_COL_CLIENT_NAME, 180);
    ui->totalSummaryTableview->setColumnWidth(SUM_COL_PUMP_SQUARE, 180);
    ui->totalSummaryTableview->setColumnWidth(SUM_COL_PUMP_TIME, 180);
    ui->totalSummaryTableview->setColumnWidth(SUM_COL_PROJECT_AMOUNT, 180);
    ui->totalSummaryTableview->setColumnWidth(SUM_COL_RECEIPT, 180);
    ui->totalSummaryTableview->setColumnWidth(SUM_COL_RECEIVABLE, 180);

    //最后一列自适应
    ui->totalSummaryTableview->horizontalHeader()
            ->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);

    QStandardItem* sumStrItem = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sumStrItem;
    mTotalSumModel->appendRow(items);
    // FIXME:update_column_data(column, data);此处data为0
}

void
ReceivableWidget::resetDetailSumTableData()
{
    setDetailPumpSquareCellValue(0);
    setDetailPumpTimeCellValue(0);
    setDetailProjectAmountCellValue(0);
    setDetailReceiptCellValue(0);
    setDetailReceivableValue(0);
}

void
ReceivableWidget::resetTotalSumTableData()
{
    setSumPumpSquareCellValue(0);
    setSumPumpTimeCellValue(0);
    setSumProjectAmountCellValue(0);
    setSumReceiptCellValue(0);
    setSumReceivableValue(0);
}

void
ReceivableWidget::setDetailPumpSquareCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mDetailSumModel->setData(mDetailSumModel->index(0, DETAIL_COL_PUMP_SQUARE),
                             QString(buf));
}

void
ReceivableWidget::setDetailPumpTimeCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mDetailSumModel->setData(mDetailSumModel->index(0, DETAIL_COL_PUMP_TIME),
                             QString(buf));
}

void
ReceivableWidget::setDetailProjectAmountCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mDetailSumModel->setData(mDetailSumModel->index(0, DETAIL_COL_PROJECT_AMOUNT),
                             QString(buf));
}

void
ReceivableWidget::setDetailReceiptCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mDetailSumModel->setData(mDetailSumModel->index(0, DETAIL_COL_RECEIPT),
                             QString(buf));
}

void
ReceivableWidget::setDetailReceivableValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mDetailSumModel->setData(mDetailSumModel->index(0, DETAIL_COL_RECEIVABLE),
                             QString(buf));
}

void
ReceivableWidget::setSumReceivableValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mTotalSumModel->setData(mTotalSumModel->index(0, SUM_COL_RECEIVABLE),
                             QString(buf));
}


void
ReceivableWidget::setSumPumpSquareCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mTotalSumModel->setData(mTotalSumModel->index(0, SUM_COL_PUMP_SQUARE),
                             QString(buf));
}

void
ReceivableWidget::setSumPumpTimeCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mTotalSumModel->setData(mTotalSumModel->index(0, SUM_COL_PUMP_TIME),
                             QString(buf));
}

void
ReceivableWidget::setSumProjectAmountCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mTotalSumModel->setData(mTotalSumModel->index(0, SUM_COL_PROJECT_AMOUNT),
                             QString(buf));
}

void
ReceivableWidget::setSumReceiptCellValue(float value)
{
    char buf[64];
    sprintf(buf, "%0.2f", value);
    mTotalSumModel->setData(mTotalSumModel->index(0, SUM_COL_RECEIPT),
                             QString(buf));
}

void
ReceivableWidget::detailPumpSquareCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, DETAIL_COL_PUMP_SQUARE).data().toFloat();
    setDetailPumpSquareCellValue(curValue+value);
}

void
ReceivableWidget::detailPumpTimeCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, DETAIL_COL_PUMP_TIME).data().toFloat();
    setDetailPumpTimeCellValue(curValue+value);
}

void
ReceivableWidget::detailProjectAmountCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, DETAIL_COL_PROJECT_AMOUNT).data().toFloat();
    setDetailProjectAmountCellValue(curValue+value);
}

void
ReceivableWidget::detailReceiptCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, DETAIL_COL_RECEIPT).data().toFloat();
    setDetailReceiptCellValue(curValue+value);
}

void
ReceivableWidget::detailReceivableCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, DETAIL_COL_RECEIVABLE).data().toFloat();
    setDetailReceivableValue(curValue+value);
}

void
ReceivableWidget::sumPumpSquareCellAddValue(float value)
{
    float curValue = mTotalSumModel->index(0, SUM_COL_PUMP_SQUARE).data().toFloat();
    setSumPumpSquareCellValue(curValue+value);
}

void
ReceivableWidget::sumPumpTimeCellAddValue(float value)
{
    float curValue = mTotalSumModel->index(0, SUM_COL_PUMP_TIME).data().toFloat();
    setSumPumpTimeCellValue(curValue+value);
}

void
ReceivableWidget::sumProjectAmountCellAddValue(float value)
{
    float curValue = mTotalSumModel->index(0, SUM_COL_PROJECT_AMOUNT).data().toFloat();
    setSumProjectAmountCellValue(curValue+value);
}

void
ReceivableWidget::sumReceiptCellAddValue(float value)
{
    float curValue = mTotalSumModel->index(0, SUM_COL_RECEIPT).data().toFloat();
    setSumReceiptCellValue(curValue+value);
}

void
ReceivableWidget::sumReceivableCellAddValue(float value)
{
    float curValue = mTotalSumModel->index(0, SUM_COL_RECEIVABLE).data().toFloat();
    setSumReceivableValue(curValue+value);
}

void
ReceivableWidget::deleteRentalDocumentSlot(QString docNum)
{

}

void
ReceivableWidget::addRentalDocumentSlot(RentalDocument &doc)
{

}

void
ReceivableWidget::clearDetailTableview()
{
    if (mDetailModel->rowCount() > 0)
        mDetailModel->removeRows(0, mDetailModel->rowCount());

    resetDetailSumTableData();
}

void
ReceivableWidget::clearSumTableview()
{
    if (mTotalModel->rowCount())
        mTotalModel->removeRows(0, mTotalModel->rowCount());

    resetTotalSumTableData();
}

void
ReceivableWidget::reflashDetailTableview(QList<RentalDocument> &docs)
{
    clearDetailTableview();
    addDetailTableRows(docs);
}

void
ReceivableWidget::reflashSumTableview(QList<RentalDocument> &docs)
{
    clearSumTableview();
    addSumTableRows(docs);
}


void
ReceivableWidget::addDetailTableRows(QList<RentalDocument> &docs)
{
    RentalDocument doc;
    for (int i = 0; i < docs.size(); i++) {
        doc = docs.at(i);
        addRowAndUpdateDetailSumTable(doc);
    }
}

void
ReceivableWidget::addRowAndUpdateDetailSumTable(RentalDocument &doc)
{
    addDetailTableRow(doc);
    detailPumpSquareCellAddValue(doc.pumpSquare);
    detailPumpTimeCellAddValue(doc.pumpTimes);
    detailProjectAmountCellAddValue(doc.projectAmount);
    detailReceiptCellAddValue(doc.receivedAccounts);
    detailReceivableCellAddValue(doc.projectAmount-doc.receivedAccounts);
}

void
ReceivableWidget::addSumTableRows(QList<RentalDocument> &docs)
{
    RentalDocument doc1, doc2;
    QList<RentalDocument> docsTemp;
    while (docs.size() > 0) {
        doc1 = docs.takeFirst();
        for (int i = 0; i < docs.size(); i++) {
            if (doc1.contractNumber == docs.at(i).contractNumber) {
                docsTemp.push_back(docs.takeAt(i--));
            }
        }

        for (int i = 0; i < docsTemp.size(); i++) {
            doc1.pumpSquare += docsTemp.at(i).pumpSquare;
            doc1.pumpTimes += docsTemp.at(i).pumpTimes;
            doc1.projectAmount += docsTemp.at(i).projectAmount;
            doc1.receivedAccounts += docsTemp.at(i).receivedAccounts;
        }
        sumPumpSquareCellAddValue(doc1.pumpSquare);
        sumPumpTimeCellAddValue(doc1.pumpTimes);
        sumProjectAmountCellAddValue(doc1.projectAmount);
        sumReceiptCellAddValue(doc1.receivedAccounts);
        sumReceivableCellAddValue(doc1.projectAmount-doc1.receivedAccounts);
        addSumTableRow(doc1);
        docsTemp.clear();
    }
}

void
ReceivableWidget::addDetailTableRow(RentalDocument &doc)
{
    char buf[64];
    ALOGDTRACE();

    QStandardItem *date = new QStandardItem(doc.date.toString(DATE_FORMAT_STR));
    QStandardItem *docNum = new QStandardItem(doc.number);
    QStandardItem *contractNumber = new QStandardItem(doc.contractNumber);
    QStandardItem *clientName = new QStandardItem(doc.clientName);
    QStandardItem *carPlateNumber = new QStandardItem(doc.carPlateNumber);
    QStandardItem *pumpType = new QStandardItem(QString("%1").arg(doc.pumpType));
    QStandardItem *concreteLable = new QStandardItem(doc.concreteLable);
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
          << pumpType << concreteLable << squareUnitPrice << pumpTimeUnitPrice
          << pumpSquare << pumpTimes << projectAmounts << receivedAccounts
          << receivable << projectName << projectAddr << constructPlace
          << principal << principalTel << remarks;
    mDetailModel->appendRow(items);
}

void
ReceivableWidget::addSumTableRow(RentalDocument &doc)
{
    char buf[64];

    QStandardItem *contractNumber = new QStandardItem(doc.contractNumber);
    QStandardItem *clientName = new QStandardItem(doc.clientName);

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
    items << contractNumber << clientName << pumpSquare << pumpTimes
          << projectAmounts << receivedAccounts << receivable;
    mTotalModel->appendRow(items);
}

void
ReceivableWidget::updateDetailSumTableviewSectionWidth(int logicalIndex,
                                                       int /*oldSize*/,
                                                       int newSize)
{
    ui->detailSummaryTablview->setColumnWidth(logicalIndex, newSize);
}

void
ReceivableWidget::updateTotalSumTableviewSectionWidth(int logicalIndex,
                                                      int /*oldSize*/,
                                                      int newSize)
{
    ui->totalSummaryTableview->setColumnWidth(logicalIndex, newSize);
}

void
ReceivableWidget::tabChangeToReceivableSlot(int index, QString tabText)
{
    ALOGDTRACE();
    QList<RentalDocument> docs;

    if (tabText != TAB_TITLE_RECEIVABLE ||
            mDb->getAllRentalDocumentData(docs))
        return;

    reflashDetailTableview(docs);
    reflashSumTableview(docs);
}

void
ReceivableWidget::on_screeningBtn_clicked()
{
    ui->screeningBtn->setStyleSheet("background-color: rgb(70, 130, 180);");
}

void
ReceivableWidget::on_clearBtn_clicked()
{
    ui->screeningBtn->setStyleSheet("background-color: rgb(234, 234, 234);");
}

void
ReceivableWidget::on_fromDateCb_toggled(bool checked)
{
    ui->fromDateEdit->setEnabled(checked);
}

void
ReceivableWidget::on_toDateCb_toggled(bool checked)
{
    ui->toDateEdit->setEnabled(checked);
}
