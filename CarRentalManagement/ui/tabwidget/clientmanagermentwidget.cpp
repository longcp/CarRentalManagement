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
#include <util.h>
#include <user.h>

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
    configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
//    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);
    ui->toolBarHorizontalLayout->addWidget(mToolBar);

    initView();

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
            this, SLOT(deleteClientItemSlot()));
    /**
     * @brief 编辑条目
     */
    connect(mActEdit, SIGNAL(triggered()),
            this, SLOT(editClientItemSlot()));

    /**
     * @brief 同步调整列宽
     */
    connect(ui->clientTableView->horizontalHeader(),&QHeaderView::sectionResized,
            this, &ClientManagermentWidget::updateSumTabSectionWidth);
    /**
     * @brief 根据进度条值同步列表位置
     */
    connect((QObject*)ui->sumTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->clientTableView->horizontalScrollBar(), SLOT(setValue(int)));

    connect(this, SIGNAL(initViewWithUserSig(User &)),
            mClientEditDialog, SLOT(initViewWithUser(User &)));
}

ClientManagermentWidget::~ClientManagermentWidget()
{
    delete ui;
}

void
ClientManagermentWidget::initView()
{
    initSumTableView();
    initClientTableView();
    setWindowTitle(TAB_TITLE_CLIENTMANAGERMENT);
    ui->typeWidget->setStyleSheet("background-color: "
                                  "rgb(234,234,234);color:rgb(0,0,0);");
    mCurPayTypeFilter = PayTypeFilter::PAYTYPE_TOTAL;
    mCurClientTypeFilter = ClientTypeFilter::CLIENTTYPE_TOTAL;
    ui->paytypeTotalRadioButton->setChecked(true);
    ui->clientTypeTotalRadioButton->setChecked(true);
}

void
ClientManagermentWidget::initViewWithUser(User &user)
{
    if (!user.isRoot()) {
        mActAdd->setEnabled(false);
        mActEdit->setEnabled(false);
        mActDelete->setEnabled(false);
    }
    emit initViewWithUserSig(user);
}

void
ClientManagermentWidget::initClientTableView()
{
    //设置首行标题
    QStringList headerList;
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

    ui->clientTableView->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->clientTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->clientTableView->setAlternatingRowColors(true);
    ui->clientTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    //列宽随内容自适应
//    ui->clientTableView->horizontalHeader()
//            ->setSectionResizeMode(QHeaderView::ResizeToContents);    //整个表格随内容自适应，如果设了这个，
                                                                        //合计表格宽度无法与之同步，所以使用单列自适应
    ui->clientTableView->horizontalHeader()
            ->setSectionResizeMode(CLIENT_COL_NUM, QHeaderView::ResizeToContents);
    ui->clientTableView->horizontalHeader()
            ->setSectionResizeMode(CLIENT_COL_NAME, QHeaderView::ResizeToContents);
    ui->clientTableView->horizontalHeader()
            ->setSectionResizeMode(CLIENT_COL_ADDRESS, QHeaderView::ResizeToContents);
    ui->clientTableView->horizontalHeader()
            ->setSectionResizeMode(CLIENT_COL_REMARKS, QHeaderView::Stretch);
    //隐藏滚动条
    ui->clientTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->clientTableView->setSortingEnabled(true);
}

void
ClientManagermentWidget::initSumTableView()
{
    //设置首行标题
    QStringList          headerList;
    headerList << "客户编号" << "客户类型" << "客户名称" << "地址"
               << "联系电话" << "传真" << "联系人"
               << "结账方式" << "月结日" << "工程款额"
               << "已付款额" << "余额" << "备注";

    mSumModel = new TableModel(0, headerList.size());
    ui->sumTableView->setModel(mSumModel);
    mSumModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->sumTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->sumTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sumTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->sumTableView->horizontalHeader()
            ->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);

    ui->sumTableView->verticalHeader()->setVisible(false);              //隐藏行表头
    ui->sumTableView->horizontalHeader()->setVisible(false);   //隐藏列表头
    ui->sumTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->sumTableView->verticalHeader()->setDefaultSectionSize(20);
    ui->sumTableView->setAlternatingRowColors(true);
    ui->sumTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    QStandardItem* sumStrItem = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sumStrItem;
    mSumModel->appendRow(items);
    clearSumTableData();
}

void
ClientManagermentWidget::initTableViewData()
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
    QStandardItem* amount = new QStandardItem(Util::doubleToDecimal2String(client.amount));
    QStandardItem* paid = new QStandardItem(Util::doubleToDecimal2String(client.paid));
    QStandardItem* balance = new QStandardItem(Util::doubleToDecimal2String(client.amount - client.paid));

    QList<QStandardItem*> items;
    items << num << clientype << name << addr << telephone
          << fax << contract << paytype << monthly
          << amount << paid << balance << remarks;
    mModel->appendRow(items);

    // 更新合计
    sumAmountCellAddValue(client.amount);
    sumPaidCellAddValue(client.paid);
    sumBalanceCellAddValue(client.amount - client.paid);
}

