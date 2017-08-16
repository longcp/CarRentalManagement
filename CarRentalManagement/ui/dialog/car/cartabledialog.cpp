#include "cartabledialog.h"
#include "ui_cartabledialog.h"
#include <car.h>
#include <database/database.h>
#include <tablemodel.h>

#define LOG_TAG                 "CAR_TABLE_DIALOG"
#include "utils/Log.h"

CarTableDialog::CarTableDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::CarTableDialog)
{
    ui->setupUi(this);
    this->setFixedSize(750, 700);
    initView();
}

CarTableDialog::~CarTableDialog()
{
    delete ui;
}


void
CarTableDialog::initView()
{
    this->setWindowTitle("选择车辆");

    //设置首行标题
    QStringList          headerList;
    headerList << "车号" << "车牌号" << "泵式" << "已泵送方量数"
               << "已泵送台班数";

    mModel = new TableModel(0, headerList.size());
    ui->carTableview->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->carTableview->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->carTableview->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->carTableview->setSelectionMode(
                QAbstractItemView::SingleSelection);
    ui->carTableview->horizontalHeader()->setSectionResizeMode(headerList.size()-1, QHeaderView::Stretch);

    ui->carTableview->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->carTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->carTableview->setAlternatingRowColors(true);
    ui->carTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    ui->carTableview->setSortingEnabled(true);

//    ui->carTableview->horizontalHeader()
//            ->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->carTableview->setColumnWidth(0, 150);
    ui->carTableview->setColumnWidth(1, 150);
    ui->carTableview->setColumnWidth(2, 150);
    ui->carTableview->setColumnWidth(3, 150);
    ui->carTableview->setColumnWidth(4, 150);
}

void
CarTableDialog::openWindow()
{
    ALOGDTRACE();
    initTableView();
    this->exec();
}

void
CarTableDialog::initTableView()
{
    int size;
    QList<Car>cars;
    Car car;

    if (mModel->rowCount() > 0)
        mModel->removeRows(0, mModel->rowCount());

    if (!mDb->getAllCarData(cars)) {
        size = cars.size();
        for (int i = 0; i < size; i++) {
            car = cars.at(i);
            addCarItem(car);
        }
    }
}

void
CarTableDialog::addCarItem(Car &car)
{
    ALOGDTRACE();
    QStandardItem* num
            = new QStandardItem(car.number);
    QStandardItem* carNumber
            = new QStandardItem(car.carNumber);
    QStandardItem* pumptype
            = new QStandardItem(Car::getPumpTypeStr(car.pumptype));
    QStandardItem* pumpedSquare
            = new QStandardItem(QString("%1").arg(car.pumpedSquare));
    QStandardItem* pumpedTimes
            = new QStandardItem(QString("%1").arg(car.pumpedTimes));

    QList<QStandardItem*> items;
    items << num << carNumber << pumptype << pumpedSquare << pumpedTimes;
    mModel->appendRow(items);
}

void
CarTableDialog::on_carTableview_doubleClicked(const QModelIndex &index)
{
    ALOGDTRACE();
    int curRow = ui->carTableview->currentIndex().row();
    emit selectedCar(mModel->index(curRow, 0).data().toString());
    this->close();
}
