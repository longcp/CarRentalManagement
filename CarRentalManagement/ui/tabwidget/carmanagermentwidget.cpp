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
#include "util.h"

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
    configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
//    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);
    mToolBar->addAction(mActInsurance);
    mToolBar->addAction(mActAnnual);
    ui->toolBarHorizontalLayout->addWidget(mToolBar);

    initView();

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
    connect(mActAdd, SIGNAL(triggered()), this, SLOT(addCarSlot()));
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
    connect(mActEdit, SIGNAL(triggered()), this, SLOT(editCarItemSlot()));
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

    /**
     * @brief 同步调整列宽
     */
    connect(ui->carTableView->horizontalHeader(),&QHeaderView::sectionResized,
            this, &CarManagermentWidget::updateSumTabSectionWidth);
    /**
     * @brief 根据进度条值同步列表位置
     */
    connect((QObject*)ui->sumTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->carTableView->horizontalScrollBar(), SLOT(setValue(int)));
}

CarManagermentWidget::~CarManagermentWidget()
{
    delete ui;
}

void
CarManagermentWidget::initView()
{
    initCarTableView();
    initSumTableView();
    initCarTableViewData();
}

void
CarManagermentWidget::initCarTableView()
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
    ui->carTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->carTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->carTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->carTableView->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->carTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");             //表头颜色

    ui->carTableView->setAlternatingRowColors(true);
    ui->carTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");  //设置间隔行颜色变化

    //隐藏滚动条
    ui->carTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->carTableView->setSortingEnabled(true);

    ui->carTableView->setColumnWidth(CAR_COL_ENGINEPOWER, 200);
    ui->carTableView->setColumnWidth(CAR_COL_MAXDELIVERYSIZES, 200);
    ui->carTableView->setColumnWidth(CAR_COL_MAXOUTPUTPRESSURE, 200);
    ui->carTableView->setColumnWidth(CAR_COL_BOOMVERTICALLEN, 200);
    ui->carTableView->setColumnWidth(CAR_COL_BOOMHORIZONTALLEN, 200);
    ui->carTableView->setColumnWidth(CAR_COL_TOTALWEIGHT, 200);
    ui->carTableView->setColumnWidth(CAR_COL_EQUIPMENTWEIGHT, 200);
}

void
CarManagermentWidget::initSumTableView()
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

    mSumModel = new TableModel(0, headerList.size());
    ui->sumTableView->setModel(mSumModel);
    mSumModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->sumTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->sumTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sumTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->sumTableView->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->sumTableView->horizontalHeader()->setVisible(false);   //隐藏列表头
    ui->sumTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");             //表头颜色

    ui->sumTableView->verticalHeader()->setDefaultSectionSize(20);
    ui->sumTableView->setAlternatingRowColors(true);
    ui->sumTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}"); //设置间隔行颜色变化
    ui->carTableView->setColumnWidth(CAR_COL_ENGINEPOWER, 200);
    ui->carTableView->setColumnWidth(CAR_COL_MAXDELIVERYSIZES, 200);
    ui->carTableView->setColumnWidth(CAR_COL_MAXOUTPUTPRESSURE, 200);
    ui->carTableView->setColumnWidth(CAR_COL_BOOMVERTICALLEN, 200);
    ui->carTableView->setColumnWidth(CAR_COL_BOOMHORIZONTALLEN, 200);
    ui->carTableView->setColumnWidth(CAR_COL_TOTALWEIGHT, 200);
    ui->carTableView->setColumnWidth(CAR_COL_EQUIPMENTWEIGHT, 200);

    QStandardItem* sumStrItem = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sumStrItem;
    mSumModel->appendRow(items);
    clearSumTableData();
}

void
CarManagermentWidget::clearSumTableData()
{
    setSumPumpedSquareCellValue(0);
    setSumPumpedTimeCellValue(0);
    setSumMilageCellValue(0);
    sumUpdateRowCount();
}

void
CarManagermentWidget::initCarTableViewData()
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
CarManagermentWidget::updateSumTabSectionWidth(int logicalIndex, int, int newSize)
{
    ui->sumTableView->setColumnWidth(logicalIndex, newSize);
}

void
CarManagermentWidget::setSumPumpedSquareCellValue(double value)
{
    mSumModel->setData(mSumModel->index(0, CAR_COL_PUMPEDSQUARE), Util::doubleToDecimal2String(value));
    mCurSumPumpedSquareValue = value;
}

void
CarManagermentWidget::setSumPumpedTimeCellValue(double value)
{
    mSumModel->setData(mSumModel->index(0, CAR_COL_PUMPEDTIMES), Util::doubleToDecimal2String(value));
    mCurSumPumpedTimeValue = value;
}

void
CarManagermentWidget::setSumMilageCellValue(double value)
{
    mSumModel->setData(mSumModel->index(0, CAR_COL_MILAGE), Util::doubleToDecimal2String(value));
    mCurSumMilageValue = value;
}

