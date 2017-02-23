#include "contractwidget.h"
#include "ui_contractwidget.h"
#include <QToolBar>
#include <tablemodel.h>

ContractWidget::ContractWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContractWidget)
{
    ui->setupUi(this);
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle("合同");

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

//    setContractTableviewData();
}

void
ContractWidget::initPriceTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "泵式" << "方量价格" << "标准台班价格"
               << "2.5小时内台板价格" << "4小时内台班价格" << "备注";

    mContractModel = new TableModel(0, headerList.size());
    ui->prictTableView->setModel(mContractModel);
    mContractModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->prictTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->prictTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->prictTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->prictTableView->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->prictTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->prictTableView->setAlternatingRowColors(true);
    ui->prictTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->prictTableView->resizeColumnToContents(3);                      //自动适应列宽
    ui->prictTableView->resizeColumnToContents(4);                      //自动适应列宽
//    setPriceTableViewData();
}
