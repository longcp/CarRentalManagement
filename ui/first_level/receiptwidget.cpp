#include "receiptwidget.h"
#include "ui_receiptwidget.h"
#include <QToolBar>
#include <receipteditdialog.h>

#define LOG_TAG                 "CLIENT_MANAGERMENT_WIDGET"
#include "utils/Log.h"

ReceiptWidget::ReceiptWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceiptWidget)
{
    ui->setupUi(this);
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle("合同收款");

    mReceiptEditDialog = new ReceiptEditDialog();

    //隐藏行表头
    ui->detailTableWidget->verticalHeader()->setVisible(false);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->detailTableWidget->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->detailTableWidget->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->detailTableWidget->setSelectionMode(
                QAbstractItemView::SingleSelection);

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
    mActImport = new QAction(QIcon(":/menu/icon/import_64.ico"),
                              tr("导入"), this);
    mActPrinter = new QAction(QIcon(":/menu/icon/printer_64.ico"),
                              tr("打印"), this);

    mToolBar = new QToolBar(tr("receiptToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActPrinter);
    mToolBar->addAction(mActExport);
    mToolBar->addAction(mActImport);

    ui->toolBarVerticalLayout->addWidget(mToolBar);

    /**
     * @brief 单元格双击事件
     */
    connect(ui->detailTableWidget, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(cellDoubleClickedSlot(int,int)));
    /**
     * @brief 单元格双击事件
     */
    connect(this, SIGNAL(openReceiptEditDialogSignal()),
            mReceiptEditDialog, SLOT(openReceiptEditDialogSlot()));
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
    mToolBar->setGeometry(0, 0, 0, 0);
    mToolBar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    mToolBar->setFocusPolicy(Qt::NoFocus);
    mToolBar->setContextMenuPolicy(Qt::DefaultContextMenu);
    mToolBar->setInputMethodHints(Qt::ImhNone);
    mToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void
ReceiptWidget::cellDoubleClickedSlot(int a,int b)
{
    ALOGD("%s, a = %d, b = %d", __FUNCTION__, a, b);
    emit openReceiptEditDialogSignal();
}