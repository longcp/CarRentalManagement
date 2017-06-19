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

    initView();

    mToolBar = new QToolBar(tr("receiptToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);

    ui->toolBarVerticalLayout->addWidget(mToolBar);

    /**
     * @brief 单元格双击事件
     */
    connect(ui->receiptTable, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(cellDoubleClickedSlot(int,int)));
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

    QStandardItem* sumStrItem = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sumStrItem;
    mReceiptSumModel->appendRow(items);
//    resetReceiptSumTableData();
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
ReceiptWidget::reflashView(QList<RentalDocument> docs)
{

}

void
ReceiptWidget::tabChangeToReceiptSlot(int index, QString tabText)
{
    ALOGDTRACE();
    QList<RentalDocument> docs;

    if (tabText != TAB_TITLE_RECEIPT ||
            mDb->getAllRentalDocumentData(docs))
        return;

    reflashView(docs);
}