void
CarManagermentWidget::sumPumpedSquareCellAddValue(double value)
{
    setSumPumpedSquareCellValue(mCurSumPumpedSquareValue+value);
}

void
CarManagermentWidget::sumPumpedTimeCellAddValue(double value)
{
    setSumPumpedTimeCellValue(mCurSumPumpedTimeValue+value);
}

void
CarManagermentWidget::sumMilageCellAddValue(double value)
{
    setSumMilageCellValue(mCurSumMilageValue+value);
}

void
CarManagermentWidget::sumPumpedSquareCellDelValue(double value)
{
    setSumPumpedSquareCellValue(mCurSumPumpedSquareValue-value);
}

void
CarManagermentWidget::sumPumpedTimeCellDelValue(double value)
{
    setSumPumpedTimeCellValue(mCurSumPumpedTimeValue-value);
}

void
CarManagermentWidget::sumMilageCellDelValue(double value)
{
    setSumMilageCellValue(mCurSumMilageValue-value);
}

void
CarManagermentWidget::sumUpdateRowCount()
{
    int rows = 0;
    if (mModel->rowCount() > 0)
        rows = mModel->rowCount();

    mSumModel->setData(mSumModel->index(0, 1), QString::number(rows));
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
    QStandardItem* number = new QStandardItem(car.number);
    QStandardItem* pumptype = new QStandardItem(car.getPumpTypeStr(car.pumptype));
    QStandardItem* carBrand = new QStandardItem(car.carBrand);
    QStandardItem* chassisBrand = new QStandardItem(car.chassisBrand);
    QStandardItem* drivingLicenseDate = new QStandardItem(car.drivingLicenseDate.toString(DATE_FORMAT_STR));
    QStandardItem* fuelCarNumber = new QStandardItem(car.fuelCarNumber);
    QStandardItem* frameNumber = new QStandardItem(car.frameNumber);
    QStandardItem* identificationNumber = new QStandardItem(car.identificationNumber);
    QStandardItem* productNumber = new QStandardItem(car.productNumber);
    QStandardItem* insuranceCardNumber = new QStandardItem(car.insuranceCardNumber);
    QStandardItem* engineNumber = new QStandardItem(car.engineNumber);
    QStandardItem* dimensions = new QStandardItem(car.dimensions);
    QStandardItem* productionDate = new QStandardItem(car.productionDate.toString(DATE_FORMAT_STR));
    QStandardItem* factoryCode = new QStandardItem(car.factoryCode);
    QStandardItem* operator1 = new QStandardItem(car.operator1);
    QStandardItem* operator2 = new QStandardItem(car.operator2);
    QStandardItem* operator3 = new QStandardItem(car.operator3);
    QStandardItem* operator4 = new QStandardItem(car.operator4);
    QStandardItem* operator5 = new QStandardItem(car.operator5);
    QStandardItem* remarks = new QStandardItem(car.remarks);
    QStandardItem* pumpedSquare = new QStandardItem(Util::doubleToDecimal2String(car.pumpedSquare));
    QStandardItem* pumpedTimes  = new QStandardItem(Util::doubleToDecimal2String(car.pumpedTimes));
    QStandardItem* milage = new QStandardItem(Util::doubleToDecimal2String(car.milage));
    QStandardItem* worth = new QStandardItem(Util::doubleToDecimal2String(car.worth));
    QStandardItem* enginePower = new QStandardItem(Util::doubleToDecimal2String(car.enginePower));
    QStandardItem* maxDeliverySizes = new QStandardItem(Util::doubleToDecimal2String(car.maxDeliverySizes));
    QStandardItem* maxOutputPressure = new QStandardItem(Util::doubleToDecimal2String(car.maxOutputPressure));
    QStandardItem* boomVerticalLen = new QStandardItem(Util::doubleToDecimal2String(car.boomVerticalLen));
    QStandardItem* boomHorizontalLen = new QStandardItem(Util::doubleToDecimal2String(car.boomHorizontalLen));
    QStandardItem* totalWeight = new QStandardItem(Util::doubleToDecimal2String(car.totalWeight));
    QStandardItem* equipmentWeight = new QStandardItem(Util::doubleToDecimal2String(car.equipmentWeight));

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

    //更新合计
    sumPumpedSquareCellAddValue(car.pumpedSquare);
    sumPumpedTimeCellAddValue(car.pumpedTimes);
    sumMilageCellAddValue(car.milage);
    sumUpdateRowCount();
}

