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

#define LOG_TAG                 "CAR_MANAGERMENT_WIDGET"
#include "utils/Log.h"

CarManagermentWidget::CarManagermentWidget(QWidget *parent) :
    QWidget(parent),
    mCarEditDialog(new CarEditDialog()),
    mInsuranceDialog(new InsuranceDialog()),
    mCarAnnualDialog(new CarAnnualDialog()),
    ui(new Ui::CarManagermentWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("车辆档案");

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
     * @brief 单元格双击事件
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
}

CarManagermentWidget::~CarManagermentWidget()
{
    delete ui;
}

void
CarManagermentWidget::initView()
{
    this->setWindowTitle("车辆资料");

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
               << "生产日期" << "出厂日期" << "出厂编码"
               << "是否购买保险" << "交险费" << "保险公司(交险)"
               << "购买日期(交险)" << "是否购买商险"
               << "商险费" << "保险公司(商险)"
               << "购买日期(商险)" << "年费" << "车船费"
               << "年审日期" << "操作员(1)" << "操作员(2)"
               << "操作员(3)" << "操作员(4)" << "操作员(5)" << "备注";

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
CarManagermentWidget::cellDoubleClickedSlot(const QModelIndex &index)
{
    Car car;
//    emit openCarEditDialogSignal(CREATEITEM, car);
}
