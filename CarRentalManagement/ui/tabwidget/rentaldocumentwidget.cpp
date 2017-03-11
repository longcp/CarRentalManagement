#include "rentaldocumentwidget.h"
#include "ui_rentaldocumentwidget.h"
#include "rentaldocumenteditdialog.h"
#include <QToolBar>
#include <tablemodel.h>
#include <contract.h>

#define LOG_TAG                 "RENTAL_DOC_WIDGET"
#include "utils/Log.h"

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

    mRentalDocEditDialog = new RentalDocumentEditDialog();

    initView();

    mActAdd = new QAction(QIcon(":/menu/icon/add_64.ico"),
                          tr("增加"), this);
    mActEdit = new QAction(QIcon(":/menu/icon/edit_64.ico"),
                          tr("修改"), this);
    mActDelete = new QAction(QIcon(":/menu/icon/delete_64.ico"),
                             tr("删除"), this);
    mActSearch = new QAction(QIcon(":/menu/icon/search_64.ico"),
                             tr("查询"), this);
    mActExport = new QAction(QIcon(":/menu/icon/export_64.ico"),
                              tr("导出"), this);

    mToolBar = new QToolBar(tr("rentalDocToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);

    ui->toolBarHorizonLayout->addWidget(mToolBar);

    /**
     * @brief 打开编辑窗口
     */
    connect(mActAdd, SIGNAL(triggered()),
            this, SLOT(addRentalDocSlot()));
    /**
     * @brief 单元格双击事件
     */
    connect(ui->docTableview, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(cellDoubleClickedSlot(const QModelIndex &)));
    /**
     * @brief 打开编辑窗口
     */
    connect(this, SIGNAL(openRentalEditDialogSignal()),
            mRentalDocEditDialog, SLOT(openRentalDocEditDialogSlot()));
}

RentalDocumentWidget::~RentalDocumentWidget()
{
    delete ui;
}

void
RentalDocumentWidget::configToolBar()
{
    if (!mToolBar)
        return;

    mToolBar->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    mToolBar->setLayoutDirection(Qt::LayoutDirectionAuto);
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
RentalDocumentWidget::cellDoubleClickedSlot(const QModelIndex &index)
{
    ALOGD("%s, a = %d, b = %d", __FUNCTION__,
          index.column(), index.row());
    emit openRentalEditDialogSignal();
}

void
RentalDocumentWidget::initView()
{
    //设置首行标题
    QStringList headerList;
    headerList << "签单号" << "承租单位/客户" << "工程名称" << "工程地址"
               << "日期" << "车牌型号" << "泵式" << "施工部位"
               << "混凝土标号" << "开工燃油" << "收工燃油"
               << "进场时间" << "工作时长" << "泵送方量(m3)"
               << "方量单价(元)" << "泵送台班数" << "台板价格(元)"
               << "现场负责人" << "负责人联系电话" << "当班司机1"
               << "当班司机2" << "当班司机3" << "备注";

    mModel = new TableModel(0, headerList.size());
    ui->docTableview->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->docTableview->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->docTableview->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->docTableview->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->docTableview->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->docTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->docTableview->setAlternatingRowColors(true);
    ui->docTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

//    initTableView();
}

void
RentalDocumentWidget::addRentalDocSlot()
{
    Contract contract;
    emit openRentalEditDialogSignal();
}
