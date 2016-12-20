#include "carmanagermentwidget.h"
#include "ui_carmanagerment.h"
#include <QTableWidget>
#include <QToolBar>

CarManagermentWidget::CarManagermentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarManagermentWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("车辆档案");
    QStringList clientWidgetHeader;
    clientWidgetHeader << "车号" << "泵式" << "已泵送方量数"
                       << "已泵送台班数" << "已行驶公里数" << "产品品牌"
                       << "底盘品牌" << "行驶证发证日期" << "专用油卡号"
                       << "车架号" << "车辆识别号" << "产品型号"
                       << "保险卡号" << "发动机号" << "车身价"
                       << "发动机额定功率(KW)" << "最大理论输送量(m3/h)"
                       << "最大理论输出压力(MPA)" << "外形尺寸"
                       << "臂架垂直长度M" << "臂架水平长度M" << "整车总质量(KG)"
                       << "整车装备质量(KG)" << "生厂日期" << "出厂编码"
                       << "是否购买交险" << "交险费" << "保险公司(交险)"
                       << "购买日期(交险)" << "是否购买商险" << "保险公司(商险)"
                       << "购买日期(商险)" << "年费" << "车船费" << "年审日期"
                       << "操作员(1)" << "操作员(2)" << "操作员(3)" << "操作员(4)"
                       << "操作员(5)" << "备注";
    mCarWidget = new QTableWidget(3, clientWidgetHeader.length());
    mCarWidget->setHorizontalHeaderLabels(clientWidgetHeader);
    mCarWidget->setItem(0,1,new QTableWidgetItem("Jan"));
    mCarWidget->setItem(1,1,new QTableWidgetItem("Feb"));
    mCarWidget->setItem(2,1,new QTableWidgetItem("Mar"));
//    mClientWidget->rowCount();
//    mClientWidget->insertRow(xxx);

    mToolBar = new QToolBar(tr("clientToolBar"), this);
    mToolBar->setStyleSheet("background-color: rgb(234,234,234);color:rgb(0,0,0);");
    mToolBar->setLayoutDirection(Qt::LeftToRight);
    mToolBar->setIconSize(QSize(24,24));
    mToolBar->setOrientation(Qt::Horizontal);
    mToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    mToolBar->setFloatable(true);
    mToolBar->setMovable(true);
    mToolBar->setEnabled(true);
    mToolBar->setGeometry(0, 0, 0, 0);
    mToolBar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    mToolBar->setFocusPolicy(Qt::NoFocus);
    mToolBar->setContextMenuPolicy(Qt::DefaultContextMenu);
    mToolBar->setInputMethodHints(Qt::ImhNone);
    mToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mActAdd = new QAction(QIcon(":/menu/icon/add_64.ico"),
                          tr("增加"), this);
    mActEdit= new QAction(QIcon(":/menu/icon/edit_64.ico"),
                          tr("修改"), this);
    mActDelete = new QAction(QIcon(":/menu/icon/delete_64.ico"),
                             tr("删除"), this);
    mActSearch = new QAction(QIcon(":/menu/icon/search_64.ico"),
                             tr("查询"), this);
    mActPrinter = new QAction(QIcon(":/menu/icon/printer_64.ico"),
                              tr("打印"), this);
    mActExport = new QAction(QIcon(":/menu/icon/export_64.ico"),
                              tr("导出"), this);
    mActImport = new QAction(QIcon(":/menu/icon/import_64.ico"),
                              tr("导入"), this);

    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActPrinter);
    mToolBar->addAction(mActExport);
    mToolBar->addAction(mActImport);

    ui->mainVerticalLayout->addWidget(mToolBar);
    ui->mainVerticalLayout->addWidget(mCarWidget);
}

CarManagermentWidget::~CarManagermentWidget()
{
    delete ui;
}
