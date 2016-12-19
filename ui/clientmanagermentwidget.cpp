#include "clientmanagermentwidget.h"
#include "ui_clientmanagermentwidget.h"
#include <QToolBar>
#include <QTableWidget>

#define LOG_TAG                 "CLIENT_MANAGERMENT_WIDGET"
#include "utils/Log.h"

ClientManagermentWidget::ClientManagermentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientManagermentWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("客户资料");
    QStringList clientWidgetHeader;
    clientWidgetHeader << "客户编号" << "客户名称" << "地址"
                       << "联系电话" << "传真" << "联系人"
                       << "结账方式" << "月结日" << "工程款额"
                       << "已付款额" << "余额" << "备注";
    mClientWidget = new QTableWidget(3, clientWidgetHeader.length());
    mClientWidget->setHorizontalHeaderLabels(clientWidgetHeader);
    mClientWidget->setItem(0,1,new QTableWidgetItem("Jan"));
    mClientWidget->setItem(1,1,new QTableWidgetItem("Feb"));
    mClientWidget->setItem(2,1,new QTableWidgetItem("Mar"));
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
    mToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

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

    ui->clientVerticalLayout->addWidget(mToolBar);
    ui->clientVerticalLayout->addWidget(mClientWidget);
}

ClientManagermentWidget::~ClientManagermentWidget()
{
    delete ui;
}
