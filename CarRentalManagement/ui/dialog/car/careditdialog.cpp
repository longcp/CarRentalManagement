#include "careditdialog.h"
#include "ui_careditdialog.h"
#include <QToolBar>
#include <car.h>
#include <tablemodel.h>
#include <QDebug>
#include <QMessageBox>
#include <rentaldocument.h>
#include <database/database.h>
#include <QMenu>
#include <QAction>
#include <util.h>
#include <user.h>

#define LOG_TAG                 "CAR_EDIT_DIALOG"
#include "utils/Log.h"

//#define                         BAN_SECTION

CarEditDialog::CarEditDialog(QWidget *parent) :
    QDialog(parent),
    mOriginCar(new Car()),
    mCarNumber(""),
    mCurRow(-1),
    ui(new Ui::CarEditDialog)
{
    ui->setupUi(this);
    ui->annualTableview->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->businessTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->paymentTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setWindowFlags(this->windowFlags()
                         &~Qt::WindowMaximizeButtonHint);               //去掉最大化按钮

    mDb = DataBase::getInstance();
    mAnnualMenu = new QMenu(ui->annualTableview);
    mBusinessMenu = new QMenu(ui->businessTableView);
    mPaymentMenu = new QMenu(ui->paymentTableView);
    mAnnualDelAct = new QAction("删除", this);
    mBusinessDelAct = new QAction("删除", this);
    mPaymentDelAct = new QAction("删除", this);

    mActSave = new QAction(QIcon(":/menu/icon/save_64.ico"),
                           tr("保存"), this);
    mActEdit = new QAction(QIcon(":/menu/icon/edit_64.ico"),
                           tr("修改"), this);
    mActExit = new QAction(QIcon(":/menu/icon/exit_out_64.ico"),
                           tr("退出"), this);
    mActCancel = new QAction(QIcon(":/menu/icon/cancel_64.ico"),
                             tr("取消"), this);
    mActSaveExit = new QAction(QIcon(":/menu/icon/ok_64.ico"),
                               tr("保存退出"), this);

    mToolBar = new QToolBar(tr("carEditToolBar"), this);
    configToolBar();
    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActCancel);
    mToolBar->addAction(mActExit);

    ui->toolBarHorizontalLayout->addWidget(mToolBar);
    initView();

    /**
     * @brief 保存退出
     */
    connect(mActSaveExit, SIGNAL(triggered(bool)),
            this, SLOT(saveAndExitEvent()));
    /**
     * @brief 保存
     */
    connect(mActSave, SIGNAL(triggered(bool)),
            this, SLOT(saveEvent()));
    /**
     * @brief 退出
     */
    connect(mActExit, SIGNAL(triggered(bool)),
            this, SLOT(closeDialog()));
    /**
     * @brief 编辑模式
     */
    connect(mActEdit, SIGNAL(triggered(bool)),
            this, SLOT(editEvent()));
    /**
     * @brief 查看模式
     */
    connect(mActCancel, SIGNAL(triggered(bool)),
            this, SLOT(cancelEvent()));

    /**
     * @brief 右键删除
     */
    connect(mAnnualDelAct,    SIGNAL(triggered()),
            this, SLOT(delAnnualTableItem()));
    connect(mBusinessDelAct,    SIGNAL(triggered()),
            this, SLOT(delBusinessTableItem()));
    connect(mPaymentDelAct,    SIGNAL(triggered()),
            this, SLOT(delPaymentTableItem()));

    connect(ui->annualTableview->horizontalHeader(),&QHeaderView::sectionResized,
            this, &CarEditDialog::updateAnnualSumSectionWidth);

    connect(ui->paymentTableView->horizontalHeader(),&QHeaderView::sectionResized,
            this, &CarEditDialog::updatePaymentSumSectionWidth);

    connect(ui->projectTableView->horizontalHeader(),&QHeaderView::sectionResized,
            this, &CarEditDialog::updateProjectSumSectionWidth);

    connect(ui->businessTableView->horizontalHeader(),&QHeaderView::sectionResized,
            this, &CarEditDialog::updateBusinessSumSectionWidth);

    connect((QObject*)ui->annualSumTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->annualTableview->horizontalScrollBar(), SLOT(setValue(int)));

    connect((QObject*)ui->paymentSumTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->paymentTableView->horizontalScrollBar(), SLOT(setValue(int)));

    connect((QObject*)ui->projectSumTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->projectTableView->horizontalScrollBar(), SLOT(setValue(int)));

    connect((QObject*)ui->businessSumTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->businessTableView->horizontalScrollBar(), SLOT(setValue(int)));

    //    connect((QDialog*)ui->annualSumTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
    //            this, SLOT(updateAnnualTableviewScrollBar(int)));
    QString num = Util::makeNumber(DataBaseTable::CAR_TABLE);
    QString num1 = Util::makeNumber(DataBaseTable::RENTALDOCUMENT_TABLE);
    ALOGD("num = %s", num.toStdString().data());
    ALOGD("num1 = %s", num1.toStdString().data());
}

void
CarEditDialog::updateAnnualSumSectionWidth(int logicalIndex,
                                           int /* oldSize */,
                                           int newSize)
{
    ui->annualSumTableView->setColumnWidth(logicalIndex, newSize);
}

void
CarEditDialog::updateProjectSumSectionWidth(int logicalIndex,
                                            int /* oldSize */,
                                            int newSize)
{
    ui->projectSumTableView->setColumnWidth(logicalIndex, newSize);
}

void
CarEditDialog::updateBusinessSumSectionWidth(int logicalIndex,
                                             int /* oldSize */,
                                             int newSize)
{
    ui->businessSumTableView->setColumnWidth(logicalIndex, newSize);
}

void
CarEditDialog::updatePaymentSumSectionWidth(int logicalIndex,
                                            int /* oldSize */,
                                            int newSize)
{
    ui->paymentSumTableView->setColumnWidth(logicalIndex, newSize);
}

CarEditDialog::~CarEditDialog()
{
    delete ui;
}

