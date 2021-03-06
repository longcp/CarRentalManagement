#include "rentaldocumentwidget.h"
#include "ui_rentaldocumentwidget.h"
#include "rentaldocumenteditdialog.h"
#include <QToolBar>
#include <tablemodel.h>
#include <contract.h>
#include <rentaldocument.h>
#include <database/database.h>
#include <client.h>
#include <QMessageBox>
#include <QDebug>
#include <util.h>
#include <user.h>

#define LOG_TAG                 "RENTAL_DOC_WIDGET"
#include "utils/Log.h"

// TODO:完成删除接口
// TODO:界面显示与数据同步问题，如签证单更新、删除、添加，应收实收界面也应相应删除、添加、更新

RentalDocumentWidget::RentalDocumentWidget(QWidget *parent) :
    QWidget(parent),
    mCurClientNumber(""),
    mDb(DataBase::getInstance()),
    mCurRow(-1),
    mCurSortCol(0),
    mIsSortAscending(true),
    ui(new Ui::RentalDocumentWidget)
{
    ui->setupUi(this);
    ui->statusFrame->setStyleSheet(
                "background-color: rgb(240,240,240);color:rgb(0,0,0);");
    ui->toolbarWidget->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle(TAB_TITLE_RENTALDOCUMENT);

    mRentalDocEditDialog = new RentalDocumentEditDialog();

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
    configToolBar();
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActDelete);
    mToolBar->addAction(mActEdit);
//    mToolBar->addAction(mActSearch);
    mToolBar->addAction(mActExport);
    ui->toolBarHorizonLayout->addWidget(mToolBar);

    initView();

    /**
     * @brief 打开编辑窗口
     */
    connect(mActAdd, SIGNAL(triggered()),
            this, SLOT(addRentalDocSlot()));
    /**
     * @brief 工具栏编辑按钮
     */
    connect(mActEdit, SIGNAL(triggered()),
            this, SLOT(editRentalDocItemSlot()));
    /**
     * @brief 工具栏删除按钮
     */
    connect(mActDelete, SIGNAL(triggered()),
            this, SLOT(deleteRentalDocItemSlot()));
    /**
     * @brief 单元格双击事件
     */
    connect(ui->docTableview, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(cellDoubleClickedSlot(const QModelIndex &)));
    /**
     * @brief 打开编辑窗口
     */
    connect(this, SIGNAL(openRentalEditDialogSignal(OpenType, RentalDocument &,
                                                    QString, QString)),
            mRentalDocEditDialog, SLOT(openWindow(OpenType, RentalDocument &,
                                                  QString, QString)));
    /**
     * @brief 添加条目
     */
    connect(mRentalDocEditDialog, SIGNAL(addRentalDocSignal(RentalDocument&)),
            this, SLOT(addRentalDoc(RentalDocument&)));
    /**
     * @brief 更新条目
     */
    connect(mRentalDocEditDialog, SIGNAL(updateDocItemSignal(RentalDocument&)),
            this, SLOT(updateDocItemSlot(RentalDocument&)));

    connect(this, SIGNAL(initViewWithUserSig(User &)),
            mRentalDocEditDialog, SLOT(initViewWithUser(User &)));

    connect(ui->docTableview->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(onHeaderClicked(int)));
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
    editRowEvent(index.row());
}

void
RentalDocumentWidget::initView()
{
    initRentalDocTableView();
    initClientTreeWidget();
    ui->totalRadio->setChecked(true);
    mCurDocState = RentalDocState::UNKNOWN_STATE;
}

void
RentalDocumentWidget::initViewWithUser(User &user)
{
    if (!user.isRoot()) {
        mActAdd->setEnabled(false);
        mActEdit->setEnabled(false);
        mActDelete->setEnabled(false);
    }
    emit initViewWithUserSig(user);
}

void
RentalDocumentWidget::onHeaderClicked(int column)
{
    ALOGDTRACE();
    if (mCurSortCol == column && mIsSortAscending) {
        ui->docTableview->sortByColumn(column, Qt::DescendingOrder);
        mIsSortAscending = false;
    } else {
        ui->docTableview->sortByColumn(column, Qt::AscendingOrder);
        mIsSortAscending = true;
    }

    mCurSortCol = column;
}

