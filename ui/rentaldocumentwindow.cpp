#include "rentaldocumentwindow.h"
#include "ui_rentaldocumentwindow.h"
#include <QToolBar>
#include <QTableWidget>

RentalDocumentWindow::RentalDocumentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RentalDocumentWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("泵送签证单");
    QStringList clientWidgetHeader;
    clientWidgetHeader << "日期"          << "车牌号"        << "机械型号(泵式)"
                       << "承租单位"       << "工程名称"      << "施工部位"
                       << "地址"          << "泵送方量"      << "到达工地时间"
                       << "开始作业时间"    << "结束作业时间"   << "作业时长"
                       << "燃油记录(开工)"  << "燃油记录(收工)" << "工程总价"
                       << "混凝土标号"      << "现场负责人"    << "负责人联系电话"
                       << "当班司机1"       << "当班司机2"    << "当班司机3";
    mRentalDocWidget = new QTableWidget(3, clientWidgetHeader.length());
    mRentalDocWidget->setHorizontalHeaderLabels(clientWidgetHeader);
    mRentalDocWidget->setItem(0,1,new QTableWidgetItem("Jan"));
    mRentalDocWidget->setItem(1,1,new QTableWidgetItem("Feb"));
    mRentalDocWidget->setItem(2,1,new QTableWidgetItem("Mar"));
//    mRentalDocWidget->rowCount();
//    mRentalDocWidget->insertRow(xxx);

    mToolBar = new QToolBar(tr("clientToolBar"), this);
    mToolBar->setStyleSheet("background-color: rgb(234,234,234);color:rgb(0,0,0);");
    mToolBar->setLayoutDirection(Qt::LayoutDirectionAuto);
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

    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActPrinter);

    ui->centralVerticalLayout->addWidget(mToolBar);
    ui->centralVerticalLayout->addWidget(mRentalDocWidget);
}

RentalDocumentWindow::~RentalDocumentWindow()
{
    delete ui;
}
