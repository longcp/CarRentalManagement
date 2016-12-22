#include "rentaldocumentwidget.h"
#include "ui_rentaldocumentwidget.h"
#include <QToolBar>

RentalDocumentWidget::RentalDocumentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RentalDocumentWidget)
{
    ui->setupUi(this);
    ui->statusFrame->setStyleSheet(
                "background-color: rgb(240,240,240);color:rgb(0,0,0);");
    ui->toolbarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle("泵送签证单");

    mToolBar = new QToolBar(tr("clientToolBar"), this);
    mToolBar->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
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

    ui->toolBarHorizonLayout->addWidget(mToolBar);
}

RentalDocumentWidget::~RentalDocumentWidget()
{
    delete ui;
}
