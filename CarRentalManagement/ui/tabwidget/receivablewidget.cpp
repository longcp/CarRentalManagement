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
    headerList << "日期" << "签证单号" << "客户名称"
               << "车号" << "泵式" << "混凝土标号"
               << "方量单价/方" << "台班单价/小时" << "泵送方量"
               << "泵送台班" << "泵送台班时间" << "总金额"
               << "已收金额" << "应收金额" << "工程名称"
               << "工程地址" << "施工部位"
               << "备注" << "联系人" << "联系电话"
               << "合同号";

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
}

void
ReceivableWidget::initDetailSumTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "日期" << "签证单号" << "客户名称"
               << "车号" << "泵式" << "混凝土标号"
               << "方量单价/方" << "台班单价/小时" << "泵送方量"
               << "泵送台班" << "泵送台班时间" << "总金额"
               << "已收金额" << "应收金额" << "工程名称"
               << "工程地址" << "施工部位"
               << "备注" << "联系人" << "联系电话"
               << "合同号";

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

    QStandardItem* sumStrItem = new QStandardItem("合计");
    QStandardItem* nullStrItem = new QStandardItem("");
    QList<QStandardItem*> items;
    items << sumStrItem;
    mDetailSumModel->appendRow(items);
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
    mTotalModel = new TableModel(0, headerList.size());
    ui->totalSummaryTableview->setModel(mTotalModel);
    mTotalModel->setHorizontalHeaderLabels(headerList);
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
    QStandardItem* nullStrItem = new QStandardItem("");
    QList<QStandardItem*> items;
    items << sumStrItem;
    mTotalModel->appendRow(items);
}

void
ReceivableWidget::clearDetailTableview()
{
    if (mDetailModel->rowCount())
        mDetailModel->removeRows(0, mDetailModel->rowCount());
}

void
ReceivableWidget::clearSumTableview()
{
    if (mDetailSumModel->rowCount())
        mTotalModel->removeRows(0, mDetailSumModel->rowCount());
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
    }
}

void
ReceivableWidget::addSumTableRows(QList<RentalDocument> &docs)
{
    RentalDocument doc;
    for (int i = 0; i < docs.size(); i++) {
        doc = docs.at(i);
        addSumTableRow(doc);
    }
}

void
ReceivableWidget::addDetailTableRow(RentalDocument &doc)
{
    ALOGDTRACE();
    QStandardItem *date = new QStandardItem(doc.date.toString(DATE_FORMAT_STR));
    QStandardItem *docNum = new QStandardItem(doc.number);
    QStandardItem *clientName = new QStandardItem(doc.clientName);
    QStandardItem *carPlateNumber = new QStandardItem(doc.carPlateNumber);
    QStandardItem *concreteLable = new QStandardItem(doc.concreteLable);
    QStandardItem *squareUnitPrice = new QStandardItem(QString("%1")
                                                       .arg(doc.squareUnitPrice));
    QStandardItem *pumpTimeUnitPrice = new QStandardItem(QString("%1")
                                                         .arg(doc.pumpTimeUnitPrice));
    QStandardItem *pumpSquare = new QStandardItem(QString("%1")
                                                  .arg(doc.pumpSquare));
    QStandardItem *pumpTimes = new QStandardItem(QString("%1")
                                                 .arg(doc.pumpTimes));

    QStandardItem *projectName = new QStandardItem(doc.projectName);
    QStandardItem *projectAddr = new QStandardItem(doc.projectAddress);
    QStandardItem *pumpType = new QStandardItem(QString("%1").arg(doc.pumpType));
    QStandardItem *constructPlace = new QStandardItem(doc.constructPlace);
    QStandardItem *beginFuel = new QStandardItem(QString("%1")
                                                 .arg(doc.beginFuel));
    QStandardItem *endFuel = new QStandardItem(QString("%1").arg(doc.endFuel));
    QStandardItem *arrivalDateTime = new QStandardItem(doc.arrivalDateTime
                                                       .toString(DATETIME_FORMAT_STR));
    QStandardItem *leaveDateTime = new QStandardItem(doc.leaveDateTime
                                                     .toString(DATETIME_FORMAT_STR));
    QStandardItem *workingHours = new QStandardItem(QString("%1")
                                                    .arg(doc.workingHours));
    QStandardItem *principal = new QStandardItem(doc.principal);
    QStandardItem *principalTel = new QStandardItem(doc.principalTel);
    QStandardItem *driver1 = new QStandardItem(doc.driver1);
    QStandardItem *driver2 = new QStandardItem(doc.driver2);
    QStandardItem *driver3 = new QStandardItem(doc.driver3);
    QStandardItem *remarks = new QStandardItem(doc.remarks);

    QList<QStandardItem*> items;
    items << docNum << clientName << projectName << projectAddr << date
          << carPlateNumber << pumpType << constructPlace << concreteLable
          << beginFuel << endFuel << arrivalDateTime << leaveDateTime
          << workingHours << pumpSquare << squareUnitPrice << pumpTimes
          << pumpTimeUnitPrice << principal << principalTel << driver1
          << driver2 << driver3 << remarks;
    mModel->appendRow(items);
}

void
ReceivableWidget::addSumTableRow(RentalDocument &doc)
{

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
    if (mDb->getAllRentalDocumentData(docs))
        return;

    clearDetailTableview();
    clearSumTableview();
    reflashDetailTableview(docs);
    reflashSumTableview(docs);
}
