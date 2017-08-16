#include "rentaldoctabledialog.h"
#include "ui_rentaldoctabledialog.h"
#include <datatype.h>
#include <database/database.h>
#include <rentaldocument.h>
#include <tablemodel.h>

#define LOG_TAG                 "RENTALDOC_TABLE_DIALOG"
#include "utils/Log.h"

RentalDocTableDialog::RentalDocTableDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::RentalDocTableDialog)
{
    ui->setupUi(this);
    setWindowTitle("选择签证单");
    initView();
}

RentalDocTableDialog::~RentalDocTableDialog()
{
    delete ui;
}

void
RentalDocTableDialog::initView()
{
    //设置首行标题
    QStringList headerList;
    headerList << "签单号" << "工程名称" << "日期" << "泵送方量(m3)"
               << "方量单价(元)" << "泵送台班数" << "台班价格(元)";

    mModel = new TableModel(0, headerList.size());
    ui->rentablDocTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->rentablDocTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->rentablDocTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->rentablDocTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->rentablDocTableView->verticalHeader()->setVisible(false);              //隐藏行表头
    ui->rentablDocTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->rentablDocTableView->setAlternatingRowColors(true);
    ui->rentablDocTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    ui->rentablDocTableView->setSortingEnabled(true);                          //点击表头排序

    ui->rentablDocTableView->horizontalHeader()
            ->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void
RentalDocTableDialog::openWindow()
{
    ALOGDTRACE();
    initTableView();
    this->exec();
}

void
RentalDocTableDialog::initTableView()
{
    int size;
    QList<RentalDocument> docs;
    RentalDocument doc;

    if (mModel->rowCount() > 0)
        mModel->removeRows(0, mModel->rowCount());

    if (!mDb->getAllRentalDocumentData(docs)) {
        size = docs.size();
        for (int i = 0; i < size; i++) {
            doc = docs.at(i);
            addRentalDocItem(doc);
        }
    }
}

void
RentalDocTableDialog::addRentalDocItem(RentalDocument &doc)
{
    char buf[64];

    QStandardItem *docNum = new QStandardItem(doc.number);
    QStandardItem *projectName = new QStandardItem(doc.projectName);
    QStandardItem *date = new QStandardItem(doc.date.toString(DATE_FORMAT_STR));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.pumpSquare);
    QStandardItem *pumpSquare = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.squareUnitPrice);
    QStandardItem *squareUnitPrice = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.pumpTimes);
    QStandardItem *pumpTimes = new QStandardItem(QString(buf));

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2f", doc.pumpTimeUnitPrice);
    QStandardItem *pumpTimeUnitPrice = new QStandardItem(QString(buf));

    QList<QStandardItem*> items;
    items << docNum  << projectName  << date << pumpSquare
          << squareUnitPrice << pumpTimes << pumpTimeUnitPrice;
    mModel->appendRow(items);
}

void
RentalDocTableDialog::on_rentablDocTableView_doubleClicked(const QModelIndex &index)
{
    ALOGDTRACE();
    int curRow = ui->rentablDocTableView->currentIndex().row();
    emit selectedDoc(mModel->index(curRow, 0).data().toString());
    this->close();
}
