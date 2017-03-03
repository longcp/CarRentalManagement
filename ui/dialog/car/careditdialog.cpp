#include "careditdialog.h"
#include "ui_careditdialog.h"
#include <QToolBar>
#include <car.h>
#include <tablemodel.h>
#include <QDebug>
#include <QMessageBox>
#include <database/database.h>

#define LOG_TAG                 "CAR_EDIT_DIALOG"
#include "utils/Log.h"

//#define                         BAN_SECTION

CarEditDialog::CarEditDialog(QWidget *parent) :
    QDialog(parent),
    mOriginCar(new Car()),
    ui(new Ui::CarEditDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags()
                         &~Qt::WindowMaximizeButtonHint);               //去掉最大化按钮
    mDb = DataBase::getInstance();
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

    mToolBar = new QToolBar(tr("carEditToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActPrev);
    mToolBar->addAction(mActNext);
    mToolBar->addAction(mActCancel);
    mToolBar->addAction(mActExit);

    ui->toolBarHorizontalLayout->addWidget(mToolBar);

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
     * @brief connect
     */
    connect(ui->annualTableview->horizontalHeader(),&QHeaderView::sectionResized, this,
            &CarEditDialog::updateAnnualSumSectionWidth);
    /**
     * @brief connect
     */
    connect(ui->paymentTableView->horizontalHeader(),&QHeaderView::sectionResized, this,
            &CarEditDialog::updatePaymentSumSectionWidth);
    /**
     * @brief connect
     */
    connect(ui->projectTableView->horizontalHeader(),&QHeaderView::sectionResized, this,
            &CarEditDialog::updateProjectSumSectionWidth);
    /**
     * @brief connect
     */
    connect(ui->businessTableView->horizontalHeader(),&QHeaderView::sectionResized, this,
            &CarEditDialog::updateBusinessSumSectionWidth);

    connect((QDialog*)ui->annualSumTableView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(updateAnnualTableviewScrollBar(int)));

    //    connect(ui->annualSumTableView->horizontalScrollBar(), SIGNAL(&QAbstractSlider::valueChanged(int)),
    //            ui->annualTableview->horizontalScrollBar(), SLOT(&QAbstractSlider::setValue(int)));
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
CarEditDialog::openCarEditDialogSlot(OpenType type, Car&car)
{
    mOpenType = type;
    if (type == OpenType::CREATEITEM) {
        mActEdit->setDisabled(true);
        mActPrev->setDisabled(true);
        mActNext->setDisabled(true);
        mActCancel->setDisabled(true);
        mActSave->setDisabled(true);
        ui->pumpTypeCbBox->setCurrentIndex(0);
        ui->createDateDE->setDate(QDate::currentDate());
        ui->drivingLicenseDateDE->setDate(QDate::currentDate());
        ui->productDateDE->setDate(QDate::currentDate());
        setEditMode();
    } else {
        //已查看内容方式打开
        mActSave->setEnabled(true);
        mActPrev->setEnabled(true);
        mActNext->setEnabled(true);
        ui->numLE->setDisabled(true);
        setViewMode();
        setOriginCar(car);
        setView(car);
    }
    ui->mainTabWidget->setCurrentIndex(0);
    ui->insuranceTabWidget->setCurrentIndex(0);
    this->exec();
}

void
CarEditDialog::updateAnnualTableviewScrollBar(int to)
{
    qDebug() << "to = " << to;
    //    ui->annualTableview->move(to, 0);
    //    ui->annualTableview->horizontalScrollbarValueChanged(to);
    //    QAbstractSlider::setValue(to);
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
    initAnnualTableview();
    initBusinessTableview();
    initPaymentTableview();
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
               << "客户名称" << "工程款额" << "备注" << "录单";

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

    //列表头不可拖动，最后一列自适应剩余空间
#ifdef BAN_SECTION
    ui->projectTableView->horizontalHeader()
            ->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->projectTableView->horizontalHeader()
            ->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->projectTableView->horizontalHeader()
            ->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->projectTableView->horizontalHeader()
            ->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->projectTableView->horizontalHeader()
            ->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->projectTableView->horizontalHeader()
            ->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->projectTableView->horizontalHeader()
            ->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->projectTableView->horizontalHeader()
            ->setSectionResizeMode(7, QHeaderView::Fixed);
#endif
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

    initProjectSumTableview();
}

void
CarEditDialog::initProjectSumTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "日期" << "合同号" << "客户编号"
               << "客户名称" << "工程款额" << "备注" << "录单";

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

    //    ui->projectSumTableView->setHorizontalScrollBarPolicy(
    //                Qt::ScrollBarAlwaysOff);                                //隐藏滚动条
    ui->projectSumTableView->verticalHeader()->setVisible(false);        //隐藏行表头
    ui->projectSumTableView->horizontalHeader()->setVisible(false);      //隐藏列表头
    ui->projectSumTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->projectSumTableView->setAlternatingRowColors(true);
    ui->projectSumTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    QStandardItem* sumStr
            = new QStandardItem("合计");
    QStandardItem* a1
            = new QStandardItem("100");
    QStandardItem* a2
            = new QStandardItem("100");
    QStandardItem* a3
            = new QStandardItem("100");
    QStandardItem* a4
            = new QStandardItem("100");
    QStandardItem* a5
            = new QStandardItem("100");
    QStandardItem* a6
            = new QStandardItem("100");
    QStandardItem* a7
            = new QStandardItem("100");
    QList<QStandardItem*> items;
    items << sumStr << a1 << a2 << a3 << a4 << a5 << a6 << a7;
    mProjectSumModel->appendRow(items);
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
    //    ui->annualTableview->setHorizontalScrollBarPolicy(
    //                Qt::ScrollBarAlwaysOff);                                 //隐藏滚动条
    ui->annualTableview->verticalHeader()->setVisible(false);           //隐藏行表头
    ui->annualTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->annualTableview->setAlternatingRowColors(true);
    ui->annualTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    initAnnualSumTableview();
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

    //    ui->annualSumTableView->setHorizontalScrollBarPolicy(
    //                Qt::ScrollBarAlwaysOff);                                //隐藏滚动条
    ui->annualSumTableView->verticalHeader()->setVisible(false);        //隐藏行表头
    ui->annualSumTableView->horizontalHeader()->setVisible(false);      //隐藏列表头
    ui->annualSumTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->annualSumTableView->setAlternatingRowColors(true);
    ui->annualSumTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    QStandardItem* sum
            = new QStandardItem("合计");
    QStandardItem* a1
            = new QStandardItem("100");
    QStandardItem* a2
            = new QStandardItem("100");
    QStandardItem* a3
            = new QStandardItem("100");
    QStandardItem* a4
            = new QStandardItem("100");
    QList<QStandardItem*> items;
    items << sum << a1 << a2 << a3 << a4;
    mAnnualSumModel->appendRow(items);
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

    initBusinessSumTableview();
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

    //    ui->businessSumTableView->setHorizontalScrollBarPolicy(
    //                Qt::ScrollBarAlwaysOff);                                //隐藏滚动条
    ui->businessSumTableView->verticalHeader()->setVisible(false);        //隐藏行表头
    ui->businessSumTableView->horizontalHeader()->setVisible(false);      //隐藏列表头
    ui->businessSumTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->businessSumTableView->setAlternatingRowColors(true);
    ui->businessSumTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    QStandardItem* sum
            = new QStandardItem("合计");
    QStandardItem* a1
            = new QStandardItem("100");
    QStandardItem* a2
            = new QStandardItem("100");
    QStandardItem* a3
            = new QStandardItem("100");
    QStandardItem* a4
            = new QStandardItem("100");
    QList<QStandardItem*> items;
    items << sum << a1 << a2 << a3 << a4;
    mBusinessSumModel->appendRow(items);
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

    initPaymentSumTableview();
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
    QStandardItem* a1
            = new QStandardItem("100");
    QStandardItem* a2
            = new QStandardItem("100");
    QStandardItem* a3
            = new QStandardItem("100");
    QStandardItem* a4
            = new QStandardItem("100");
    QList<QStandardItem*> items;
    items << sum << a1 << a2 << a3 << a4;
    mPaymentSumModel->appendRow(items);
}

void
CarEditDialog::setEditMode()
{
    mActEdit->setDisabled(true);
    mActCancel->setEnabled(true);
    mActSave->setEnabled(true);
    setMode(true);
}


void
CarEditDialog::setViewMode()
{
    mActCancel->setDisabled(true);
    mActEdit->setEnabled(true);
    mActSave->setDisabled(true);
    setMode(false);
}

void
CarEditDialog::setMode(bool mode)
{
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
    ui->worthLE->setEnabled(mode);
    ui->insuranceCardNumberLE->setEnabled(mode);
    ui->factoryCodeLE->setEnabled(mode);
    ui->productNumberLE->setEnabled(mode);
    ui->enginePowerLE->setEnabled(mode);
    ui->maxDeliverySizesLE->setEnabled(mode);
    ui->maxOutputPressureLE->setEnabled(mode);
    ui->dimensionsLE->setEnabled(mode);
    ui->boomVerticalLenLE->setEnabled(mode);
    ui->boomHorizontalLenLE->setEnabled(mode);
    ui->totalWeightLE->setEnabled(mode);
    ui->equipmentWeightLE->setEnabled(mode);
    ui->productDateDE->setEnabled(mode);
    ui->operator1LE->setEnabled(mode);
    ui->operator2LE->setEnabled(mode);
    ui->operator3LE->setEnabled(mode);
    ui->operator4LE->setEnabled(mode);
    ui->operator5LE->setEnabled(mode);
    ui->remarksTextEdit->setEnabled(mode);
    ui->pumpedSquareLE->setEnabled(mode);
    ui->pumpedTimesLE->setEnabled(mode);
    ui->milageLE->setEnabled(mode);
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

    ui->pumpedSquareLE->setText(QString::number(car.pumpedSquare));
    ui->pumpedTimesLE->setText(QString::number(car.pumpedTimes));
    ui->milageLE->setText(QString::number(car.milage));
    ui->worthLE->setText(QString::number(car.worth));
    ui->enginePowerLE->setText(QString::number(car.enginePower));
    ui->maxDeliverySizesLE->setText(QString::number(car.maxDeliverySizes));
    ui->maxOutputPressureLE->setText(QString::number(car.maxOutputPressure));
    ui->boomVerticalLenLE->setText(QString::number(car.boomVerticalLen));
    ui->boomHorizontalLenLE->setText(QString::number(car.boomHorizontalLen));
    ui->totalWeightLE->setText(QString::number(car.totalWeight));
    ui->equipmentWeightLE->setText(QString::number(car.equipmentWeight));

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
    if (ui->numLE->isModified() ||
            ui->ownerLE->isModified() ||
            ui->bankAccountLE->isModified() ||
            ui->pumpTypeCbBox->isWindowModified() ||
            ui->carNumberLE->isModified() ||
            ui->carBrandLE->isModified() ||
            ui->chassisBrandLE->isModified() ||
            ui->drivingLicenseDateDE->isWindowModified() ||
            ui->fuelCarNumberLE->isModified() ||
            ui->frameNumberLE->isModified() ||
            ui->identificationNumberLE->isModified() ||
            ui->engineNumberLE->isModified() ||
            ui->worthLE->isModified() ||
            ui->insuranceCardNumberLE->isModified() ||
            ui->factoryCodeLE->isModified() ||
            ui->productNumberLE->isModified() ||
            ui->enginePowerLE->isModified() ||
            ui->maxDeliverySizesLE->isModified() ||
            ui->maxOutputPressureLE->isModified() ||
            ui->dimensionsLE->isModified() ||
            ui->boomVerticalLenLE->isModified() ||
            ui->boomHorizontalLenLE->isModified() ||
            ui->totalWeightLE->isModified() ||
            ui->equipmentWeightLE->isModified() ||
            ui->productDateDE->isWindowModified() ||
            ui->operator1LE->isModified() ||
            ui->operator2LE->isModified() ||
            ui->operator3LE->isModified() ||
            ui->operator4LE->isModified() ||
            ui->operator5LE->isModified() ||
            ui->remarksTextEdit->isWindowModified() ||
            ui->pumpedSquareLE->isModified() ||
            ui->pumpedTimesLE->isModified() ||
            ui->milageLE->isModified() ||
            ui->creatorLE->isModified() ||
            ui->createDateDE->isWindowModified()) {
        ALOGE("is modified!!!");
        return true;
    }

    return false;
}

void
CarEditDialog::closeEvent(QCloseEvent *event)
{
    if (isModified()) {
        // 有内容发生修改
        ALOGD("isModified");
        int ret = QMessageBox::warning(this, tr("温馨提示"),
                                       tr("是否保存修改？\n"),
                                       QMessageBox::Yes |
                                       QMessageBox::No |
                                       QMessageBox::Cancel,
                                       QMessageBox::Yes);
        if (ret == QMessageBox::Yes)
            saveEvent();
        else
            return;
    }
    clean();
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

    ui->pumpedSquareLE->setText("");
    ui->pumpedTimesLE->setText("");
    ui->milageLE->setText("");
    ui->worthLE->setText("");
    ui->enginePowerLE->setText("");
    ui->maxDeliverySizesLE->setText("");
    ui->maxOutputPressureLE->setText("");
    ui->boomVerticalLenLE->setText("");
    ui->boomHorizontalLenLE->setText("");
    ui->totalWeightLE->setText("");
    ui->equipmentWeightLE->setText("");
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

}

void
CarEditDialog::dumpCar(Car &car)
{
    ALOGD("");
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