void
CarEditDialog::initViewWithUser(User &user)
{
    if (!user.isRoot()) {
        mActSaveExit->setEnabled(false);
        mActSave->setEnabled(false);
        mActEdit->setEnabled(false);
        mActCancel->setEnabled(false);
        mCurUserIsRoot = false;
    } else {
        mCurUserIsRoot = true;
    }
}

void
CarEditDialog::openCarEditDialogSlot(OpenType type, Car&car)
{
    Car tmp;
    mOpenType = type;
    if (type == OpenType::CREATEITEM) {
        mActEdit->setDisabled(true);
        ui->numLE->setText(Util::makeNumber(DataBaseTable::CAR_TABLE));
        ui->pumpTypeCbBox->setCurrentIndex(0);
        ui->createDateDE->setDate(QDate::currentDate());
        ui->drivingLicenseDateDE->setDate(QDate::currentDate());
        ui->productDateDE->setDate(QDate::currentDate());
        setEditMode();
        mActSave->setDisabled(true);
        mActCancel->setDisabled(true);
        ui->mainTabWidget->removeTab(2);
        ui->mainTabWidget->removeTab(1);
        saveUiContent(tmp);
        setOriginCar(tmp);
    } else {
        //已查看内容方式打开
        mCarNumber = car.number;
        if (mCurUserIsRoot)
            mActSave->setEnabled(true);
        ui->numLE->setDisabled(true);
        setViewMode();
        setOriginCar(car);
        setView(car);
        ui->mainTabWidget->addTab(ui->insuranceTab, "保险资料");
        ui->mainTabWidget->addTab(ui->projectRecordTab, "工程记录");
        updateAllTableView();
    }
    updateProjectTableView();
    ui->mainTabWidget->setCurrentIndex(0);
    ui->insuranceTabWidget->setCurrentIndex(0);
    this->exec();
}

void
CarEditDialog::updateAnnualTableviewScrollBar(int to)
{

}

void
CarEditDialog::configToolBar()
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
CarEditDialog::initView()
{
    ui->toolBarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle("车辆档案");
    this->setFixedSize(1000, 700);
    setPumpTypeView();
    initProjectTableview();
    initProjectSumTableview();
    initAnnualTableview();
    initAnnualSumTableview();
    initBusinessTableview();
    initBusinessSumTableview();
    initPaymentTableview();
    initPaymentSumTableview();
}

void
CarEditDialog::setPumpTypeView()
{
    ui->pumpTypeCbBox->insertItem(PUMP_TYPE_37M1_COLUMN,
                                  PUMP_TYPE_37M1_STR);
    ui->pumpTypeCbBox->insertItem(PUMP_TYPE_48M_COLUMN,
                                  PUMP_TYPE_48M_STR);
    ui->pumpTypeCbBox->insertItem(PUMP_TYPE_52M_COLUMN,
                                  PUMP_TYPE_52M_STR);
    ui->pumpTypeCbBox->insertItem(PUMP_TYPE_56M_COLUMN,
                                  PUMP_TYPE_56M_STR);
    ui->pumpTypeCbBox->insertItem(PUMP_TYPE_60M_COLUMN,
                                  PUMP_TYPE_60M_STR);
    ui->pumpTypeCbBox->insertItem(PUMP_TYPE_CAR_PUMP_COLUMN,
                                  PUMP_TYPE_CAR_PUMP_STR);
}

void
CarEditDialog::initProjectTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "日期" << "合同号" << "客户编号"
               << "客户名称" << "工程款额" << "备注" << "签单号";

    mProjectModel = new TableModel(0, headerList.size());
    ui->projectTableView->setModel(mProjectModel);
    mProjectModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->projectTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->projectTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->projectTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    //最后一列自适应剩余空间
    ui->projectTableView->horizontalHeader()
            ->setSectionResizeMode(7, QHeaderView::Stretch);

    ui->projectTableView->verticalHeader()->setVisible(false);          //隐藏行表头
    ui->projectTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->projectTableView->setAlternatingRowColors(true);
    ui->projectTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    //隐藏滚动条
    ui->projectTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏滚动条
    ui->projectTableView->setSortingEnabled(true);

    ui->projectTableView->setColumnWidth(PRO_REC_COL_CONTRACT_NUM, 200);
}

void
CarEditDialog::initProjectSumTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "日期" << "合同号" << "客户编号"
               << "客户名称" << "工程款额" << "备注" << "签单号";

    mProjectSumModel = new TableModel(0, headerList.size());
    ui->projectSumTableView->setModel(mProjectSumModel);
    mProjectSumModel->setHorizontalHeaderLabels(headerList);
    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->projectSumTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->projectSumTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->projectSumTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->projectSumTableView->verticalHeader()->setVisible(false);       //隐藏行表头
    ui->projectSumTableView->horizontalHeader()->setVisible(false);     //隐藏列表头
    ui->projectSumTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->projectSumTableView->setAlternatingRowColors(true);
    ui->projectSumTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->projectSumTableView->setColumnWidth(PRO_REC_COL_CONTRACT_NUM, 200);

    QStandardItem* sumStr = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sumStr;
    mProjectSumModel->appendRow(items);

    clearProSumTabData();
}

void
CarEditDialog::initAnnualTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "日期" << "年费" << "车船费" << "备注";

    mAnnualModel = new TableModel(0, headerList.size());
    ui->annualTableview->setModel(mAnnualModel);
    mAnnualModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->annualTableview->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->annualTableview->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->annualTableview->setSelectionMode(
                QAbstractItemView::SingleSelection);

    //列表头不可拖动，最后一列自适应剩余空间
#ifdef BAN_SECTION
    ui->annualTableview->horizontalHeader()
            ->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->annualTableview->horizontalHeader()
            ->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->annualTableview->horizontalHeader()
            ->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->annualTableview->horizontalHeader()
            ->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->annualTableview->horizontalHeader()
            ->setSectionResizeMode(4, QHeaderView::Fixed);