void
CarManagermentWidget::updateCarItemSlot(Car &car)
{
    //更新合计
    double squares = mModel->index(curRow, CAR_COL_PUMPEDSQUARE).data().toDouble();
    double times = mModel->index(curRow, CAR_COL_PUMPEDTIMES).data().toDouble();
    double milage = mModel->index(curRow, CAR_COL_MILAGE).data().toDouble();
    sumPumpedSquareCellAddValue(car.pumpedSquare - squares);
    sumPumpedTimeCellAddValue(car.pumpedTimes - times);
    sumMilageCellAddValue(car.milage - milage);

    mModel->setData(mModel->index(curRow, CAR_COL_NUM), car.number);
    mModel->setData(mModel->index(curRow, CAR_COL_PUMPTYPE), car.getPumpTypeStr(car.pumptype));
    mModel->setData(mModel->index(curRow, CAR_COL_PUMPEDSQUARE), Util::doubleToDecimal2String(car.pumpedSquare));
    mModel->setData(mModel->index(curRow, CAR_COL_PUMPEDTIMES), Util::doubleToDecimal2String(car.pumpedTimes));
    mModel->setData(mModel->index(curRow, CAR_COL_MILAGE), Util::doubleToDecimal2String(car.milage));

    mModel->setData(mModel->index(curRow, CAR_COL_CARBRAND), car.carBrand);
    mModel->setData(mModel->index(curRow, CAR_COL_CHASSISBRAND), car.chassisBrand);
    mModel->setData(mModel->index(curRow, CAR_COL_DRIVINGLICENSEDATE),
                    car.drivingLicenseDate.toString(DATE_FORMAT_STR));
    mModel->setData(mModel->index(curRow, CAR_COL_FUELCARNUMBER), car.fuelCarNumber);
    mModel->setData(mModel->index(curRow, CAR_COL_FRAMENUMBER), car.frameNumber);
    mModel->setData(mModel->index(curRow, CAR_COL_IDENTIFICATIONNUMBER), car.identificationNumber);
    mModel->setData(mModel->index(curRow, CAR_COL_PRODUCTNUMBER), car.productNumber);
    mModel->setData(mModel->index(curRow, CAR_COL_INSURANCECARDNUMBER), car.insuranceCardNumber);
    mModel->setData(mModel->index(curRow, CAR_COL_ENGINENUMBER), car.engineNumber);
    mModel->setData(mModel->index(curRow, CAR_COL_WORTH), Util::doubleToDecimal2String(car.worth));
    mModel->setData(mModel->index(curRow, CAR_COL_ENGINEPOWER), Util::doubleToDecimal2String(car.enginePower));
    mModel->setData(mModel->index(curRow, CAR_COL_MAXDELIVERYSIZES), Util::doubleToDecimal2String(car.maxDeliverySizes));
    mModel->setData(mModel->index(curRow, CAR_COL_MAXOUTPUTPRESSURE), Util::doubleToDecimal2String(car.maxOutputPressure));
    mModel->setData(mModel->index(curRow, CAR_COL_DIMENSIONS), car.dimensions);
    mModel->setData(mModel->index(curRow, CAR_COL_BOOMVERTICALLEN), Util::doubleToDecimal2String(car.boomVerticalLen));
    mModel->setData(mModel->index(curRow, CAR_COL_BOOMHORIZONTALLEN), Util::doubleToDecimal2String(car.boomHorizontalLen));
    mModel->setData(mModel->index(curRow, CAR_COL_TOTALWEIGHT), Util::doubleToDecimal2String(car.totalWeight));
    mModel->setData(mModel->index(curRow, CAR_COL_EQUIPMENTWEIGHT), Util::doubleToDecimal2String(car.equipmentWeight));
    mModel->setData(mModel->index(curRow, CAR_COL_PRODUCTIONDATE),
                    car.productionDate.toString(DATE_FORMAT_STR));
    mModel->setData(mModel->index(curRow, CAR_COL_FACTORYCODE), car.factoryCode);
    mModel->setData(mModel->index(curRow, CAR_COL_OPERATOR1), car.operator1);
    mModel->setData(mModel->index(curRow, CAR_COL_OPERATOR2), car.operator2);
    mModel->setData(mModel->index(curRow, CAR_COL_OPERATOR3), car.operator3);
    mModel->setData(mModel->index(curRow, CAR_COL_OPERATOR4), car.operator4);
    mModel->setData(mModel->index(curRow, CAR_COL_OPERATOR5), car.operator5);
    mModel->setData(mModel->index(curRow, CAR_COL_REMARKS), car.remarks);
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
    number = mModel->index(curRow, CAR_COL_NUM).data().toString();
    double squares = mModel->index(curRow, CAR_COL_PUMPEDSQUARE).data().toDouble();
    double times = mModel->index(curRow, CAR_COL_PUMPEDTIMES).data().toDouble();
    double milage = mModel->index(curRow, CAR_COL_MILAGE).data().toDouble();
    if (!mDb->deleteCarDataInNumber(number)) {
        ALOGD("%s, delete ok", __FUNCTION__);
        mModel->removeRow(curRow);
        //更新合计
        sumPumpedSquareCellDelValue(squares);
        sumPumpedTimeCellDelValue(times);
        sumMilageCellDelValue(milage);
        sumUpdateRowCount();
    }
}
