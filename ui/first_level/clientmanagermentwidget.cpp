#include "clientmanagermentwidget.h"
#include "ui_clientmanagermentwidget.h"
#include <QToolBar>
#include <QTableWidget>
#include <clienteditdialog.h>

#define LOG_TAG                 "CLIENT_MANAGERMENT_WIDGET"
#include "utils/Log.h"

ClientManagermentWidget::ClientManagermentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientManagermentWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("客户资料");

    mClientEditDialog = new ClientEditDialog();

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->clientTableWidget->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->clientTableWidget->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->clientTableWidget->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->typeWidget->setStyleSheet("background-color: "
                                  "rgb(234,234,234);color:rgb(0,0,0);");

    ui->clientTableWidget->verticalHeader()->setVisible(false);         //隐藏行表头
    ui->clientTableWidget->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->clientTableWidget->setAlternatingRowColors(true);
    ui->clientTableWidget->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

//    ui->clientTableWidget->rowCount();
//    ui->clientTableWidget->insertRow(xxx);

    mActAdd = new QAction(QIcon(":/menu/icon/add_64.ico"),
                          tr("增加"), this);
    mActEdit= new QAction(QIcon(":/menu/icon/edit_64.ico"),
                          tr("修改"), this);
    mActDelete = new QAction(QIcon(":/menu/icon/delete_64.ico"),
                             tr("删除"), this);
    mActSearch = new QAction(QIcon(":/menu/icon/search_64.ico"),
                             tr("查询"), this);
    mActExport = new QAction(QIcon(":/menu/icon/export_64.ico"),
                              tr("导出"), this);
    mActImport = new QAction(QIcon(":/menu/icon/import_64.ico"),
                              tr("导入"), this);
    mActPrinter = new QAction(QIcon(":/menu/icon/printer_64.ico"),
                              tr("打印"), this);

    mToolBar = new QToolBar(tr("clientToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActPrinter);
    mToolBar->addAction(mActExport);
    mToolBar->addAction(mActImport);

    ui->toolBarHorizontalLayout->addWidget(mToolBar);

    /**
     * @brief 单元格双击事件
     */
    connect(ui->clientTableWidget, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(cellDoubleClickedSlot(int,int)));
    /**
     * @brief 打开编辑窗口
     */
    connect(this, SIGNAL(openClientEditDialogSignal(bool)),
            mClientEditDialog, SLOT(openClientEditDialogSlot(bool)));
    /**
     * @brief 增加条目
     */
    connect(mActAdd, SIGNAL(triggered()),
            this, SLOT(addClientSlot()));
}

ClientManagermentWidget::~ClientManagermentWidget()
{
    delete ui;
}

void
ClientManagermentWidget::configToolBar()
{
    if (!mToolBar)
        return;

    mToolBar->setStyleSheet("background-color: rgb(234,234,234);color:rgb(0,0,0);");
    mToolBar->setLayoutDirection(Qt::LeftToRight);
    mToolBar->setIconSize(QSize(24,24));
    mToolBar->setOrientation(Qt::Horizontal);
    mToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    mToolBar->setFloatable(true);
    mToolBar->setMovable(true);
    mToolBar->setEnabled(true);
    mToolBar->setGeometry(0, 0, 0, 0);
    mToolBar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    mToolBar->setFocusPolicy(Qt::NoFocus);
    mToolBar->setContextMenuPolicy(Qt::DefaultContextMenu);
    mToolBar->setInputMethodHints(Qt::ImhNone);
    mToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void
ClientManagermentWidget::cellDoubleClickedSlot(int a,int b)
{
    ALOGD("%s, a = %d, b = %d", __FUNCTION__, a, b);
    emit openClientEditDialogSignal(false);
}

void
ClientManagermentWidget::addClientSlot()
{
    emit openClientEditDialogSignal(true);
}