void
RentalDocumentWidget::initRentalDocTableView()
{
    //设置首行标题
    QStringList headerList;
    headerList << "签单号" << "承租单位/客户" << "工程名称" << "工程地址"
               << "日期" << "车牌型号" << "泵式" << "施工部位"
               << "混凝土标号" << "开工燃油" << "收工燃油"
               << "进场时间" << "出场时间" << "工作时长" << "泵送方量(m3)"
               << "方量单价(元)" << "泵送台班数" << "台班价格(元)"
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

    ui->docTableview->verticalHeader()->setVisible(false);              //隐藏行表头
    ui->docTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->docTableview->setAlternatingRowColors(true);
    ui->docTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->docTableview->setColumnWidth(0, 200);
    ui->docTableview->setColumnWidth(1, 200);
    ui->docTableview->setColumnWidth(2, 200);
    ui->docTableview->setColumnWidth(3, 200);
    ui->docTableview->setColumnWidth(11, 200);
    ui->docTableview->setColumnWidth(12, 200);
}

void
RentalDocumentWidget::initClientTreeWidget()
{
    ui->clientTreeWidget->setColumnWidth(0, 220);
    mRootItem = new QTreeWidgetItem(ui->clientTreeWidget, QStringList("所有客户"));
    mRootItem->setIcon(0, QIcon(":/menu/icon/client.png"));
    addAllClientItem();
}

void
RentalDocumentWidget::updateClientTreeWidget(RentalDocument &doc)
{
    QTreeWidgetItem *item;
    int count = mRootItem->childCount();

    for (int i = 0; i < count; i++) {
        item = mRootItem->child(i);
        ALOGD("name = %s, num = %s", item->text(mClientNameColumn).toStdString().data(), item->text(mClientNumberColumn).toStdString().data());
        if (doc.clientNumber == item->text(mClientNumberColumn)) {
            if (mDb->isClientHasRentalDoc(doc.clientNumber))
                item->setIcon(0, QIcon(":/menu/icon/contract_64.ico"));
            else
                item->setIcon(0, QIcon(":/menu/icon/empty_64.ico"));
        }
    }
}

void
RentalDocumentWidget::addClientSlot(Client &client)
{
    ALOGDTRACE();
    addClientItem(client);
}

void
RentalDocumentWidget::addClientItem(Client &client)
{
    QStringList itemList;
    itemList << client.name << client.number;
    QTreeWidgetItem *newItem =
            new QTreeWidgetItem(mRootItem, itemList);
    if (mDb->isClientHasRentalDoc(client.number))
        newItem->setIcon(0, QIcon(":/menu/icon/contract_64.ico"));
    else
        newItem->setIcon(0, QIcon(":/menu/icon/empty_64.ico"));
    mRootItem->addChild(newItem);
}

void
RentalDocumentWidget::addAllClientItem()
{
    int size;
    QList<Client> clients;
    Client c;
    if (!mDb->getAllClientData(clients)) {
        size = clients.size();
        for (int i = 0; i < size; i++) {
            c = clients.at(i);
            addClientItem(c);
        }
    }
}

