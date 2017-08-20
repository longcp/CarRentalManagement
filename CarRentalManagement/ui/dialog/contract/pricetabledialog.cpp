#include "pricetabledialog.h"
#include "ui_pricetabledialog.h"
#include <tablemodel.h>
#include <car.h>
#include <database/database.h>

#define LOG_TAG                 "PRICE_TABLE_DIALOG"
#include "utils/Log.h"

PriceTableDialog::PriceTableDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::PriceTableDialog)
{
    ui->setupUi(this);
    setFixedSize(750, 700);
    initView();
}

PriceTableDialog::~PriceTableDialog()
{
    delete ui;
}

void
PriceTableDialog::initView()
{
    this->setWindowTitle("选择价格");

    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "合同编号" << "泵式" << "方量单价/方"
               << "台班单价/台";

    mModel = new TableModel(0, headerList.size());
    ui->tableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->tableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(
                QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);

    ui->tableView->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->tableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    ui->tableView->setSortingEnabled(true);
//    ui->clientTableView->horizontalHeader()
//            ->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setColumnWidth(0, 150);
    ui->tableView->setColumnWidth(1, 150);
    ui->tableView->setColumnWidth(2, 150);
    ui->tableView->setColumnWidth(3, 150);
    ui->tableView->setColumnWidth(4, 150);
}

void
PriceTableDialog::openWindow(QString contractNumber)
{
    ALOGDTRACE();
    mContractNumber = contractNumber;
    initTableView();
    this->exec();
}

void
PriceTableDialog::initTableView()
{
    int size;
    QList<CONTRACT_PRICE> prices;
    CONTRACT_PRICE price;

    if (mModel->rowCount() > 0)
        mModel->removeRows(0, mModel->rowCount());

    if (!mDb->getAllContractPriceData(mContractNumber, prices)) {
        size = prices.size();
        for (int i = 0; i < size; i++) {
            price = prices.at(i);
            addPriceItem(price);
        }
    }
}

void
PriceTableDialog::addPriceItem(CONTRACT_PRICE price)
{
    ALOGDTRACE();
    char buf[64];

    QStandardItem* num
            = new QStandardItem(price.number);
    QStandardItem* contractNum
            = new QStandardItem(mContractNumber);
    QStandardItem* PumpType
            = new QStandardItem(Car::getPumpTypeStr(price.pumpType));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", price.squarePrice);
    QStandardItem *squarePrice
            = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", price.standardPrice);
    QStandardItem *standardPrice
            = new QStandardItem(QString(buf));

    QList<QStandardItem*> items;
    items << num << contractNum << PumpType << standardPrice;
    mModel->appendRow(items);
}

void PriceTableDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    ALOGDTRACE();
    CONTRACT_PRICE price;
    int curRow = ui->tableView->currentIndex().row();
    QString number = mModel->index(curRow, 0).data().toString();
    if (!mDb->getPriceInNumber(number, price)) {
        emit selectedPrice(price);
    }
    this->close();
}
