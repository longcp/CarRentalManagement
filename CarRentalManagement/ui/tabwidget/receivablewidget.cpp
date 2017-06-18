#include "receivablewidget.h"
#include "ui_receivablewidget.h"
#include <QToolBar>
#include <tablemodel.h>
#include <QScrollBar>
#include <contract.h>
#include <rentaldocument.h>
#include <database/database.h>

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
    this->setWindowTitle(TAB_TITLE_RECEIVABLE);

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

    mToolBar = new QToolBar(tr("receivableToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);

    ui->toolBarVerticalLayout->addWidget(mToolBar);

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
    initDetailTableview();
    initDetailSumTableview();
    initTotalTableview();
    initTotalSumTableview();
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

    ui->detailTableview->setColumnWidth(COLUMN_DOC_NUMBER, 200);
    ui->detailTableview->setColumnWidth(COLUMN_CONTRACT_NUMBER, 200);
    ui->detailTableview->setColumnWidth(COLUMN_CLIENT_NAME, 200);
    ui->detailTableview->setColumnWidth(COLUMN_CAR_PLATE_NUMBER, 200);
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

    ui->detailSummaryTablview->setColumnWidth(COLUMN_DOC_NUMBER, 200);
    ui->detailSummaryTablview->setColumnWidth(COLUMN_CONTRACT_NUMBER, 200);
    ui->detailSummaryTablview->setColumnWidth(COLUMN_CLIENT_NAME, 200);
    ui->detailSummaryTablview->setColumnWidth(COLUMN_CAR_PLATE_NUMBER, 200);

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

    ui->totalTableview->setColumnWidth(0, 180);
    ui->totalTableview->setColumnWidth(1, 180);
    ui->totalTableview->setColumnWidth(2, 180);
    ui->totalTableview->setColumnWidth(3, 180);
    ui->totalTableview->setColumnWidth(4, 180);
    ui->totalTableview->setColumnWidth(5, 180);
    ui->totalTableview->setColumnWidth(6, 180);

    //隐藏滚动条
    ui->totalTableview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

    ui->totalSummaryTableview->setColumnWidth(0, 180);
    ui->totalSummaryTableview->setColumnWidth(1, 180);
    ui->totalSummaryTableview->setColumnWidth(2, 180);
    ui->totalSummaryTableview->setColumnWidth(3, 180);
    ui->totalSummaryTableview->setColumnWidth(4, 180);
    ui->totalSummaryTableview->setColumnWidth(5, 180);
    ui->totalSummaryTableview->setColumnWidth(6, 180);

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
    setPumpSquareCellValue(0);
    setPumpTimeCellValue(0);
    setProjectAmountCellValue(0);
    setReceiptCellValue(0);
    setReceivableValue(0);
}

void
ReceivableWidget::resetTotalSumTableData()
{

}

void
ReceivableWidget::setPumpSquareCellValue(float value)
{
    mDetailSumModel->setData(mDetailSumModel->index(0, COLUMN_PUMP_SQUARE),
                             QString("%1").arg(value, 0, 'f', 2));
}

void
ReceivableWidget::setPumpTimeCellValue(float value)
{
    mDetailSumModel->setData(mDetailSumModel->index(0, COLUMN_PUMP_TIME),
                             QString("%1").arg(value, 0, 'f', 2));
}

void
ReceivableWidget::setProjectAmountCellValue(float value)
{
    mDetailSumModel->setData(mDetailSumModel->index(0, COLUMN_PROJECT_AMOUNT),
                             QString("%1").arg(value, 0, 'f', 2));
}

void
ReceivableWidget::setReceiptCellValue(float value)
{

    mDetailSumModel->setData(mDetailSumModel->index(0, COLUMN_RECEIPT),
                             QString("%1").arg(value, 0, 'f', 2));
}

void
ReceivableWidget::setReceivableValue(float value)
{
    mDetailSumModel->setData(mDetailSumModel->index(0, COLUMN_RECEIVABLE),
                             QString("%1").arg(value, 0, 'f', 2));
}

void
ReceivableWidget::pumpSquareCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, COLUMN_PUMP_SQUARE).data().toFloat();
    setPumpSquareCellValue(curValue+value);
}

void
ReceivableWidget::pumpTimeCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, COLUMN_PUMP_TIME).data().toFloat();
    setPumpTimeCellValue(curValue+value);
}