#endif
    ui->annualTableview->horizontalHeader()
            ->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->annualTableview->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->annualTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->annualTableview->setAlternatingRowColors(true);
    ui->annualTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    //隐藏滚动条
    ui->annualTableview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->annualTableview->setSortingEnabled(true);
}

void
CarEditDialog::initAnnualSumTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "日期" << "年费" << "车船费" << "备注";
    mAnnualSumModel = new TableModel(0, headerList.size());
    ui->annualSumTableView->setModel(mAnnualSumModel);
    mAnnualSumModel->setHorizontalHeaderLabels(headerList);
    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->annualSumTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->annualSumTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->annualSumTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->annualSumTableView->verticalHeader()->setVisible(false);        //隐藏行表头
    ui->annualSumTableView->horizontalHeader()->setVisible(false);      //隐藏列表头
    ui->annualSumTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->annualSumTableView->setAlternatingRowColors(true);
    ui->annualSumTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    QStandardItem* sum = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sum;
    mAnnualSumModel->appendRow(items);

    clearAnnualSumTabData();
}

void
CarEditDialog::initBusinessTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "日期" << "保险费用" << "保险公司" << "备注";

    mBusinessModel = new TableModel(0, headerList.size());
    ui->businessTableView->setModel(mBusinessModel);
    mBusinessModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->businessTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->businessTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->businessTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    //列表头不可拖动，最后一列自适应剩余空间
#ifdef BAN_SECTION
    ui->businessTableView->horizontalHeader()
            ->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->businessTableView->horizontalHeader()
            ->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->businessTableView->horizontalHeader()
            ->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->businessTableView->horizontalHeader()
            ->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->businessTableView->horizontalHeader()
            ->setSectionResizeMode(4, QHeaderView::Fixed);
#endif
    ui->businessTableView->horizontalHeader()
            ->setSectionResizeMode(4, QHeaderView::Stretch);

    ui->businessTableView->verticalHeader()->setVisible(false);         //隐藏行表头
    ui->businessTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->businessTableView->setAlternatingRowColors(true);
    ui->businessTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    //隐藏滚动条
    ui->businessTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->businessTableView->setSortingEnabled(true);
}

void
CarEditDialog::initBusinessSumTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "日期" << "保险费用" << "保险公司" << "备注";

    mBusinessSumModel = new TableModel(0, headerList.size());
    ui->businessSumTableView->setModel(mBusinessSumModel);
    mBusinessSumModel->setHorizontalHeaderLabels(headerList);
    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->businessSumTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->businessSumTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->businessSumTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->businessSumTableView->verticalHeader()->setVisible(false);      //隐藏行表头
    ui->businessSumTableView->horizontalHeader()->setVisible(false);    //隐藏列表头
    ui->businessSumTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->businessSumTableView->setAlternatingRowColors(true);
    ui->businessSumTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    QStandardItem* sum = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sum;
    mBusinessSumModel->appendRow(items);

    clearBusSumTabData();
}

void
CarEditDialog::initPaymentTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "日期" << "保险费用" << "保险公司" << "备注";

    mPaymentModel = new TableModel(0, headerList.size());
    ui->paymentTableView->setModel(mPaymentModel);
    mPaymentModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->paymentTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->paymentTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->paymentTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    //列表头不可拖动，最后一列自适应剩余空间
#ifdef BAN_SECTION
    ui->paymentTableView->horizontalHeader()
            ->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->paymentTableView->horizontalHeader()
            ->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->paymentTableView->horizontalHeader()
            ->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->paymentTableView->horizontalHeader()
            ->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->paymentTableView->horizontalHeader()
            ->setSectionResizeMode(4, QHeaderView::Fixed);
#endif
    ui->paymentTableView->horizontalHeader()
            ->setSectionResizeMode(4, QHeaderView::Stretch);

    ui->paymentTableView->verticalHeader()->setVisible(false);          //隐藏行表头
    ui->paymentTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->paymentTableView->setAlternatingRowColors(true);
    ui->paymentTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
    //隐藏滚动条
    ui->paymentTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->paymentTableView->setSortingEnabled(true);
}

void
CarEditDialog::initPaymentSumTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "日期" << "保险费用" << "保险公司" << "备注";

    mPaymentSumModel = new TableModel(0, headerList.size());
    ui->paymentSumTableView->setModel(mPaymentSumModel);
    mPaymentSumModel->setHorizontalHeaderLabels(headerList);
    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->paymentSumTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->paymentSumTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->paymentSumTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    //    ui->paymentSumTableView->setHorizontalScrollBarPolicy(
    //                Qt::ScrollBarAlwaysOff);                                //隐藏滚动条
    ui->paymentSumTableView->verticalHeader()->setVisible(false);        //隐藏行表头
    ui->paymentSumTableView->horizontalHeader()->setVisible(false);      //隐藏列表头
    ui->paymentSumTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->paymentSumTableView->setAlternatingRowColors(true);
    ui->paymentSumTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    QStandardItem* sum
            = new QStandardItem("合计");
    QList<QStandardItem*> items;
    items << sum;
    mPaymentSumModel->appendRow(items);

    clearPaySumTabData();
}

void
CarEditDialog::clearAnnualSumTabData()
{
    setAnnSumAnnFeeCellVal(0);
    setAnnSumTraFeeCellVal(0);
    annSumUpdateRowCount();
}

void
CarEditDialog::clearBusSumTabData()
{
    setBusSumFeeCellVal(0);
    busSumUpdateRowCount();
}

void
CarEditDialog::clearPaySumTabData()
{
    setPaySumFeeCellVal(0);
    paySumUpdateRowCount();
}

void
CarEditDialog::clearProSumTabData()
{
    setProSumAmountCellVal(0);
    proSumUpdateRowCount();
}

void
CarEditDialog::annSumUpdateRowCount()
{
    int rows = 0;
    if (mAnnualModel->rowCount() > 0)
        rows = mAnnualModel->rowCount();

    mAnnualSumModel->setData(mAnnualSumModel->index(0, 1), QString::number(rows));
}