void
ClientManagermentWidget::updateClientItemSlot(Client &client)
{
    //更新合计表
    double oldAmount = mModel->index(curRow, CLIENT_COL_AMOUNT).data().toDouble();
    double oldPaid = mModel->index(curRow, CLIENT_COL_PAID).data().toDouble();
    double oldBalance = mModel->index(curRow, CLIENT_COL_BALANCE).data().toDouble();
    sumAmountCellAddValue(client.amount - oldAmount);
    sumPaidCellAddValue(client.paid - oldPaid);
    sumBalanceCellAddValue((client.amount - client.paid) - oldBalance);

    //更新数据表
    mModel->setData(mModel->index(curRow, CLIENT_COL_NUM), client.number);
    mModel->setData(mModel->index(curRow, CLIENT_COL_TYPE),client.getClientTypeStr(client.clienttype));
    mModel->setData(mModel->index(curRow, CLIENT_COL_NAME), client.name);
    mModel->setData(mModel->index(curRow, CLIENT_COL_ADDRESS), client.address);
    mModel->setData(mModel->index(curRow, CLIENT_COL_TEL), client.telephone);
    mModel->setData(mModel->index(curRow, CLIENT_COL_FAX), client.fax);
    mModel->setData(mModel->index(curRow, CLIENT_COL_CONTRACT), client.contract);
    mModel->setData(mModel->index(curRow, CLIENT_COL_PAYTYPE),client.getPayTypeStr(client.paytype));
    mModel->setData(mModel->index(curRow, CLIENT_COL_MONTHLY),QString::number(client.monthly));
    mModel->setData(mModel->index(curRow, CLIENT_COL_AMOUNT), Util::doubleToDecimal2String(client.amount));
    mModel->setData(mModel->index(curRow, CLIENT_COL_PAID), Util::doubleToDecimal2String(client.paid));
    mModel->setData(mModel->index(curRow, CLIENT_COL_BALANCE), Util::doubleToDecimal2String(client.amount - client.paid));
    mModel->setData(mModel->index(curRow, CLIENT_COL_REMARKS), client.remarks);
}

void
ClientManagermentWidget::deleteClientItemSlot()
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
                                   tr("确定要删除该条目吗？\n"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    QString number = "";
    Client client;
    number = mModel->index(curRow, CLIENT_COL_NUM).data().toString();
    ret = mDb->getClientInNumber(number, client);
    if (ret)
        return;

    if (!mDb->deleteClientInNumber(number)) {
        ALOGD("%s, delete ok", __FUNCTION__);
        mModel->removeRow(curRow);
        // 更新合计
        sumAmountCellDelValue(client.amount);
        sumPaidCellDelValue(client.paid);
        sumBalanceCellDelValue(client.amount - client.paid);
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
ClientManagermentWidget::removeAllRows()
{
    if (mModel->rowCount() > 0)
        mModel->removeRows(0, mModel->rowCount());
    clearSumTableData();
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
        removeAllRows();
        size = clients.size();
        for (int i = 0; i < size; i++) {
            client = clients.at(i);
            addClientItemSlot(client);
        }
    }
    
    enableFilterView();
}

void
ClientManagermentWidget::updateSumTabSectionWidth(int logicalIndex, int, int newSize)
{
    ui->sumTableView->setColumnWidth(logicalIndex, newSize);
}

void
ClientManagermentWidget::clearSumTableData()
{
    setSumAmountCellValue(0);
    setSumPaidCellValue(0);
    setSumBalanceCellValue(0);
}

void
ClientManagermentWidget::setSumAmountCellValue(double value)
{
    mSumModel->setData(mSumModel->index(0, CLIENT_COL_AMOUNT), Util::doubleToDecimal2String(value));
    mCurSumAmountValue = value;
}

void
ClientManagermentWidget::setSumPaidCellValue(double value)
{
    mSumModel->setData(mSumModel->index(0, CLIENT_COL_PAID), Util::doubleToDecimal2String(value));
    mCurSumPaidValue = value;
}

void
ClientManagermentWidget::setSumBalanceCellValue(double value)
{
    mSumModel->setData(mSumModel->index(0, CLIENT_COL_BALANCE), Util::doubleToDecimal2String(value));
    mCurSumBalanceValue = value;
}

void
ClientManagermentWidget::sumAmountCellAddValue(double value)
{
    setSumAmountCellValue(mCurSumAmountValue+value);
}

void
ClientManagermentWidget::sumPaidCellAddValue(double value)
{
    setSumPaidCellValue(mCurSumPaidValue+value);
}

void
ClientManagermentWidget::sumBalanceCellAddValue(double value)
{
    setSumBalanceCellValue(mCurSumBalanceValue+value);
}

void
ClientManagermentWidget::sumAmountCellDelValue(double value)
{
    setSumAmountCellValue(mCurSumAmountValue-value);
}

void
ClientManagermentWidget::sumPaidCellDelValue(double value)
{
    setSumPaidCellValue(mCurSumPaidValue-value);
}

void
ClientManagermentWidget::sumBalanceCellDelValue(double value)
{
    setSumBalanceCellValue(mCurSumBalanceValue-value);
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
    if (!checked)
        return;

    mCurClientTypeFilter = ClientTypeFilter::TEMPORARY;
    updateTableView();
}

void
ClientManagermentWidget::on_clientTypeTotalRadioButton_toggled(bool checked)
{
    ALOGDTRACE();
    if (!checked)
        return;

    mCurClientTypeFilter = ClientTypeFilter::CLIENTTYPE_TOTAL;
    updateTableView();
}
