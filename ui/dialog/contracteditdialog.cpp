#include "contracteditdialog.h"
#include "ui_contracteditdialog.h"
#include <database/database.h>
#include <QAction>
#include <QToolBar>
#include <tablemodel.h>

ContractEditDialog::ContractEditDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    ui(new Ui::ContractEditDialog)
{
    ui->setupUi(this);

    initView();

    mActSave = new QAction(QIcon(":/menu/icon/save_64.ico"),
                           tr("保存"), this);
    mActEdit = new QAction(QIcon(":/menu/icon/edit_64.ico"),
                           tr("修改"), this);
    mActPrev = new QAction(QIcon(":/menu/icon/arrow_left_64.ico"),
                           tr("上一条"), this);
    mActNext = new QAction(QIcon(":/menu/icon/arrow_right_64.ico"),
                           tr("下一条"), this);
    mActExit = new QAction(QIcon(":/menu/icon/exit_out_64.ico"),
                           tr("退出"), this);
    mActCancel = new QAction(QIcon(":/menu/icon/cancel_64.ico"),
                             tr("取消"), this);
    mActSaveExit = new QAction(QIcon(":/menu/icon/ok_64.ico"),
                               tr("保存退出"), this);

    mToolBar = new QToolBar(tr("contractEditToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActPrev);
    mToolBar->addAction(mActNext);
    mToolBar->addAction(mActCancel);
    mToolBar->addAction(mActExit);
    ui->toolBarHorizontalLayout->addWidget(mToolBar);
}

ContractEditDialog::~ContractEditDialog()
{
    delete ui;
}

void
ContractEditDialog::initView()
{
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle("编辑合同");
    this->setFixedSize(1300, 700);

    initPriceTableView();
}

void
ContractEditDialog::configToolBar()
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
ContractEditDialog::openContractEditDialogSlot()
{
    this->exec();
}

void
ContractEditDialog::initPriceTableView()
{
    QStringList headerList;
    headerList << "编号" << "泵式" << "方量价格" << "标准台班价格"
               << "2.5小时内台班价格" << "4小时内台班价格" << "备注";

    mModel = new TableModel(0, headerList.size());
    ui->priceTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑
    ui->priceTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->priceTableView->verticalHeader()->setVisible(false);            //隐藏行表头
    ui->priceTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->priceTableView->setAlternatingRowColors(true);
    ui->priceTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->priceTableView->resizeColumnToContents(4);                      //自动适应列宽
    ui->priceTableView->resizeColumnToContents(5);
    ui->priceTableView->horizontalHeader()
            ->setSectionResizeMode(6, QHeaderView::Stretch);                  //自动适应列宽
}

void
ContractEditDialog::on_addBtn_clicked()
{

}

void
ContractEditDialog::on_deleteBtn_clicked()
{

}
