#include "carmanagermentwidget.h"
#include "ui_carmanagermentwidget.h"
#include <QTableWidget>
#include <QToolBar>
#include <careditdialog.h>

#define LOG_TAG                 "CAR_MANAGERMENT_WIDGET"
#include "utils/Log.h"

CarManagermentWidget::CarManagermentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarManagermentWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("车辆档案");

    mCarEditDialog = new CarEditDialog();

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->carTableWidget->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->carTableWidget->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->carTableWidget->setSelectionMode(
                QAbstractItemView::SingleSelection);

//    ui->carTableWidget->rowCount();
//    ui->carTableWidget->insertRow(xxx);

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
    mActImport = new QAction(QIcon(":/menu/icon/import_64.ico"),
                              tr("导入"), this);
    mActPrinter = new QAction(QIcon(":/menu/icon/printer_64.ico"),
                              tr("打印"), this);

    mToolBar = new QToolBar(tr("carToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActPrinter);
    mToolBar->addAction(mActExport);
    mToolBar->addAction(mActImport);

    ui->toolBarHorizontalLayout->addWidget(mToolBar);

    /**
     * @brief 单元格双击事件
     */
    connect(ui->carTableWidget, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(cellDoubleClickedSlot(int,int)));
    /**
     * @brief 单元格双击事件
     */
    connect(this, SIGNAL(openCarEditDialogSignal()),
            mCarEditDialog, SLOT(openCarEditDialogSlot()));
}

CarManagermentWidget::~CarManagermentWidget()
{
    delete ui;
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
    mToolBar->setGeometry(0, 0, 0, 0);
    mToolBar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    mToolBar->setFocusPolicy(Qt::NoFocus);
    mToolBar->setContextMenuPolicy(Qt::DefaultContextMenu);
    mToolBar->setInputMethodHints(Qt::ImhNone);
    mToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void
CarManagermentWidget::cellDoubleClickedSlot(int a,int b)
{
    ALOGD("%s, a = %d, b = %d", __FUNCTION__, a, b);
    emit openCarEditDialogSignal();
}
