#include "clientmanagermentwidget.h"
#include "ui_clientmanagermentwidget.h"
#include <QToolBar>
#include <QTableWidget>
#include <clienteditdialog.h>
#include <client.h>
#include <datatype.h>
#include <QStandardItem>
#include <database/database.h>
#include <tablemodel.h>
#include <QMessageBox>
#include <QDebug>

#define LOG_TAG                 "CLIENT_MANAGERMENT_WIDGET"
#include "utils/Log.h"

ClientManagermentWidget::ClientManagermentWidget(QWidget *parent) :
    QWidget(parent),
    curRow(-1),
    ui(new Ui::ClientManagermentWidget)
{
    ui->setupUi(this);

    mDb = DataBase::getInstance();
    mClientEditDialog = new ClientEditDialog();

    initView();

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

    mToolBar = new QToolBar(tr("clientToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);

    ui->toolBarHorizontalLayout->addWidget(mToolBar);

    /**
     * @brief 单元格双击事件
     */
    connect(ui->clientTableView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(cellDoubleClickedSlot(const QModelIndex &)));
    /**
     * @brief 单元格单击事件
     */
    connect(ui->clientTableView, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(cellClickedSlot(const QModelIndex&)));
    /**
     * @brief 打开编辑窗口
     */
    connect(this, SIGNAL(openClientEditDialogSignal(OpenType, Client &)),
            mClientEditDialog,
            SLOT(openClientEditDialogSlot(OpenType, Client &)));
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
    /**
     * @brief 更新条目
     */
    connect(mClientEditDialog, SIGNAL(updateClientItemSignal(Client &)),
            this, SLOT(updateClientItemSlot(Client &)));
    /**
     * @brief 删除条目
     */
    connect(mActDelete, SIGNAL(triggered()),
            this, SLOT(deleteClientItemSlog()));
    /**
     * @brief 编辑条目
     */
    connect(mActEdit, SIGNAL(triggered()),
            this, SLOT(editClientItemSlot()));

}

ClientManagermentWidget::~ClientManagermentWidget()
{
    delete ui;
}

void
ClientManagermentWidget::initView()
{
    this->setWindowTitle(TAB_TITLE_CLIENTMANAGERMENT);

    //设置首行标题
    QStringList          headerList;
    headerList << "客户编号" << "客户类型" << "客户名称" << "地址"
               << "联系电话" << "传真" << "联系人"
               << "结账方式" << "月结日" << "工程款额"
               << "已付款额" << "余额" << "备注";

    mModel = new TableModel(0, headerList.size());
    ui->clientTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->clientTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->clientTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->clientTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->clientTableView->horizontalHeader()
            ->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);

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
    ui->clientTableView->setSortingEnabled(true);

    mCurPayTypeFilter = PayTypeFilter::PAYTYPE_TOTAL;
    mCurClientTypeFilter = ClientTypeFilter::CLIENTTYPE_TOTAL;
    ui->paytypeTotalRadioButton->setChecked(true);
    ui->clientTypeTotalRadioButton->setChecked(true);
}

void
ClientManagermentWidget::initTableView()
{
    int i, ret, size;
    QList<Client>clients;
    Client client;

    ret = mDb->getAllClientData(clients);
    if (!ret) {
        size = clients.size();
        for (i = 0; i < size; i++) {
            client = clients.at(i);
            addClientItemSlot(client);
        }
    }
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
    ALOGD("%s, a = %d, b = %d", __FUNCTION__,
          index.column(), index.row());
    editRowEvent(index.row());
}

void
ClientManagermentWidget::editRowEvent(int row)
{
    Client client;
    QString clientNum = mModel->index(curRow, 0).data().toString();
    if (mDb->getClientInNumber(clientNum, client)) {
        ALOGE("getClientInNumber failed, sql err = %s",
              mDb->lastError().toStdString().data());
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("未知错误,无法查看该项.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    emit openClientEditDialogSignal(SHOWITEM, client);
}

void
ClientManagermentWidget::cellClickedSlot(const QModelIndex &index)
{
    curRow = index.row();
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
    char buf[64];
    QStandardItem* num = new QStandardItem(client.number);
    QStandardItem* clientype
            = new QStandardItem(client.getClientTypeStr(client.clienttype));
    QStandardItem* name = new QStandardItem(client.name);
    QStandardItem* addr = new QStandardItem(client.address);
    QStandardItem* telephone = new QStandardItem(client.telephone);
    QStandardItem* fax = new QStandardItem(client.fax);
    QStandardItem* contract = new QStandardItem(client.contract);
    QStandardItem* paytype
            = new QStandardItem(client.getPayTypeStr(client.paytype));
    QStandardItem* monthly
            = new QStandardItem(QString::number(client.monthly));
    QStandardItem* remarks = new QStandardItem(client.remarks);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", client.amount);
    QStandardItem* amount = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", client.paid);
    QStandardItem* paid = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", client.amount - client.paid);
    QStandardItem* balance = new QStandardItem(QString(buf));

    QList<QStandardItem*> items;
    items << num << clientype << name << addr << telephone
          << fax << contract << paytype << monthly
          << amount << paid << balance << remarks;
    mModel->appendRow(items);
}

void
ClientManagermentWidget::updateClientItemSlot(Client &client)
{
    char buf[64];
    mModel->setData(mModel->index(curRow, 0), client.number);
    mModel->setData(mModel->index(curRow, 1),
                    client.getClientTypeStr(client.clienttype));
    mModel->setData(mModel->index(curRow, 2), client.name);
    mModel->setData(mModel->index(curRow, 3), client.address);
    mModel->setData(mModel->index(curRow, 4), client.telephone);
    mModel->setData(mModel->index(curRow, 5), client.fax);
    mModel->setData(mModel->index(curRow, 6), client.contract);
    mModel->setData(mModel->index(curRow, 7), client.getPayTypeStr(client.paytype));
    mModel->setData(mModel->index(curRow, 8), QString::number(client.monthly));
    mModel->setData(mModel->index(curRow, 12), client.remarks);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", client.amount);
    mModel->setData(mModel->index(curRow, 9),
                    QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", client.paid);
    mModel->setData(mModel->index(curRow, 10),
                    QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", client.amount - client.paid);
    mModel->setData(mModel->index(curRow, 11),
                    QString(buf));
}

void
ClientManagermentWidget::deleteClientItemSlog()
{
    if (curRow < 0) {
        QMessageBox::warning(this,
                             tr("温馨提示"),
                             tr("请选择要删除条目.\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    int ret = QMessageBox::warning(this,
                                   tr("温馨提示"),
                                   tr("确定要删除该条目吗？.\n"),
                                   QMessageBox::Yes |
                                   QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    QString number = "";
    number = mModel->index(curRow, 0).data().toString();
    if (!mDb->deleteClientInNumber(number)) {
        ALOGD("%s, delete ok", __FUNCTION__);
        mModel->removeRow(curRow);
    }
}

void
ClientManagermentWidget::editClientItemSlot()
{
    if (curRow < 0) {
        QMessageBox::information(this,
                                 tr("温馨提示"),
                                 tr("请选择要编辑条目.\n"),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        return;
    }

    editRowEvent(curRow);
}

void
ClientManagermentWidget::setFilterViewState(bool state)
{
    ui->cashRadioButton->setEnabled(state);
    ui->monthlyRadioButton->setEnabled(state);
    ui->paytypeTotalRadioButton->setEnabled(state);
    ui->contractRadioButton->setEnabled(state);
    ui->temporaryRadioButton->setEnabled(state);
    ui->clientTypeTotalRadioButton->setEnabled(state);
}

void
ClientManagermentWidget::disableFilterView()
{
    setFilterViewState(false);
}

void
ClientManagermentWidget::enableFilterView()
{
    setFilterViewState(true);
}

void
ClientManagermentWidget::updateTableView()
{
    int ret, size;
    QString payTypeFilter = "";
    QString clientTypeFilter = "";
    QString whereStr = " WHERE ";
    QString andstr = " AND ";
    QString filter = "";
    QList<Client> clients;
    Client client;
    
    disableFilterView();
    
    switch (mCurPayTypeFilter) {
    case PayTypeFilter::CASH:
        payTypeFilter = "paytype=0";
        break;

    case PayTypeFilter::MONTHLY:
        payTypeFilter = "paytype=1";
        break;

    default:
        break;
    }

    switch (mCurClientTypeFilter) {
    case ClientTypeFilter::CONTRACT:
        clientTypeFilter += "clienttype=0;";
        break;

    case ClientTypeFilter::TEMPORARY:
        clientTypeFilter += "clienttype=1;";
        break;

    default:
        break;
    }

    if (payTypeFilter != "" && clientTypeFilter != "")
        filter = whereStr + payTypeFilter + andstr + clientTypeFilter;
    else if (payTypeFilter != "" || clientTypeFilter != "")
        filter = whereStr + payTypeFilter + clientTypeFilter;

    if(!mDb->getClientInFilter(clients, filter)) {
        if (mModel->rowCount() > 0)
            mModel->removeRows(0, mModel->rowCount());

        size = clients.size();
        for (int i = 0; i < size; i++) {
            client = clients.at(i);
            addClientItemSlot(client);
        }
    }
    
    enableFilterView();
}

void 
ClientManagermentWidget::on_cashRadioButton_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurPayTypeFilter = PayTypeFilter::CASH;
    updateTableView();
}

void
ClientManagermentWidget::on_monthlyRadioButton_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurPayTypeFilter = PayTypeFilter::MONTHLY;
    updateTableView();
}

void
ClientManagermentWidget::on_paytypeTotalRadioButton_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurPayTypeFilter = PayTypeFilter::PAYTYPE_TOTAL;
    updateTableView();
}

void
ClientManagermentWidget::on_contractRadioButton_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurClientTypeFilter = ClientTypeFilter::CONTRACT;
    updateTableView();
}

void
ClientManagermentWidget::on_temporaryRadioButton_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurClientTypeFilter = ClientTypeFilter::TEMPORARY;
    updateTableView();
}

void
ClientManagermentWidget::on_clientTypeTotalRadioButton_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurClientTypeFilter = ClientTypeFilter::CLIENTTYPE_TOTAL;
    updateTableView();
}
