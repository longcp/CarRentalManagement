#include "carmanagermentwidget.h"
#include "ui_carmanagermentwidget.h"
#include <QTableWidget>
#include <QToolBar>
#include <careditdialog.h>
#include <tablemodel.h>
#include <car.h>
#include <insurancedialog.h>
#include <carannualdialog.h>
#include <datatype.h>
#include <database/database.h>
#include <QMessageBox>

#define LOG_TAG                 "CAR_MANAGERMENT_WIDGET"
#include "utils/Log.h"

CarManagermentWidget::CarManagermentWidget(QWidget *parent) :
    QWidget(parent),
    curRow(-1),
    mCarEditDialog(new CarEditDialog()),
    mInsuranceDialog(new InsuranceDialog()),
    mCarAnnualDialog(new CarAnnualDialog()),
    ui(new Ui::CarManagermentWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(TAB_TITLE_CAR_INFOMATION);
    mDb = DataBase::getInstance();

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
    mActInsurance = new QAction(QIcon(":/menu/icon/insurance_add_64.ico"),
                                tr("购买保险"), this);
    mActAnnual = new QAction(QIcon(":/menu/icon/annual_64.ico"),
                             tr("车辆年审"), this);

    mToolBar = new QToolBar(tr("carToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);
    mToolBar->addAction(mActInsurance);
    mToolBar->addAction(mActAnnual);

    ui->toolBarHorizontalLayout->addWidget(mToolBar);

    /**
     * @brief 单元格双击事件
     */
    connect(ui->carTableView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(cellDoubleClickedSlot(const QModelIndex &)));
    /**
     * @brief 打开车辆资料编辑窗口
     */
    connect(this, SIGNAL(openCarEditDialogSignal(OpenType, Car&)),
            mCarEditDialog, SLOT(openCarEditDialogSlot(OpenType, Car&)));
    /**
     * @brief 打开编辑窗口
     */
    connect(mActAdd, SIGNAL(triggered()),
            this, SLOT(addCarSlot()));
    /**
     * @brief 打开购买保险窗口
     */
    connect(mActInsurance, SIGNAL(triggered()),
            mInsuranceDialog, SLOT(openWindow()));
    /**
     * @brief 打开车辆年审窗口
     */
    connect(mActAnnual, SIGNAL(triggered()),
            mCarAnnualDialog, SLOT(openWindow()));
    /**
     * @brief 增加条目
     */
    connect(mCarEditDialog, SIGNAL(addCarItemSignal(Car &)),
            this, SLOT(addCarItemSlot(Car&)));
    /**
     * @brief 更新条目
     */
    connect(mCarEditDialog, SIGNAL(updateCarItemSignal(Car&)),
            this, SLOT(updateCarItemSlot(Car&)));
    /**
     * @brief 编辑条目
     */
    connect(mActEdit, SIGNAL(triggered()),
            this, SLOT(editCarItemSlot()));
    /**
     * @brief 删除条目
     */
    connect(mActDelete, SIGNAL(triggered()),
            this, SLOT(deleteCarItemSlot()));
    /**
     * @brief 添加年费条目
     */
    connect(mCarAnnualDialog, SIGNAL(addAnnualItemSignal(ANNUALFEE_RECORD &)),
            mCarEditDialog, SLOT(addAnnualItemSlot(ANNUALFEE_RECORD &)));
    /**
     * @brief 添加交险条目
     */
    connect(mInsuranceDialog, SIGNAL(addInsuranceItemSignal(INSURANCE_RECORD &)),
            mCarEditDialog, SLOT(addInsuranceItemSlot(INSURANCE_RECORD &)));
    /**
     * @brief 添加商险条目
     */
    connect(mInsuranceDialog, SIGNAL(addBusinessInsuranceItemSignal(INSURANCE_RECORD &)),
            mCarEditDialog, SLOT(addBusinessInsuranceItemSlot(INSURANCE_RECORD &)));
}

CarManagermentWidget::~CarManagermentWidget()
{
    delete ui;
}

void
CarManagermentWidget::initView()
{
    //设置首行标题
    QStringList headerList;
    headerList << "车号" << "泵式" << "已泵送方量数" << "已泵送台班数"
               << "已行驶公里数" << "产品品牌" << "底盘品牌"
               << "行驶证发证日期" << "专用油卡号" << "车架号"
               << "车辆识别号" << "产品型号" << "保险卡号"
               << "发动机号" << "车身价" << "发动机额定功率(KW)"
               << "最大理论输送量(m3/h)" << "最大理论输出压力(MPA)" << "外形尺寸"
               << "臂架垂直长度(M)" << "臂架水平长度(M)"
               << "整车总质量(KG)" << "整车装备质量(KG)"
               << "生产日期" << "出厂编码"
               << "操作员(1)" << "操作员(2)" << "操作员(3)"
               << "操作员(4)" << "操作员(5)" << "备注";

    mModel = new TableModel(0, headerList.size());
    ui->carTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->carTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->carTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->carTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->carTableView->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->carTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");             //表头颜色

    ui->carTableView->setAlternatingRowColors(true);
    ui->carTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");  //设置间隔行颜色变化

    ui->carTableView->setColumnWidth(15, 200);
    ui->carTableView->setColumnWidth(16, 200);
    ui->carTableView->setColumnWidth(17, 200);
    ui->carTableView->setColumnWidth(19, 200);
    ui->carTableView->setColumnWidth(20, 200);
    ui->carTableView->setColumnWidth(21, 200);
    ui->carTableView->setColumnWidth(22, 200);

    initTableView();
}

void
CarManagermentWidget::initTableView()
{
    int size;

    Car car;
    QList<Car> cars;

    if (!mDb->getAllCarData(cars)) {
        if (cars.isEmpty())
            return;

        size = cars.size();
        for (int i = 0; i < size; i++) {
            car = cars.at(i);
            addCarItemSlot(car);
        }
    }
}

void
CarManagermentWidget::configToolBar()
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
CarManagermentWidget::addCarSlot()
{
    Car car;
    emit openCarEditDialogSignal(CREATEITEM, car);
}

void
CarManagermentWidget::editRowEvent(int row)
{
    Car car;
    QString number = mModel->index(row, 0).data().toString();
    if (mDb->getCarInNumber(number, car)) {
        ALOGE("getClientInNumber failed, sql err = %s", mDb->lastError());
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("未知错误,无法查看该项.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    emit openCarEditDialogSignal(SHOWITEM, car);
}

void
CarManagermentWidget::cellDoubleClickedSlot(const QModelIndex &index)
{
    editRowEvent(index.row());
}

void
CarManagermentWidget::editCarItemSlot()
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
CarManagermentWidget::on_carTableView_clicked(const QModelIndex &index)
{
    curRow = index.row();
}

void
CarManagermentWidget::addCarItemSlot(Car &car)
{
    QStandardItem* number
            = new QStandardItem(car.number);
    QStandardItem* pumptype
            = new QStandardItem(car.getPumpTypeStr(car.pumptype));
    QStandardItem* pumpedSquare
            = new QStandardItem(QString("%1").arg(car.pumpedSquare));
    QStandardItem* pumpedTimes
            = new QStandardItem(QString("%1").arg(car.pumpedTimes));
    QStandardItem* milage
            = new QStandardItem(QString("%1").arg(car.milage));
    QStandardItem* carBrand
            = new QStandardItem(car.carBrand);
    QStandardItem* chassisBrand
            = new QStandardItem(car.chassisBrand);
    QStandardItem* drivingLicenseDate
            = new QStandardItem(car.drivingLicenseDate.toString(DATE_FORMAT_STR));
    QStandardItem* fuelCarNumber
            = new QStandardItem(car.fuelCarNumber);
    QStandardItem* frameNumber
            = new QStandardItem(car.frameNumber);
    QStandardItem* identificationNumber
            = new QStandardItem(car.identificationNumber);
    QStandardItem* productNumber
            = new QStandardItem(car.productNumber);
    QStandardItem* insuranceCardNumber
            = new QStandardItem(car.insuranceCardNumber);
    QStandardItem* engineNumber
            = new QStandardItem(car.engineNumber);
    QStandardItem* worth
            = new QStandardItem(QString("%1").arg(car.worth));
    QStandardItem* enginePower
            = new QStandardItem(QString("%1").arg(car.enginePower));
    QStandardItem* maxDeliverySizes
            = new QStandardItem(QString("%1").arg(car.maxDeliverySizes));
    QStandardItem* maxOutputPressure
            = new QStandardItem(QString("%1").arg(car.maxOutputPressure));
    QStandardItem* dimensions
            = new QStandardItem(car.dimensions);
    QStandardItem* boomVerticalLen
            = new QStandardItem(QString("%1").arg(car.boomVerticalLen));
    QStandardItem* boomHorizontalLen
            = new QStandardItem(QString("%1").arg(car.boomHorizontalLen));
    QStandardItem* totalWeight
            = new QStandardItem(QString("%1").arg(car.totalWeight));
    QStandardItem* equipmentWeight
            = new QStandardItem(QString("%1").arg(car.equipmentWeight));
    QStandardItem* productionDate
            = new QStandardItem(car.productionDate.toString(DATE_FORMAT_STR));
    QStandardItem* factoryCode
            = new QStandardItem(car.factoryCode);
    QStandardItem* operator1
            = new QStandardItem(car.operator1);
    QStandardItem* operator2
            = new QStandardItem(car.operator2);
    QStandardItem* operator3
            = new QStandardItem(car.operator3);
    QStandardItem* operator4
            = new QStandardItem(car.operator4);
    QStandardItem* operator5
            = new QStandardItem(car.operator5);
    QStandardItem* remarks
            = new QStandardItem(car.remarks);

    QList<QStandardItem*> items;
    items << number << pumptype << pumpedSquare << pumpedTimes << milage
          << carBrand << chassisBrand << drivingLicenseDate << fuelCarNumber
          << frameNumber << identificationNumber << productNumber << insuranceCardNumber
          << engineNumber << worth << enginePower << maxDeliverySizes
          << maxOutputPressure << dimensions << boomVerticalLen << boomHorizontalLen
          << totalWeight << equipmentWeight << productionDate << factoryCode
          << operator1 << operator2 << operator3 << operator4
          << operator5 << remarks;
    mModel->appendRow(items);
}

void
CarManagermentWidget::updateCarItemSlot(Car &car)
{
    mModel->setData(mModel->index(curRow, 0),
                    car.number);
    mModel->setData(mModel->index(curRow, 1),
                    car.getPumpTypeStr(car.pumptype));
    mModel->setData(mModel->index(curRow, 2),
                    QString("%1").arg(car.pumpedSquare));
    mModel->setData(mModel->index(curRow, 3),
                    QString("%1").arg(car.pumpedTimes));
    mModel->setData(mModel->index(curRow, 4),
                    QString("%1").arg(car.milage));
    mModel->setData(mModel->index(curRow, 5),
                    car.carBrand);
    mModel->setData(mModel->index(curRow, 6),
                    car.chassisBrand);
    mModel->setData(mModel->index(curRow, 7),
                    car.drivingLicenseDate.toString(DATE_FORMAT_STR));
    mModel->setData(mModel->index(curRow, 8),
                    car.fuelCarNumber);
    mModel->setData(mModel->index(curRow, 9),
                    car.frameNumber);
    mModel->setData(mModel->index(curRow, 10),
                    car.identificationNumber);
    mModel->setData(mModel->index(curRow, 11),
                    car.productNumber);
    mModel->setData(mModel->index(curRow, 12),
                    car.insuranceCardNumber);
    mModel->setData(mModel->index(curRow, 13),
                    car.engineNumber);
    mModel->setData(mModel->index(curRow, 14),
                    QString("%1").arg(car.worth));
    mModel->setData(mModel->index(curRow, 15),
                    QString("%1").arg(car.enginePower));
    mModel->setData(mModel->index(curRow, 16),
                    QString("%1").arg(car.maxDeliverySizes));
    mModel->setData(mModel->index(curRow, 17),
                    QString("%1").arg(car.maxOutputPressure));
    mModel->setData(mModel->index(curRow, 18),
                    car.dimensions);
    mModel->setData(mModel->index(curRow, 19),
                    QString("%1").arg(car.boomVerticalLen));
    mModel->setData(mModel->index(curRow, 20),
                    QString("%1").arg(car.boomHorizontalLen));
    mModel->setData(mModel->index(curRow, 21),
                    QString("%1").arg(car.totalWeight));
    mModel->setData(mModel->index(curRow, 22),
                    QString("%1").arg(car.equipmentWeight));
    mModel->setData(mModel->index(curRow, 23),
                    car.productionDate.toString(DATE_FORMAT_STR));
    mModel->setData(mModel->index(curRow, 24),
                    car.factoryCode);
    mModel->setData(mModel->index(curRow, 25),
                    car.operator1);
    mModel->setData(mModel->index(curRow, 26),
                    car.operator2);
    mModel->setData(mModel->index(curRow, 27),
                    car.operator3);
    mModel->setData(mModel->index(curRow, 28),
                    car.operator4);
    mModel->setData(mModel->index(curRow, 29),
                    car.operator5);
    mModel->setData(mModel->index(curRow, 30),
                    car.remarks);
}

void
CarManagermentWidget::deleteCarItemSlot()
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
                                   QMessageBox::Yes |
                                   QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    QString number = "";
    number = mModel->index(curRow, 0).data().toString();
    if (!mDb->deleteCarDataInNumber(number)) {
        ALOGD("%s, delete ok", __FUNCTION__);
        mModel->removeRow(curRow);
    }
}