void
ReceivableWidget::projectAmountCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, COLUMN_PROJECT_AMOUNT).data().toFloat();
    setProjectAmountCellValue(curValue+value);
}

void
ReceivableWidget::receiptCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, COLUMN_RECEIPT).data().toFloat();
    setReceiptCellValue(curValue+value);
}

void
ReceivableWidget::receivableCellAddValue(float value)
{
    float curValue = mDetailSumModel->index(0, COLUMN_RECEIVABLE).data().toFloat();
    setReceivableValue(curValue+value);
}

void
ReceivableWidget::clearDetailTableview()
{
    if (mDetailModel->rowCount())
        mDetailModel->removeRows(0, mDetailModel->rowCount());

    resetDetailSumTableData();
}

void
ReceivableWidget::clearSumTableview()
{
    if (mDetailSumModel->rowCount())
        mTotalModel->removeRows(0, mDetailSumModel->rowCount());

    // FIXME: reset
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
        addDetailTableRow(doc);
        pumpSquareCellAddValue(doc.pumpSquare);
        pumpTimeCellAddValue(doc.pumpTimes);
        projectAmountCellAddValue(doc.projectAmount);
        receiptCellAddValue(doc.receivedAccounts);
        receivableCellAddValue(doc.projectAmount-doc.receivedAccounts);
    }
}

void
ReceivableWidget::addSumTableRows(QList<RentalDocument> &docs)
{
    RentalDocument doc;
    for (int i = 0; i < docs.size(); i++) {
        doc = docs.at(i);
        // FIXME:合同号一样的条目，只添加一行，数据合计到该行上
        addSumTableRow(doc);
    }
}

void
ReceivableWidget::addDetailTableRow(RentalDocument &doc)
{
    ALOGDTRACE();
    QStandardItem *date = new QStandardItem(doc.date.toString(DATE_FORMAT_STR));
    QStandardItem *docNum = new QStandardItem(doc.number);
    QStandardItem *contractNumber = new QStandardItem(doc.contractNumber);
    QStandardItem *clientName = new QStandardItem(doc.clientName);
    QStandardItem *carPlateNumber = new QStandardItem(doc.carPlateNumber);
    QStandardItem *pumpType = new QStandardItem(QString("%1").arg(doc.pumpType));
    QStandardItem *concreteLable = new QStandardItem(doc.concreteLable);
    QStandardItem *squareUnitPrice = new QStandardItem(QString("%1")
                                                       .arg(doc.squareUnitPrice));
    QStandardItem *pumpTimeUnitPrice = new QStandardItem(QString("%1")
                                                         .arg(doc.pumpTimeUnitPrice));
    QStandardItem *pumpSquare = new QStandardItem(QString("%1")
                                                  .arg(doc.pumpSquare));
    QStandardItem *pumpTimes = new QStandardItem(QString("%1")
                                                 .arg(doc.pumpTimes));
    QStandardItem *projectAmounts =  new QStandardItem(QString("%1")
                                                       .arg(doc.projectAmount));
    QStandardItem *receivedAccounts = new QStandardItem(QString("%1")
                                                        .arg(doc.receivedAccounts));
    QStandardItem *receivable = new QStandardItem(QString("%1")
                                                  .arg(doc.projectAmount - doc.receivedAccounts));
    QStandardItem *projectName = new QStandardItem(doc.projectName);
    QStandardItem *projectAddr = new QStandardItem(doc.projectAddress);
    QStandardItem *constructPlace = new QStandardItem(doc.constructPlace);
    QStandardItem *principal = new QStandardItem(doc.principal);
    QStandardItem *principalTel = new QStandardItem(doc.principalTel);
    QStandardItem *remarks = new QStandardItem(doc.remarks);

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
    QStandardItem *contractNumber = new QStandardItem(doc.contractNumber);
    QStandardItem *clientName = new QStandardItem(doc.clientName);
    QStandardItem *pumpSquare = new QStandardItem(QString("%1")
                                                  .arg(doc.pumpSquare));
    QStandardItem *pumpTimes = new QStandardItem(QString("%1")
                                                 .arg(doc.pumpTimes));
    QStandardItem *projectAmounts =  new QStandardItem(QString("%1")
                                                       .arg(doc.projectAmount));
    QStandardItem *receivedAccounts = new QStandardItem(QString("%1")
                                                        .arg(doc.receivedAccounts));
    QStandardItem *receivable = new QStandardItem(QString("%1")
                                                  .arg(doc.projectAmount - doc.receivedAccounts));
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
