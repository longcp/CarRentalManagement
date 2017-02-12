#include "clientmanagermentwidget.h"
#include "ui_clientmanagermentwidget.h"
#include <QToolBar>
#include <QTableWidget>
#include <clienteditdialog.h>
#include <client.h>
#include <datatype.h>
#include <QStandardItem>
#include <database/database.h>
#include <warnmodel.h>

#define LOG_TAG                 "CLIENT_MANAGERMENT_WIDGET"
#include "utils/Log.h"

ClientManagermentWidget::ClientManagermentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientManagermentWidget)
{
    ui->setupUi(this);

    mDb = DataBase::getInstance();
    mClientEditDialog = new ClientEditDialog();

    initView();

//    ui->clientTableView->rowCount();
//    ui->clientTableView->insertRow(xxx);

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
    connect(ui->clientTableView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(cellDoubleClickedSlot(const QModelIndex &)));
    /**
     * @brief 打开编辑窗口
     */
    connect(this, SIGNAL(openClientEditDialogSignal(OpenType, const Client &)),
            mClientEditDialog,
            SLOT(openClientEditDialogSlot(OpenType, const Client &)));
    /**
     * @brief 打开编辑窗口
     */
    connect(mActAdd, SIGNAL(triggered()),
            this, SLOT(addClientSlot()));
    /**
     * @brief 增加条目
     */
    connect(mClientEditDialog, SIGNAL(addClientItemSignal(Client &)),
            this, SLOT(addClientItemSlot(Client&)));
}

ClientManagermentWidget::~ClientManagermentWidget()
{
    delete ui;
}

void
ClientManagermentWidget::initView()
{
    this->setWindowTitle("客户资料");

    //设置首行标题
    QStringList          headerList;
    headerList << "客户编号" << "客户类型" << "客户名称" << "地址"
               << "联系电话" << "传真" << "联系人"
               << "结账方式" << "月结日" << "工程款额"
               << "已付款额" << "余额" << "备注";

    mModel = new WarnModel(0, headerList.size());
    ui->clientTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);


    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->clientTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->clientTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->clientTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->typeWidget->setStyleSheet("background-color: "
                                  "rgb(234,234,234);color:rgb(0,0,0);");

    ui->clientTableView->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->clientTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->clientTableView->setAlternatingRowColors(true);
    ui->clientTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
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
    mToolBar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    mToolBar->setFocusPolicy(Qt::NoFocus);
    mToolBar->setContextMenuPolicy(Qt::DefaultContextMenu);
    mToolBar->setInputMethodHints(Qt::ImhNone);
    mToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void
ClientManagermentWidget::cellDoubleClickedSlot(const QModelIndex & index)
{
    ALOGD("%s, a = %d, b = %d", __FUNCTION__, index.column(), index.row());
//    emit openClientEditDialogSignal(OpenType::SHOWITEM);
}

void
ClientManagermentWidget::addClientSlot()
{
    Client client;
    emit openClientEditDialogSignal(CREATEITEM, client);
}

void
ClientManagermentWidget::addClientItemSlot(Client &client)
{
    ALOGD("ClientManagermentWidget enter!\n");
#if 0
    ALOGD("name = %s, number = %s, telephone = %s, \n"
          "address = %s, email = %s, fax = %s, \n"
          "contract = %s, remarks = %s, creator = %s, \n"
          "createDate = %s, paytype = %d, monthly = %d, \n"
          "clienttype = %d\n",
          client.name.toStdString().data(),
          client.number.toStdString().data(),
          client.telephone.toStdString().data(),
          client.address.toStdString().data(),
          client.email.toStdString().data(),
          client.fax.toStdString().data(),
          client.contract.toStdString().data(),
          client.remarks.toStdString().data(),
          client.creator.toStdString().data(),
          client.createDate.toString("yyyy-MM-dd").toStdString().data(),
          client.paytype,
          client.monthly, client.clienttype);
#endif

    QStandardItem* num
            = new QStandardItem(client.number);
    QStandardItem* clientype
            = new QStandardItem(client.getClientTypeStr(client.clienttype));
    QStandardItem* name
            = new QStandardItem(client.name);
    QStandardItem* addr
            = new QStandardItem(client.address);
    QStandardItem* telephone
            = new QStandardItem(client.telephone);
    QStandardItem* fax
            = new QStandardItem(client.fax);
    QStandardItem* contract
            = new QStandardItem(client.contract);
    QStandardItem* paytype
            = new QStandardItem(client.getPayTypeStr(client.paytype));
    QStandardItem* monthly
            = new QStandardItem(QString::number(client.monthly));
    QStandardItem* amount
            = new QStandardItem(QString("%1").arg(client.amount));
    QStandardItem* paid
            = new QStandardItem(QString("%1").arg(client.paid));
    QStandardItem* balance
            = new QStandardItem(QString("%1").arg(client.amount - client.paid));
    QStandardItem* remarks
            = new QStandardItem(client.remarks);

    QList<QStandardItem*> items;
    items << num << clientype << name << addr << telephone
          << fax << contract << paytype << monthly
          << amount << paid << balance << remarks;
    mModel->appendRow(items);
}