void
CarEditDialog::paySumUpdateRowCount()
{
    int rows = 0;
    if (mPaymentModel->rowCount() > 0)
        rows = mPaymentModel->rowCount();

    mPaymentSumModel->setData(mPaymentSumModel->index(0, 1), QString::number(rows));
}

void
CarEditDialog::busSumUpdateRowCount()
{
    int rows = 0;
    if (mBusinessModel->rowCount() > 0)
        rows = mBusinessModel->rowCount();

    mBusinessSumModel->setData(mBusinessSumModel->index(0, 1), QString::number(rows));
}

void
CarEditDialog::proSumUpdateRowCount()
{
    int rows = 0;
    if (mProjectModel->rowCount() > 0)
        rows = mProjectModel->rowCount();

    mProjectSumModel->setData(mProjectSumModel->index(0, 1), QString::number(rows));
}

void
CarEditDialog::setAnnSumAnnFeeCellVal(double value)
{
    mAnnualSumModel->setData(mAnnualSumModel->index(0, ANN_REC_COL_ANNFEE),
                             Util::doubleToDecimal2String(value));
    mCurAnnSumAnnFee = value;
}

void
CarEditDialog::setAnnSumTraFeeCellVal(double value)
{
    mAnnualSumModel->setData(mAnnualSumModel->index(0, ANN_REC_COL_TRAVELFEE),
                             Util::doubleToDecimal2String(value));
    mCurAnnSumTraFee = value;
}

void
CarEditDialog::setPaySumFeeCellVal(double value)
{
    mPaymentSumModel->setData(mPaymentSumModel->index(0, INSURANCE_REC_COL_FEE),
                              Util::doubleToDecimal2String(value));
    mCurPaySumFee = value;
}

void
CarEditDialog::setBusSumFeeCellVal(double value)
{
    mBusinessSumModel->setData(mBusinessSumModel->index(0, INSURANCE_REC_COL_FEE),
                               Util::doubleToDecimal2String(value));
    mCurBusSumFee = value;
}

void
CarEditDialog::setProSumAmountCellVal(double value)
{
    mProjectSumModel->setData(mProjectSumModel->index(0, PRO_REC_COL_AMOUNT),
                              Util::doubleToDecimal2String(value));
    mCurProSumAmount = value;
}

void
CarEditDialog::annSumAnnFeeCellAddVal(double value)
{
    setAnnSumAnnFeeCellVal(mCurAnnSumAnnFee+value);
}

void
CarEditDialog::annSumTraFeeCellAddVal(double value)
{
    setAnnSumTraFeeCellVal(mCurAnnSumTraFee+value);
}

void
CarEditDialog::paySumFeeCellAddVal(double value)
{
    setPaySumFeeCellVal(mCurPaySumFee+value);
}

void
CarEditDialog::busSumFeeCellAddVal(double value)
{
    setBusSumFeeCellVal(mCurBusSumFee+value);
}

void
CarEditDialog::proSumAmountCellAddVal(double value)
{
    setProSumAmountCellVal(mCurProSumAmount+value);
}

void
CarEditDialog::annSumAnnFeeCellDelVal(double value)
{
    setAnnSumAnnFeeCellVal(mCurAnnSumAnnFee-value);
}

void
CarEditDialog::annSumTraFeeCellDelVal(double value)
{
    setAnnSumTraFeeCellVal(mCurAnnSumTraFee-value);
}

void
CarEditDialog::paySumFeeCellDelVal(double value)
{
    setPaySumFeeCellVal(mCurPaySumFee-value);
}

void
CarEditDialog::busSumFeeCellDelVal(double value)
{
    setBusSumFeeCellVal(mCurBusSumFee-value);
}

void
CarEditDialog::proSumAmountCellDelVal(double value)
{
    setProSumAmountCellVal(mCurProSumAmount-value);
}

void
CarEditDialog::setEditMode()
{
    if (mOpenType == CREATEITEM)
        ui->numLE->setFocus();
    else
        ui->carNumberLE->setFocus();

    if (mCurUserIsRoot) {
        mActEdit->setDisabled(true);
        mActCancel->setEnabled(true);
        mActSave->setEnabled(true);
    }
    setMode(true);
}


void
CarEditDialog::setViewMode()
{
    if (mCurUserIsRoot) {
        mActCancel->setDisabled(true);
        mActEdit->setEnabled(true);
        mActSave->setDisabled(true);
    }
    setMode(false);
}

void
CarEditDialog::setMode(bool mode)
{
    qDebug() << "mIsModifing = " << mIsModifing;
    mIsModifing = mode;
    ui->numLE->setEnabled(mode);
    ui->ownerLE->setEnabled(mode);
    ui->bankAccountLE->setEnabled(mode);
    ui->pumpTypeCbBox->setEnabled(mode);
    ui->carNumberLE->setEnabled(mode);
    ui->carBrandLE->setEnabled(mode);
    ui->chassisBrandLE->setEnabled(mode);
    ui->drivingLicenseDateDE->setEnabled(mode);
    ui->fuelCarNumberLE->setEnabled(mode);
    ui->frameNumberLE->setEnabled(mode);
    ui->identificationNumberLE->setEnabled(mode);
    ui->engineNumberLE->setEnabled(mode);
    ui->worthDSB->setEnabled(mode);
    ui->insuranceCardNumberLE->setEnabled(mode);
    ui->factoryCodeLE->setEnabled(mode);
    ui->productNumberLE->setEnabled(mode);
    ui->enginePowerDSB->setEnabled(mode);
    ui->maxDeliverySizesDSB->setEnabled(mode);
    ui->maxOutputPressureDSB->setEnabled(mode);
    ui->dimensionsLE->setEnabled(mode);
    ui->boomVerticalLenDSB->setEnabled(mode);
    ui->boomHorizontalLenDSB->setEnabled(mode);
    ui->totalWeightDSB->setEnabled(mode);
    ui->equipmentWeightDSB->setEnabled(mode);
    ui->productDateDE->setEnabled(mode);
    ui->operator1LE->setEnabled(mode);
    ui->operator2LE->setEnabled(mode);
    ui->operator3LE->setEnabled(mode);
    ui->operator4LE->setEnabled(mode);
    ui->operator5LE->setEnabled(mode);
    ui->remarksTextEdit->setEnabled(mode);
    ui->pumpedSquareDSB->setEnabled(mode);
    ui->pumpedTimesDSB->setEnabled(mode);
    ui->milageDSB->setEnabled(mode);
    ui->creatorLE->setEnabled(mode);
    ui->createDateDE->setEnabled(mode);
}

