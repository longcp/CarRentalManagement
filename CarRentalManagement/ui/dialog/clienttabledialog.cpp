#include "clienttabledialog.h"
#include "ui_clienttabledialog.h"
#include <tablemodel.h>
#include <client.h>'
#include <database/database.h>

#define LOG_TAG                 "CLIENT_TABLE_DIALOG"
#include "utils/Log.h"

ClientTableDialog::ClientTableDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::ClientTableDialog)
{
    ui->setupUi(this);
    initView();
}

ClientTableDialog::~ClientTableDialog()
{
    delete ui;
}

void
ClientTableDialog::initView()
{
    this->setWindowTitle("选择客户");

    //设置首行标题
    QStringList          headerList;
    headerList << "客户编号" << "客户类型" << "客户名称" << "地址"
               << "联系电话";

    mModel = new TableModel(0, headerList.size());
    ui->clientTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->clientTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->clientTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->clientTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);
    ui->clientTableView->horizontalHeader()->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);

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
ClientTableDialog::openWindow()
{
    ALOGDTRACE();
    initTableView();
    this->exec();
}

void
ClientTableDialog::initTableView()
{
    int size;
    QList<Client>clients;
    Client client;

    if (mModel->rowCount() > 0)
        mModel->removeRows(0, mModel->rowCount());

    if (!mDb->getAllClientData(clients)) {
        size = clients.size();
        for (int i = 0; i < size; i++) {
            client = clients.at(i);
            addClientItem(client);
        }
    }
}

void
ClientTableDialog::addClientItem(Client &client)
{
    ALOGDTRACE();
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

    QList<QStandardItem*> items;
    items << num << clientype << name << addr << telephone;
    mModel->appendRow(items);
}

void
ClientTableDialog::on_clientTableView_doubleClicked(const QModelIndex &index)
{
    ALOGDTRACE();
    int curRow = ui->clientTableView->currentIndex().row();
    emit selectedClient(mModel->index(curRow, 0).data().toString());
    this->close();
}