void
RentalDocumentWidget::addRentalDocSlot()
{
    RentalDocument rentalDoc;
    if (mCurClientNumber == "") {
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("请指定一位客户.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    Client c;
    if (mDb->getClientInNumber(mCurClientNumber, c)) {
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("未知错误，无法为该客户添加签证单.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    emit openRentalEditDialogSignal(OpenType::CREATEITEM, rentalDoc,
                                    c.name, mCurClientNumber);
}

void
RentalDocumentWidget::addRentalDoc(RentalDocument &doc)
{
    ALOGDTRACE();

    if (doc.state != mCurDocState &&
            mCurDocState != RentalDocState::UNKNOWN_STATE)
        return;

    addRentalDocTableRow(doc);
    updateClientTreeWidget(doc);
    emit addRentalDocumentSignal(doc);
    ui->docTableview->selectRow(mModel->rowCount() - 1);
}

void
RentalDocumentWidget::addRentalDocRows(QList<RentalDocument> &docs)
{
    RentalDocument doc;

    for (int i = 0; i < docs.size(); i++) {
        doc = docs.at(i);
        addRentalDocTableRow(doc);
    }
}

void
RentalDocumentWidget::editRowEvent(int row)
{
    ALOGDTRACE();
    RentalDocument doc;
    QString number = mModel->index(row, 0).data().toString();
    if (mDb->getRentalDocumentDataInNumber(number, doc)) {
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("未知错误,无法查看该项.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    emit openRentalEditDialogSignal(OpenType::SHOWITEM, doc,
                                    doc.clientName, doc.clientNumber);
}

void
RentalDocumentWidget::editRentalDocItemSlot()
{
    if (mCurRow < 0) {
        QMessageBox::information(this,
                                 tr("温馨提示"),
                                 tr("请选择要编辑条目.\n"),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        return;
    }

    editRowEvent(mCurRow);
}

void
RentalDocumentWidget::deleteRentalDocItemSlot()
{
    if (mCurRow < 0) {
        QMessageBox::warning(this,
                             tr("温馨提示"),
                             tr("请选择要删除条目.\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    int ret = QMessageBox::warning(this,
                                   tr("温馨提示"),
                                   tr("确定要删除该条目吗？\n"),
                                   QMessageBox::Yes |
                                   QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    RentalDocument doc;
    QString num = mModel->index(mCurRow, 0).data().toString();
    if (!mDb->deleteRentalDocumentInNumber(num)) {
        ALOGD("%s, delete ok", __FUNCTION__);
        mModel->removeRow(mCurRow);
        if (!mDb->getRentalDocumentDataInNumber(num, doc)) {
            updateClientTreeWidget(doc);
        }
        emit deleteRentalDocumentSignal(num);
    }
}

void
RentalDocumentWidget::clearRentalDocTable()
{
    if (mModel->rowCount() > 0)
        mModel->removeRows(0, mModel->rowCount());
}

void
RentalDocumentWidget::addRentalDocTableRow(RentalDocument &doc)
{
    QStandardItem *docNum = new QStandardItem(doc.number);
    QStandardItem *clientName = new QStandardItem(doc.clientName);
    QStandardItem *projectName = new QStandardItem(doc.projectName);
    QStandardItem *projectAddr = new QStandardItem(doc.projectAddress);
    QStandardItem *date = new QStandardItem(doc.date.toString(DATE_FORMAT_STR));
    QStandardItem *carPlateNumber = new QStandardItem(doc.carPlateNumber);
    QStandardItem *pumpType = new QStandardItem(QString("%1").arg(doc.pumpType));
    QStandardItem *constructPlace = new QStandardItem(doc.constructPlace);
    QStandardItem *concreteLable = new QStandardItem(doc.concreteLable);
    QStandardItem *arrivalDateTime = new QStandardItem(doc.arrivalDateTime
                                                       .toString(DATETIME_FORMAT_STR));
    QStandardItem *leaveDateTime = new QStandardItem(doc.leaveDateTime
                                                     .toString(DATETIME_FORMAT_STR));
    QStandardItem *principal = new QStandardItem(doc.principal);
    QStandardItem *principalTel = new QStandardItem(doc.principalTel);
    QStandardItem *driver1 = new QStandardItem(doc.driver1);
    QStandardItem *driver2 = new QStandardItem(doc.driver2);
    QStandardItem *driver3 = new QStandardItem(doc.driver3);
    QStandardItem *remarks = new QStandardItem(doc.remarks);
    QStandardItem *beginFuel = new QStandardItem(Util::doubleToDecimal2String(doc.beginFuel));
    QStandardItem *endFuel = new QStandardItem(Util::doubleToDecimal2String(doc.endFuel));
    QStandardItem *workingHours = new QStandardItem(Util::doubleToDecimal2String(doc.workingHours));
    QStandardItem *pumpSquare = new QStandardItem(Util::doubleToDecimal2String(doc.pumpSquare));
    QStandardItem *squareUnitPrice = new QStandardItem(Util::doubleToDecimal2String(doc.squareUnitPrice));
    QStandardItem *pumpTimes = new QStandardItem(Util::doubleToDecimal2String(doc.pumpTimes));
    QStandardItem *pumpTimeUnitPrice = new QStandardItem(Util::doubleToDecimal2String(doc.pumpTimeUnitPrice));

    QList<QStandardItem*> items;
    items << docNum << clientName << projectName << projectAddr << date
          << carPlateNumber << pumpType << constructPlace << concreteLable
          << beginFuel << endFuel << arrivalDateTime << leaveDateTime
          << workingHours << pumpSquare << squareUnitPrice << pumpTimes
          << pumpTimeUnitPrice << principal << principalTel << driver1
          << driver2 << driver3 << remarks;
    mModel->appendRow(items);
}

void
RentalDocumentWidget::updateDocItemSlot(RentalDocument &doc)
{
    ALOGDTRACE();

    mModel->setData(mModel->index(mCurRow, 0), doc.number);
    mModel->setData(mModel->index(mCurRow, 1), doc.clientName);
    mModel->setData(mModel->index(mCurRow, 2), doc.projectName);
    mModel->setData(mModel->index(mCurRow, 3), doc.projectAddress);
    mModel->setData(mModel->index(mCurRow, 4), doc.date.toString(DATE_FORMAT_STR));
    mModel->setData(mModel->index(mCurRow, 5), doc.carPlateNumber);
    mModel->setData(mModel->index(mCurRow, 6), QString("%1").arg(doc.pumpType));
    mModel->setData(mModel->index(mCurRow, 7), doc.constructPlace);
    mModel->setData(mModel->index(mCurRow, 8), doc.concreteLable);
    mModel->setData(mModel->index(mCurRow, 9), Util::doubleToDecimal2String(doc.beginFuel));
    mModel->setData(mModel->index(mCurRow, 10), Util::doubleToDecimal2String(doc.endFuel));
    mModel->setData(mModel->index(mCurRow, 11), doc.arrivalDateTime.toString(DATETIME_FORMAT_STR));
    mModel->setData(mModel->index(mCurRow, 12), doc.leaveDateTime.toString(DATETIME_FORMAT_STR));
    mModel->setData(mModel->index(mCurRow, 13), Util::doubleToDecimal2String(doc.workingHours));
    mModel->setData(mModel->index(mCurRow, 14), Util::doubleToDecimal2String(doc.pumpSquare));
    mModel->setData(mModel->index(mCurRow, 15), Util::doubleToDecimal2String(doc.squareUnitPrice));
    mModel->setData(mModel->index(mCurRow, 16), Util::doubleToDecimal2String(doc.pumpTimes));
    mModel->setData(mModel->index(mCurRow, 17), Util::doubleToDecimal2String(doc.pumpTimeUnitPrice));
    mModel->setData(mModel->index(mCurRow, 18), doc.principal);
    mModel->setData(mModel->index(mCurRow, 19), doc.principalTel);
    mModel->setData(mModel->index(mCurRow, 20), doc.driver1);
    mModel->setData(mModel->index(mCurRow, 21), doc.driver2);
    mModel->setData(mModel->index(mCurRow, 22), doc.driver3);
    mModel->setData(mModel->index(mCurRow, 23), doc.remarks);
}

void
RentalDocumentWidget::on_docTableview_clicked(const QModelIndex &index)
{
    mCurRow = index.row();
}

void RentalDocumentWidget::on_clientTreeWidget_itemClicked(QTreeWidgetItem *item,
                                                           int column)
{
    int ret;
    QList<RentalDocument> docs;

    ALOGDTRACE();
    clearRentalDocTable();
    if (item->parent() == NULL) {
        //根节点
        mCurClientNumber = "";
        mCurRow = -1;
        mCurItemIsParentItem = true;
    } else {
        //子节点
        mCurItemIsParentItem = false;
        mCurClientNumber = item->text(mClientNumberColumn);
    }

    updateTableView();
}

void
RentalDocumentWidget::updateTableView()
{
    QList<RentalDocument> docs;

    if (mCurItemIsParentItem) {
        clearRentalDocTable();
        if (mCurDocState == RentalDocState::UNKNOWN_STATE)
            mDb->getAllRentalDocumentData(docs);
        else
            mDb->getAllRenDocInState(mCurDocState, docs);
    } else {
        if (mCurClientNumber != "") {
            clearRentalDocTable();
            if (mCurDocState == RentalDocState::UNKNOWN_STATE)
                mDb->getRentalDocInClientNumber(mCurClientNumber, docs);
            else
                mDb->getRentalDocInStateAndClientNum(mCurClientNumber,
                                                     mCurDocState,
                                                     docs);
        }
    }

    addRentalDocRows(docs);
    ui->docTableview->sortByColumn(mCurSortCol, mIsSortAscending ?
                                       Qt::AscendingOrder : Qt::DescendingOrder);
}

void
RentalDocumentWidget::on_reservationRadio_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurDocState = RentalDocState::RESERVATION_STATE;
    updateTableView();
}

void
RentalDocumentWidget::on_unconfirmedRadio_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurDocState = RentalDocState::UNCONFIRMED_STATE;
    updateTableView();
}

void
RentalDocumentWidget::on_confirmedRadio_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurDocState = RentalDocState::CONFIRMED_STATE;
    updateTableView();
}

void
RentalDocumentWidget::on_totalRadio_toggled(bool checked)
{
    ALOGDTRACE();
    qDebug() << checked;
    if (!checked)
        return;

    mCurDocState = RentalDocState::UNKNOWN_STATE;
    updateTableView();
}
