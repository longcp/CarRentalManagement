#include "contracteditdialog.h"
#include "ui_contracteditdialog.h"
#include <database/database.h>
#include <QAction>
#include <QToolBar>
#include <tablemodel.h>
#include <contractpricedialog.h>
#include <contract.h>

#define LOG_TAG                 "CONTRACT_EDIT_DIALOG"
#include "utils/Log.h"

ContractEditDialog::ContractEditDialog(QWidget *parent) :
    QDialog(parent),
    mDb(DataBase::getInstance()),
    mContractPriceDialog(new ContractPriceDialog()),
    mOriginContract(new Contract()),
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

    connect(mActEdit, SIGNAL(triggered()),
            this, SLOT(editEvent()));
    connect(this, SIGNAL(openContractPriceWindowSignal(QString)),
            mContractPriceDialog, SLOT(openWindow(QString)));
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
ContractEditDialog::openContractEditDialogSlot(OpenType opentype,
                                               Contract &contract)
{
    mOpenType = opentype;
    if (opentype == OpenType::CREATEITEM) {
        //以创建条目方式打开
        mActEdit->setDisabled(true);
        mActPrev->setDisabled(true);
        mActNext->setDisabled(true);
        ui->signedDateDE->setDate(QDate::currentDate());
        ui->startDateDE->setDate(QDate::currentDate());
        ui->endDateDE->setDate(QDate::currentDate());
        ui->createDateDE->setDate(QDate::currentDate());
        setEditMode();
        mActSave->setDisabled(true);
        mActCancel->setDisabled(true);
    } else {
        //以查看内容方式打开
        mActSave->setEnabled(true);
        mActPrev->setEnabled(true);
        mActNext->setEnabled(true);
        ui->contractNumberLE->setDisabled(true);
        setViewMode();
        setOriginContract(contract);
        setView(contract);
    }
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
            ->setSectionResizeMode(6, QHeaderView::Stretch);            //自动适应列宽
}

void
ContractEditDialog::setEditMode()
{
    if (mOpenType == OpenType::CREATEITEM)
        ui->contractNumberLE->setFocus();
    else
        ui->projectNameLE->setFocus();

    mActEdit->setDisabled(true);
    mActCancel->setEnabled(true);
    mActSave->setEnabled(true);
    setMode(true);
}

void
ContractEditDialog::setViewMode()
{
    mActCancel->setDisabled(true);
    mActEdit->setEnabled(true);
    mActSave->setDisabled(true);
    setMode(false);
}

void
ContractEditDialog::setMode(bool mode)
{
    ui->contractNumberLE->setEnabled(mode);
    ui->signedDateDE->setEnabled(mode);
    ui->clientNameCB->setEnabled(mode);
    ui->clientNumberLebel->setEnabled(mode);
    ui->projectNameLE->setEnabled(mode);
    ui->projectAddrLE->setEnabled(mode);
    ui->deliveryDSB->setEnabled(mode);
    ui->structureLevelDSB->setEnabled(mode);
    ui->startDateDE->setEnabled(mode);
    ui->endDateDE->setEnabled(mode);
    ui->requireTE->setEnabled(mode);
    ui->supplementTE->setEnabled(mode);
    ui->remarksTE->setEnabled(mode);
    ui->isIncludeTexCB->setEnabled(mode);
    ui->texRateSB->setEnabled(mode);
    ui->creatorLE->setEnabled(mode);
    ui->createDateDE->setEnabled(mode);
    ui->addBtn->setEnabled(mode);
    ui->deleteBtn->setEnabled(mode);
}

void
ContractEditDialog::setOriginContract(Contract &contract)
{
    ALOGDTRACE();
    mOriginContract->copy(contract);
}

void
ContractEditDialog::setView(Contract &contract)
{

}

void
ContractEditDialog::editEvent()
{
    setEditMode();
    //已存在的合同，其合同编码不可再次编辑
    ui->contractNumberLE->setDisabled(true);
}

void
ContractEditDialog::on_addBtn_clicked()
{
    ALOGDTRACE();
    emit openContractPriceWindowSignal("");
}

void
ContractEditDialog::on_deleteBtn_clicked()
{

}
