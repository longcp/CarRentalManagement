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

    QStringList clientWidgetHeader;
    clientWidgetHeader << "客户编号" << "客户名称" << "地址"
                       << "联系电话" << "传真" << "联系人"
                       << "结账方式" << "月结日" << "工程款额"
                       << "已付款额" << "余额" << "备注";
    mClientWidget = new QTableWidget(3, clientWidgetHeader.length());
    mClientWidget->setHorizontalHeaderLabels(clientWidgetHeader);

    //设备单元格不可编辑,单击选中一行且只能选中一行
    mClientWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mClientWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mClientWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    mClientWidget->setItem(0,1,new QTableWidgetItem("Jan"));
    mClientWidget->setItem(1,1,new QTableWidgetItem("Feb"));
    mClientWidget->setItem(2,1,new QTableWidgetItem("Mar"));
//    mClientWidget->rowCount();
//    mClientWidget->insertRow(xxx);

    mToolBar = new QToolBar(tr("clientToolBar"), this);
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

    mActAdd = new QAction(QIcon(":/menu/icon/add_64.ico"),
                          tr("增加"), this);
    mActEdit= new QAction(QIcon(":/menu/icon/edit_64.ico"),
                          tr("修改"), this);
    mActDelete = new QAction(QIcon(":/menu/icon/delete_64.ico"),
                             tr("删除"), this);
    mActSearch = new QAction(QIcon(":/menu/icon/search_64.ico"),
                             tr("查询"), this);
    mActPrinter = new QAction(QIcon(":/menu/icon/printer_64.ico"),
                              tr("打印"), this);
    mActExport = new QAction(QIcon(":/menu/icon/export_64.ico"),
                              tr("导出"), this);
    mActImport = new QAction(QIcon(":/menu/icon/import_64.ico"),
                              tr("导入"), this);

    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActPrinter);
    mToolBar->addAction(mActExport);
    mToolBar->addAction(mActImport);

    ui->clientVerticalLayout->addWidget(mToolBar);
    ui->clientVerticalLayout->addWidget(mClientWidget);

    /**
     * @brief 单元格双击事件
     */
    connect(mClientWidget, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(cellDoubleClickedSlot(int,int)));
    /**
     * @brief 单元格双击事件
     */
    connect(this, SIGNAL(openClientEditDialogSignal()),
            mClientEditDialog, SLOT(openClientEditDialogSlot()));
}

ClientManagermentWidget::~ClientManagermentWidget()
{
    delete ui;
}

void
ClientManagermentWidget::cellDoubleClickedSlot(int a,int b)
{
    ALOGD("%s, a = %d, b = %d", __FUNCTION__, a, b);
    emit openClientEditDialogSignal();
}
