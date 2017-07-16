#include "contracttabledialog.h"
#include "ui_contracttabledialog.h"

#include <database/database.h>
#include <rentaldocument.h>
#include <tablemodel.h>
#include <contract.h>

#define LOG_TAG                 "CONTRACT_TABLE_DIALOG"
#include "utils/Log.h"

ContractTableDialog::ContractTableDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::ContractTableDialog)
{
    ui->setupUi(this);
    this->setFixedSize(750, 700);
    initView();
}

ContractTableDialog::~ContractTableDialog()
{
    delete ui;
}

void
ContractTableDialog::initView()
{
    this->setWindowTitle("选择合同");

    //设置首行标题
    QStringList          headerList;
    headerList << "合同号" << "客户名称" << "工程名" << "工程地址"
               << "签订日期";

    mModel = new TableModel(0, headerList.size());
    ui->contractTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->contractTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->contractTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->contractTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);
    ui->contractTableView->horizontalHeader()->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);

    ui->contractTableView->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->contractTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->contractTableView->setAlternatingRowColors(true);
    ui->contractTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->contractTableView->setColumnWidth(0, 150);
    ui->contractTableView->setColumnWidth(1, 150);
    ui->contractTableView->setColumnWidth(2, 150);
    ui->contractTableView->setColumnWidth(3, 150);
    ui->contractTableView->setColumnWidth(4, 150);
}

void
ContractTableDialog::initTableView()
{
    int size;
    QList<Contract>contracts;
    Contract contract;

    if (mModel->rowCount() > 0)
        mModel->removeRows(0, mModel->rowCount());

    if (!mDb->getAllContractData(contracts)) {
        size = contracts.size();
        for (int i = 0; i < size; i++) {
            contract = contracts.at(i);
            addContractItem(contract);
        }
    }
}

void
ContractTableDialog::addContractItem(Contract &contract)
{
    QStandardItem* num
            = new QStandardItem(contract.number);
    QStandardItem* clientName
            = new QStandardItem(contract.clientName);
    QStandardItem* projectName
            = new QStandardItem(contract.projectName);
    QStandardItem* projectAddr
            = new QStandardItem(contract.projectAddress);
    QStandardItem* signedDate
            = new QStandardItem(contract.signedDate.toString(DATE_FORMAT_STR));

    QList<QStandardItem*> items;
    items << num << clientName << projectName << projectAddr << signedDate;
    mModel->appendRow(items);
}

void
ContractTableDialog::openWindow()
{
    ALOGDTRACE();
    initTableView();
    this->exec();
}

void
ContractTableDialog::on_contractTableView_doubleClicked(const QModelIndex &index)
{
    ALOGDTRACE();
    int curRow = ui->contractTableView->currentIndex().row();
    emit selectedContract(mModel->index(curRow, 0).data().toString());
    this->close();
}