void
CarEditDialog::setOriginCar(Car &car)
{
    mOriginCar->copy(car, mOriginCar);
}

void
CarEditDialog::setView(Car &car)
{
    ui->numLE->setText(car.number);
    ui->ownerLE->setText(car.owner);
    ui->bankAccountLE->setText(car.bankAccount);
    ui->carNumberLE->setText(car.carNumber);
    ui->carBrandLE->setText(car.carBrand);
    ui->chassisBrandLE->setText(car.chassisBrand);
    ui->fuelCarNumberLE->setText(car.fuelCarNumber);
    ui->frameNumberLE->setText(car.frameNumber);
    ui->identificationNumberLE->setText(car.identificationNumber);
    ui->engineNumberLE->setText(car.engineNumber);
    ui->insuranceCardNumberLE->setText(car.insuranceCardNumber);
    ui->factoryCodeLE->setText(car.factoryCode);
    ui->productNumberLE->setText(car.productNumber);
    ui->dimensionsLE->setText(car.dimensions);
    ui->operator1LE->setText(car.operator1);
    ui->operator2LE->setText(car.operator2);
    ui->operator3LE->setText(car.operator3);
    ui->operator4LE->setText(car.operator4);
    ui->operator5LE->setText(car.operator5);
    ui->remarksTextEdit->setText(car.remarks);
    ui->creatorLE->setText(car.creator);

    ui->pumpedSquareDSB->setValue(car.pumpedSquare);
    ui->pumpedTimesDSB->setValue(car.pumpedTimes);
    ui->milageDSB->setValue(car.milage);
    ui->worthDSB->setValue(car.worth);
    ui->enginePowerDSB->setValue(car.enginePower);
    ui->maxDeliverySizesDSB->setValue(car.maxDeliverySizes);
    ui->maxOutputPressureDSB->setValue(car.maxOutputPressure);
    ui->boomVerticalLenDSB->setValue(car.boomVerticalLen);
    ui->boomHorizontalLenDSB->setValue(car.boomHorizontalLen);
    ui->totalWeightDSB->setValue(car.totalWeight);
    ui->equipmentWeightDSB->setValue(car.equipmentWeight);

    ui->drivingLicenseDateDE->setDate(car.drivingLicenseDate);
    ui->productDateDE->setDate(car.productionDate);
    ui->createDateDE->setDate(car.createDate);
    ui->pumpTypeCbBox->setCurrentIndex(getPumpTypePosition(car.pumptype));
}

int
CarEditDialog::getPumpTypePosition(PumpType type)
{

    switch (type) {
    case TYPE_37M1:
       return PUMP_TYPE_37M1_COLUMN;

    case TYPE_48M:
       return PUMP_TYPE_48M_COLUMN;

    case TYPE_52M:
       return PUMP_TYPE_52M_COLUMN;

    case TYPE_56M:
       return PUMP_TYPE_56M_COLUMN;

    case TYPE_60M:
       return PUMP_TYPE_60M_COLUMN;

    case TYPE_CAR_PUMP:
       return PUMP_TYPE_CAR_PUMP_COLUMN;

    default:
        break;
    }

    return PUMP_TYPE_37M1_COLUMN;
}

void
CarEditDialog::saveAndExitEvent()
{
    int ret;

    if (ui->numLE->text().isEmpty() ||
            ui->carNumberLE->text().isEmpty()) {
        QMessageBox::warning(this, tr("温馨提示"),
                             tr("车号与车牌号不能为空！\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    if (!isModified()) {
        closeDialog();
        return;
    }

    Car car;
    saveUiContent(car);
    dumpCar(car);

    if (mOpenType == CREATEITEM) {
        // 插入数据库,更新到界面
        if (mDb->isCarExist(car)) {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("该车辆已存在，添加失败!\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
            return;
        }

        if(!mDb->insertCarTable(car)) {
            resetView(car);
            emit addCarItemSignal(car);
            QMessageBox::information(this,
                                     tr("温馨提示"),
                                     tr("添加成功.\n"),
                                     QMessageBox::Ok,
                                     QMessageBox::Ok);
        } else {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("添加失败!未知错误.\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
            return;
        }
    } else {
        // 更新到数据库
        if (!mDb->updateCarTableData(car)) {
            resetView(car);
            updateCarItemSignal(car);
            ret = QMessageBox::information(this,
                                           tr("温馨提示"),
                                           tr("已保存.\n"),
                                           QMessageBox::Ok,
                                           QMessageBox::Ok);
        } else {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("保存失败!未知错误.\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
            return;
        }
    }

    this->close();
}

bool
CarEditDialog::isModified()
{
    Car tmp;
    saveUiContent(tmp);
    if (mOriginCar->isValueEqualWithoutRecords(tmp))
        return false;

    return true;
}

void
CarEditDialog::closeEvent(QCloseEvent *event)
{
    if (mOpenType == CREATEITEM)
        ALOGD("CREATEITEM");
    else if (isModified()) {
        // 有内容发生修改
        ALOGD("isModified");
        int ret = QMessageBox::warning(this, tr("温馨提示"),
                                       tr("是否保存修改？\n"),
                                       QMessageBox::Yes |
                                       QMessageBox::No,
                                       QMessageBox::Yes);
        if (ret == QMessageBox::Yes)
            saveEvent();
    }
    clean();
    mCarNumber = "";
}

void
CarEditDialog::clean()
{
    cleanContent();
}

void
CarEditDialog::cleanContent()
{
    ui->numLE->setText("");
    ui->ownerLE->setText("");
    ui->bankAccountLE->setText("");
    ui->carNumberLE->setText("");
    ui->carBrandLE->setText("");
    ui->chassisBrandLE->setText("");
    ui->fuelCarNumberLE->setText("");
    ui->frameNumberLE->setText("");
    ui->identificationNumberLE->setText("");
    ui->engineNumberLE->setText("");
    ui->insuranceCardNumberLE->setText("");
    ui->factoryCodeLE->setText("");
    ui->productNumberLE->setText("");
    ui->dimensionsLE->setText("");
    ui->operator1LE->setText("");
    ui->operator2LE->setText("");
    ui->operator3LE->setText("");
    ui->operator4LE->setText("");
    ui->operator5LE->setText("");
    ui->remarksTextEdit->setText("");
    ui->creatorLE->setText("");

    ui->pumpedSquareDSB->setValue(0);
    ui->pumpedTimesDSB->setValue(0);
    ui->milageDSB->setValue(0);
    ui->worthDSB->setValue(0);
    ui->enginePowerDSB->setValue(0);
    ui->maxDeliverySizesDSB->setValue(0);
    ui->maxOutputPressureDSB->setValue(0);
    ui->boomVerticalLenDSB->setValue(0);
    ui->boomHorizontalLenDSB->setValue(0);
    ui->totalWeightDSB->setValue(0);
    ui->equipmentWeightDSB->setValue(0);
}

void
CarEditDialog::closeDialog()
{
    ALOGD("closeDialog");
    this->close();
}

void
CarEditDialog::saveUiContent(Car &car)
{
    bool ok;

    car.number = ui->numLE->text();
    car.owner = ui->ownerLE->text();
    car.bankAccount = ui->bankAccountLE->text();
    car.carNumber = ui->carNumberLE->text();
    car.carBrand = ui->carBrandLE->text();
    car.chassisBrand = ui->chassisBrandLE->text();
    car.fuelCarNumber = ui->fuelCarNumberLE->text();
    car.frameNumber = ui->frameNumberLE->text();
    car.identificationNumber = ui->identificationNumberLE->text();
    car.engineNumber = ui->engineNumberLE->text();
    car.insuranceCardNumber = ui->insuranceCardNumberLE->text();
    car.factoryCode = ui->factoryCodeLE->text();
    car.productNumber = ui->productNumberLE->text();
    car.dimensions = ui->dimensionsLE->text();
    car.operator1 = ui->operator1LE->text();
    car.operator2 = ui->operator2LE->text();
    car.operator3 = ui->operator3LE->text();
    car.operator4 = ui->operator4LE->text();
    car.operator5 = ui->operator5LE->text();
    car.remarks = ui->remarksTextEdit->toPlainText();
    car.creator = ui->creatorLE->text();

    car.drivingLicenseDate = QDate::fromString(
                ui->drivingLicenseDateDE->text(), DATE_FORMAT_STR);
    car.productionDate = QDate::fromString(
                ui->productDateDE->text(), DATE_FORMAT_STR);
    car.createDate = QDate::fromString(
                ui->createDateDE->text(), DATE_FORMAT_STR);

    car.pumpedSquare = ui->pumpedSquareDSB->value();
    car.pumpedTimes = ui->pumpedTimesDSB->value();
    car.milage = ui->milageDSB->value();
    car.worth = ui->worthDSB->value();
    car.enginePower = ui->enginePowerDSB->value();
    car.maxDeliverySizes = ui->maxDeliverySizesDSB->value();
    car.maxOutputPressure = ui->maxOutputPressureDSB->value();
    car.boomVerticalLen = ui->boomVerticalLenDSB->value();
    car.boomHorizontalLen = ui->boomHorizontalLenDSB->value();
    car.totalWeight = ui->totalWeightDSB->value();
    car.equipmentWeight = ui->equipmentWeightDSB->value();

    car.pumptype = car.getPumpType(ui->pumpTypeCbBox->currentText());
}

void
CarEditDialog::dumpCar(Car &car)
{
    ALOGD("car.number = %s\n"
          "car.owner = %s\n"
          "car.bankAccount = %s\n"
          "car.carNumber = %s\n"
          "car.carBrand = %s\n"
          "car.chassisBrand = %s\n"
          "car.fuelCarNumber = %s\n"
          "car.frameNumber = %s\n"
          "car.identificationNumber = %s\n"
          "car.engineNumber = %s\n"
          "car.insuranceCardNumber = %s\n"
          "car.factoryCode = %s\n"
          "car.productNumber = %s\n"
          "car.dimensions = %s\n"
          "car.operator1 = %s\n"
          "car.operator2 = %s\n"
          "car.operator3 = %s\n"
          "car.operator4 = %s\n"
          "car.operator5 = %s\n"
          "car.remarks = %s\n"
          "car.creator = %s\n"
          "car.pumpedSquare = %lf\n"
          "car.pumpedTimes = %lf\n"
          "car.milage = %lf\n"
          "car.worth = %lf\n"
          "car.enginePower = %lf\n"
          "car.maxDeliverySizes = %lf\n"
          "car.maxOutputPressure = %lf\n"
          "car.boomVerticalLen = %lf\n"
          "car.boomHorizontalLen = %lf\n"
          "car.totalWeight = %lf\n"
          "car.equipmentWeight = %lf\n"
          "car.pumptype = %d\n",
          "car.pumptype = %d\n",
          "car.pumptype = %d\n",
          "car.pumptype = %d\n",
          "car.drivingLicenseDate",
          "car.createDate",
          "car.productionDate",
          car.number.toStdString().data(),
          car.owner.toStdString().data(),
          car.bankAccount.toStdString().data(),
          car.carNumber.toStdString().data(),
          car.carBrand.toStdString().data(),
          car.chassisBrand.toStdString().data(),
          car.fuelCarNumber.toStdString().data(),
          car.frameNumber.toStdString().data(),
          car.identificationNumber.toStdString().data(),
          car.engineNumber.toStdString().data(),
          car.insuranceCardNumber.toStdString().data(),
          car.factoryCode.toStdString().data(),
          car.productNumber.toStdString().data(),
          car.dimensions.toStdString().data(),
          car.operator1.toStdString().data(),
          car.operator2.toStdString().data(),
          car.operator3.toStdString().data(),
          car.operator4.toStdString().data(),
          car.operator5.toStdString().data(),
          car.remarks.toStdString().data(),
          car.creator.toStdString().data(),
          car.pumpedSquare,
          car.pumpedTimes,
          car.milage,
          car.worth,
          car.enginePower,
          car.maxDeliverySizes,
          car.maxOutputPressure,
          car.boomVerticalLen,
          car.boomHorizontalLen,
          car.totalWeight,
          car.equipmentWeight,
          car.pumptype,
          car.drivingLicenseDate.toString(DATE_FORMAT_STR).toStdString().data(),
          car.createDate.toString(DATE_FORMAT_STR).toStdString().data(),
          car.productionDate.toString(DATE_FORMAT_STR).toStdString().data());
}

void
CarEditDialog::saveEvent()
{
    Car car;

    saveUiContent(car);
    //更新数据到数据库、更新界面数据
    if (!mDb->updateCarTableData(car)) {
        resetView(car);
        emit updateCarItemSignal(car);
    } else {
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("保存失败!未知错误.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    }
}

void
CarEditDialog::editEvent()
{
    setEditMode();
    ui->numLE->setDisabled(true);
}

void
CarEditDialog::cancelEvent()
{
    resetView();
    setViewMode();
}

void
CarEditDialog::resetView(Car &car)
{
    setOriginCar(car);
    resetView();
}

void
CarEditDialog::resetView()
{
    if (!isModified())
        return;

    setView(*mOriginCar);
}

void
CarEditDialog::addAnnualItemSlot(ANNUALFEE_RECORD &record)
{
    QStandardItem* num = new QStandardItem(record.number);
    QStandardItem* date = new QStandardItem(record.date.toString(DATE_FORMAT_STR));
    QStandardItem* remarks = new QStandardItem(record.remarks);
    QStandardItem* annualFee = new QStandardItem(Util::doubleToDecimal2String(record.annualFee));
    QStandardItem* travelExpenses = new QStandardItem(Util::doubleToDecimal2String(record.travelExpenses));

    QList<QStandardItem*> items;
    items << num << date << annualFee << travelExpenses << remarks;
    mAnnualModel->appendRow(items);

    // 更新合计表
    annSumAnnFeeCellAddVal(record.annualFee);
    annSumTraFeeCellAddVal(record.travelExpenses);
    annSumUpdateRowCount();
}

void
CarEditDialog::addInsuranceItemSlot(INSURANCE_RECORD &record)
{
    QStandardItem* num = new QStandardItem(record.number);
    QStandardItem* date = new QStandardItem(record.date.toString(DATE_FORMAT_STR));
    QStandardItem* remarks = new QStandardItem(record.remarks);
    QStandardItem* company = new QStandardItem(record.company);
    QStandardItem* fee = new QStandardItem(Util::doubleToDecimal2String(record.fee));

    QList<QStandardItem*> items;
    items << num << date << fee << company << remarks;
    mPaymentModel->appendRow(items);

    // 更新合计表
    paySumFeeCellAddVal(record.fee);
    paySumUpdateRowCount();
}

void
CarEditDialog::addBusinessInsuranceItemSlot(INSURANCE_RECORD &record)
{
    QStandardItem* num = new QStandardItem(record.number);
    QStandardItem* date = new QStandardItem(record.date.toString(DATE_FORMAT_STR));
    QStandardItem* company = new QStandardItem(record.company);
    QStandardItem* remarks = new QStandardItem(record.remarks);
    QStandardItem* fee = new QStandardItem(Util::doubleToDecimal2String(record.fee));

    QList<QStandardItem*> items;
    items << num << date << fee << company << remarks;
    mBusinessModel->appendRow(items);

    // 更新合计表
    busSumFeeCellAddVal(record.fee);
    busSumUpdateRowCount();
}

void
CarEditDialog::addProjectItemSlot(PROJECT_RECORD &record)
{
    QStandardItem* num = new QStandardItem(QString::number(record.number));
    QStandardItem* date = new QStandardItem(record.date.toString(DATE_FORMAT_STR));
    QStandardItem* contractNum = new QStandardItem(record.contractNum);
    QStandardItem* clientNum = new QStandardItem(record.clientNum);
    QStandardItem* clientName = new QStandardItem(record.clientName);
    QStandardItem* amount = new QStandardItem(Util::doubleToDecimal2String(record.amount));
    QStandardItem* remarks = new QStandardItem(record.remarks);
    QStandardItem* rentalDocNum = new QStandardItem(record.rentalDocNum);

    QList<QStandardItem*> items;
    items << num << date << contractNum << clientNum << clientName
          << amount << remarks << rentalDocNum;
    mProjectModel->appendRow(items);
    // 更新合计表
    proSumAmountCellAddVal(record.amount);
    proSumUpdateRowCount();
}

void
CarEditDialog::updateAllTableView()
{
    updateAnnualTableView();
    updateInsuranceTableView();
    updateBusinessInsuanceTableView();
    updateProjectTableView();
}


void
CarEditDialog::updateAnnualTableView()
{
    int size;
    ANNUALFEE_RECORD record;
    QList<ANNUALFEE_RECORD> records;

    if (mAnnualModel->rowCount() > 0) {
        mAnnualModel->removeRows(0, mAnnualModel->rowCount());
        clearAnnualSumTabData();
    }

    if (!mDb->getAnnualDataInCarNumber(mCarNumber, records)) {
        if (records.isEmpty())
            return;

        size = records.size();
        for (int i = 0; i < size; i++) {
            record = records.at(i);
            addAnnualItemSlot(record);
        }
    }
}

void
CarEditDialog::updateInsuranceTableView()
{
    int size;
    INSURANCE_RECORD record;
    QList<INSURANCE_RECORD> records;

    if (mPaymentModel->rowCount() > 0) {
        mPaymentModel->removeRows(0, mPaymentModel->rowCount());
        clearPaySumTabData();
    }

    if (!mDb->getInsuranceDataInCarNumber(mCarNumber, records)) {
        if (records.isEmpty())
            return;

        size = records.size();
        for (int i = 0; i < size; i++) {
            record = records.at(i);
            addInsuranceItemSlot(record);
        }
    }
}

void
CarEditDialog::updateBusinessInsuanceTableView()
{
    int size;
    INSURANCE_RECORD record;
    QList<INSURANCE_RECORD> records;

    if (mBusinessModel->rowCount() > 0) {
        //删除所有行
        mBusinessModel->removeRows(0, mBusinessModel->rowCount());
        clearBusSumTabData();
    }

    if (!mDb->getBusinessInsuranceDataInCarNumber(mCarNumber, records)) {
        if (records.isEmpty())
            return;

        size = records.size();
        for (int i = 0; i < size; i++) {
            record = records.at(i);
            addBusinessInsuranceItemSlot(record);
        }
    }
}

void
CarEditDialog::updateProjectTableView()
{
    PROJECT_RECORD record;
    QList<RentalDocument> docs;
    int count = 0;

    if (mProjectModel->rowCount() > 0) {
        //删除所有行
        mProjectModel->removeRows(0, mProjectModel->rowCount());
        clearProSumTabData();
    }

    if (!mDb->getRentalDocumentDataInCarNumber(mCarNumber, docs)) {
        if (docs.isEmpty())
            return;

        for (RentalDocument doc : docs) {
            record = getProjectFromRentaldoc(doc);
            record.number = ++count;
            addProjectItemSlot(record);
        }
    }
}

PROJECT_RECORD
CarEditDialog::getProjectFromRentaldoc(const RentalDocument &doc)
{
    PROJECT_RECORD record;
    record.carNumber = doc.carNumber;
    record.rentalDocNum = doc.number;
    record.date = doc.date;
    record.contractNum = doc.contractNumber;
    record.clientNum = doc.clientNumber;
    record.clientName = doc.clientName;
    record.amount = doc.projectAmount;
    record.remarks = doc.remarks;
    return record;
}

void
CarEditDialog::delAnnualTableItem()
{
    int ret = QMessageBox::warning(this, tr("温馨提示"),
                                   tr("该操作为不可撤回操作，是否确定删除?\n"),
                                   QMessageBox::Yes |
                                   QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    QString number = mAnnualModel->index(mCurRow, 0).data().toString();
    double annFee = mAnnualModel->index(mCurRow, ANN_REC_COL_ANNFEE).data().toDouble();
    double traFee = mAnnualModel->index(mCurRow, ANN_REC_COL_TRAVELFEE).data().toDouble();
    if (!mDb->delAnnualDataInNumber(number)) {
        ALOGD("%s, delete (%s)", __FUNCTION__, number.toStdString().data());
        mAnnualModel->removeRow(mCurRow);
        // 更新合计表
        annSumAnnFeeCellDelVal(annFee);
        annSumTraFeeCellDelVal(traFee);
        annSumUpdateRowCount();
    }
}

void
CarEditDialog::delBusinessTableItem()
{
    int ret = QMessageBox::warning(this, tr("温馨提示"),
                                   tr("该操作为不可撤回操作，是否确定删除?\n"),
                                   QMessageBox::Yes |
                                   QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    QString number = mBusinessModel->index(mCurRow, 0).data().toString();
    double fee = mBusinessModel->index(mCurRow, INSURANCE_REC_COL_FEE).data().toDouble();
    if (!mDb->delBusinessInsuranceDataInNumber(number)) {
        ALOGD("%s, delete (%s)", __FUNCTION__, number.toStdString().data());
        mBusinessModel->removeRow(mCurRow);
        // 更新合计表
        busSumFeeCellDelVal(fee);
        busSumUpdateRowCount();
    }
}

void
CarEditDialog::delPaymentTableItem()
{
    int ret = QMessageBox::warning(this, tr("温馨提示"),
                                   tr("该操作为不可撤回操作，是否确定删除?\n"),
                                   QMessageBox::Yes |
                                   QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    QString number = mPaymentModel->index(mCurRow, 0).data().toString();
    double fee = mPaymentModel->index(mCurRow, INSURANCE_REC_COL_FEE).data().toDouble();
    if (!mDb->delInsuranceDataInNumber(number)) {
        ALOGD("%s, delete (%s)", __FUNCTION__, number.toStdString().data());
        mPaymentModel->removeRow(mCurRow);
        // 更新合计表
        paySumFeeCellDelVal(fee);
        paySumUpdateRowCount();
    }
}

void
CarEditDialog::on_annualTableview_clicked(const QModelIndex &index)
{
    mCurRow = index.row();
}

void
CarEditDialog::on_businessTableView_clicked(const QModelIndex &index)
{
    mCurRow = index.row();
}

void
CarEditDialog::on_paymentTableView_clicked(const QModelIndex &index)
{
    mCurRow = index.row();
}

void
CarEditDialog::on_annualTableview_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->annualTableview->indexAt(pos);
    if (!index.isValid() || !mIsModifing)
        return;

    mCurRow = index.row();
    mAnnualMenu->clear();
    mAnnualMenu->addAction(mAnnualDelAct);
    mAnnualMenu->exec(QCursor::pos());
}

void
CarEditDialog::on_businessTableView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->businessTableView->indexAt(pos);
    if (!index.isValid() || !mIsModifing)
        return;

    mCurRow = index.row();
    mBusinessMenu->clear();
    mBusinessMenu->addAction(mBusinessDelAct);
    mBusinessMenu->exec(QCursor::pos());
}

void
CarEditDialog::on_paymentTableView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->paymentTableView->indexAt(pos);
    if (!index.isValid() || !mIsModifing)
        return;

    mCurRow = index.row();
    mPaymentMenu->clear();
    mPaymentMenu->addAction(mPaymentDelAct);
    mPaymentMenu->exec(QCursor::pos());
}
