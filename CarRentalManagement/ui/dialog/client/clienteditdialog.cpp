#include "clienteditdialog.h"
#include "ui_clienteditdialog.h"
#include <QToolBar>
#include <QMessageBox>
#include <regularexpression.h>
#include <client.h>
#include <datatype.h>
#include <QDebug>
#include <user.h>
#include "database/database.h"
#include <util.h>

#define LOG_TAG                 "CLIENT_EDIT_DIALOG"
#include "utils/Log.h"

ClientEditDialog::ClientEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientEditDialog)
{
    ui->setupUi(this);
    mDb = DataBase::getInstance();
    mOriginClient = new Client();

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

    mToolBar = new QToolBar(tr("clientEditToolBar"), this);
    configToolBar();
    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActCancel);
    mToolBar->addAction(mActExit);
    ui->toolBarVerticalLayout->addWidget(mToolBar);
    initView();
    initParam();

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
}

ClientEditDialog::~ClientEditDialog()
{
    delete ui;
}

void
ClientEditDialog::initView()
{
    this->setWindowFlags(this->windowFlags()
                         &~Qt::WindowMaximizeButtonHint);               //去掉最大化按钮
    ui->toolBarFrame->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle("客户资料");
    this->setFixedSize(800, 600);

    /* 输入长度限制 */
    ui->clientNumLineEdit->setMaxLength(16);
    ui->telLineEdit->setMaxLength(11);
    ui->faxLineEdit->setMaxLength(11);
    ui->emailLineEdit->setMaxLength(64);
    ui->addressLineEdit->setMaxLength(64);
    ui->balanceLineEdit->setMaxLength(16);
    ui->contractLineEdit->setMaxLength(16);
    ui->clientNameLineEdit->setMaxLength(16);
    ui->createPeopleLineEdit->setMaxLength(16);

    /* 输入格式限制 */
    QValidator *letterAndValidator
            = new QRegExpValidator(RegularExpression::getLetterAndNumRegExp(), this);   //英文字母+数字

    QValidator *emailValidator
            = new QRegExpValidator(RegularExpression::getEmailRegExp(), this);  //email

    QValidator *nameValidator
            = new QRegExpValidator(RegularExpression::getNameRegExp(), this);   //英文字母+数字+中文

    QValidator *telValidator
            = new QRegExpValidator(RegularExpression::getTelRegExp(), this);    //手机+固话+传真

    QValidator *moneyValidator
            = new QRegExpValidator(RegularExpression::getMoneyRegExp(), this);  //金额

    ui->clientNumLineEdit->setValidator(letterAndValidator);
    ui->emailLineEdit->setValidator(emailValidator);
    ui->clientNameLineEdit->setValidator(nameValidator);
    ui->addressLineEdit->setValidator(nameValidator);
    ui->contractLineEdit->setValidator(nameValidator);
    ui->createPeopleLineEdit->setValidator(nameValidator);
    ui->telLineEdit->setValidator(telValidator);
    ui->faxLineEdit->setValidator(telValidator);
    ui->balanceLineEdit->setValidator(moneyValidator);
}

void
ClientEditDialog::initViewWithUser(User &user)
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
ClientEditDialog::openClientEditDialogSlot(OpenType opentype,
                                           Client & client)
{
    Client tmp;
    mOpenType = opentype;
    if (opentype == OpenType::CREATEITEM) {
        //以创建条目方式打开
        mActEdit->setDisabled(true);
        mActPrev->setDisabled(true);
        mActNext->setDisabled(true);
        ui->clientNumLineEdit->setText(Util::makeNumber(DataBaseTable::CLIENT_TABLE));
        ui->monthlyRadioButton->setChecked(true);
        ui->temporaryRadioButton->setChecked(true);
        ui->createDateEdit->setDate(QDate::currentDate());
        setEditMode();                                                  //编辑模式
        mActSave->setDisabled(true);
        mActCancel->setDisabled(true);
        saveUiContent(tmp);
        setOriginClient(tmp);
    } else {
        //以查看内容方式打开
        if (mCurUserIsRoot)
            mActSave->setEnabled(true);
        ui->clientNumLineEdit->setDisabled(true);
        setViewMode();                                                  //默认为查看模式
        setOriginClient(client);
        setView(client);
    }
    this->exec();
}

void
ClientEditDialog::configToolBar()
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
ClientEditDialog::initParam()
{
    mIsInternalClose = false;
}

void
ClientEditDialog::setOriginClient(Client &client)
{
    ALOGD("%s", __FUNCTION__);
    mOriginClient->name = client.name;
    mOriginClient->number = client.number;
    mOriginClient->telephone = client.telephone;
    mOriginClient->address = client.address;
    mOriginClient->email = client.email;
    mOriginClient->fax = client.fax;
    mOriginClient->contract = client.contract;
    mOriginClient->remarks = client.remarks;
    mOriginClient->creator = client.creator;
    mOriginClient->clienttype = client.clienttype;
    mOriginClient->paytype = client.paytype;
    mOriginClient->createDate = client.createDate;
    mOriginClient->monthly = client.monthly;
    mOriginClient->amount = client.amount;
    mOriginClient->paid = client.paid;
}

void
ClientEditDialog::setView(Client &client)
{
    ui->clientNameLineEdit->setText(client.name);
    ui->clientNumLineEdit->setText(client.number);
    ui->telLineEdit->setText(client.telephone);
    ui->faxLineEdit->setText(client.fax);
    ui->addressLineEdit->setText(client.address);
    ui->contractLineEdit->setText(client.contract);
    ui->emailLineEdit->setText(client.email);
    ui->createDateEdit->setDate(client.createDate);
    ui->createPeopleLineEdit->setText(client.creator);
    ui->remarksFxtEdit->setText(client.remarks);
    ui->amountDSB->setValue(client.amount);
    ui->paidDSB->setValue(client.paid);
    ui->balanceLineEdit->setText(QString::number(client.amount
                                                 - client.paid));
    ui->monthlySpinBox->setValue(client.monthly);
    if (client.paytype == PayType::CASH) {
        ui->cashRadioButton->setChecked(true);
        ui->monthlyRadioButton->setChecked(false);
    } else if (client.paytype == PayType::MONTHLY) {
        ui->cashRadioButton->setChecked(false);
        ui->monthlyRadioButton->setChecked(true);
    }
    if (client.clienttype == ClientType::CONTRACT) {
        ui->contractRadioButton->setChecked(true);
        ui->temporaryRadioButton->setChecked(false);
    } else if (client.clienttype == ClientType::TEMPORARY) {
        ui->contractRadioButton->setChecked(false);
        ui->temporaryRadioButton->setChecked(true);
    }
}

void
ClientEditDialog::resetView(Client &client)
{
    setOriginClient(client);
    resetView();
}

void
ClientEditDialog::resetView()
{
    if (!isModified()) {
        ALOGD("%s, !isModified()", __FUNCTION__);
        return;
    }

    setView(*mOriginClient);
}

void
ClientEditDialog::closeEvent(QCloseEvent *event)
{
    ALOGD("closeEvent");
    if (mOpenType == CREATEITEM) {
        // 添加条目
        ALOGD("CREATEITEM");
    } else if (isModified()) {
        // 有内容发生修改
        ALOGD("isModified");
        int ret = QMessageBox::warning(this, tr("温馨提示"),
                                       tr("是否保存修改？\n"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes);
        if (ret == QMessageBox::Yes)
            saveEvent();
    }
    clean();
}

void
ClientEditDialog::closeDialog()
{
    ALOGD("closeDialog...");
    this->close();
}

void
ClientEditDialog::clean()
{
    cleanContent();
}

void
ClientEditDialog::cleanContent()
{
    ui->telLineEdit->setText("");
    ui->faxLineEdit->setText("");
    ui->paidDSB->setValue(0);
    ui->emailLineEdit->setText("");
    ui->remarksFxtEdit->setText("");
    ui->amountDSB->setValue(0);
    ui->addressLineEdit->setText("");
    ui->balanceLineEdit->setText("");
    ui->contractLineEdit->setText("");
    ui->clientNumLineEdit->setText("");
    ui->clientNameLineEdit->setText("");
//    ui->createDateEdit->setDate("");
    ui->createPeopleLineEdit->setText("");
}

bool
ClientEditDialog::isModified()
{
    Client tmp;
    saveUiContent(tmp);
    if (mOriginClient->isValueEqual(tmp)) {
        return false;
    }
    return true;
}

void
ClientEditDialog::saveAndExitEvent()
{
    int ret;

    if (ui->clientNumLineEdit->text().isEmpty() ||
            ui->clientNameLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("温馨提示"),
                             tr("客户编号与客户名称不能为空！\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    if (!isModified()) {
        // 内容没变化，直接退出
        closeDialog();
        return;
    }

    Client client;
    saveUiContent(client);

    if (mOpenType == CREATEITEM) {
        // 添加条目
        if (mDb->isClientExist(client)) {
            QMessageBox::critical(this,
                                  tr("温馨提示"),
                                  tr("该客户已存在，添加失败!\n"),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
            return;
        }

        ret = mDb->insertClientTable(client);
        if (!ret) {
            resetView(client);
            addClientItemSignal(client);
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
        // 编辑条目
        ret = mDb->updateClientTableItem(client);
        if (!ret) {
            resetView(client);
            updateClientItemSignal(client);
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

void
ClientEditDialog::saveUiContent(Client &client)
{
    bool ok;
    client.name     = ui->clientNameLineEdit->text();
    client.number   = ui->clientNumLineEdit->text();
    client.telephone = ui->telLineEdit->text();
    client.address  = ui->addressLineEdit->text();
    client.email    = ui->emailLineEdit->text();
    client.fax      = ui->faxLineEdit->text();
    client.contract = ui->contractLineEdit->text();
    client.remarks  = ui->remarksFxtEdit->toPlainText();
    client.creator  = ui->createPeopleLineEdit->text();
    client.createDate = QDate::fromString(
                ui->createDateEdit->text(), DATE_FORMAT_STR);
    client.amount   = ui->amountDSB->value();
    client.paid     = ui->paidDSB->value();
    client.monthly = ui->monthlySpinBox->text().toInt(&ok, 10);

    if (ui->cashRadioButton->isChecked())
        client.paytype = PayType::CASH;
    else
        client.paytype = PayType::MONTHLY;

    if (ui->contractRadioButton->isChecked())
        client.clienttype = ClientType::CONTRACT;
    else
        client.clienttype = ClientType::TEMPORARY;
}

void
ClientEditDialog::saveEvent()
{
    Client client;

    saveUiContent(client);

    if (!mDb->updateClientTableItem(client)) {
        resetView(client);
        updateClientItemSignal(client);
    } else {
        QMessageBox::critical(this,
                              tr("温馨提示"),
                              tr("保存失败!未知错误.\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    }
}

void
ClientEditDialog::editEvent()
{
    setEditMode();
    // 已存在客户，其客户编号不可再编辑
    ui->clientNumLineEdit->setDisabled(true);
}

void
ClientEditDialog::cancelEvent()
{
    resetView();
    setViewMode();
}

void
ClientEditDialog::setEditMode()
{
    if (mOpenType == CREATEITEM)
        ui->clientNumLineEdit->setFocus();
    else
        ui->clientNameLineEdit->setFocus();

    if (mCurUserIsRoot) {
        mActEdit->setDisabled(true);
        mActCancel->setEnabled(true);
        mActSave->setEnabled(true);
    }
    setMode(true);
}

void
ClientEditDialog::setViewMode()
{
    if (mCurUserIsRoot) {
        mActCancel->setDisabled(true);
        mActEdit->setEnabled(true);
        mActSave->setDisabled(true);
    }
    setMode(false);
}

void
ClientEditDialog::setMode(bool mode)
{
    ui->clientNumLineEdit->setEnabled(mode);
    ui->clientNameLineEdit->setEnabled(mode);
    ui->addressLineEdit->setEnabled(mode);
    ui->telLineEdit->setEnabled(mode);
    ui->faxLineEdit->setEnabled(mode);
    ui->contractLineEdit->setEnabled(mode);
    ui->emailLineEdit->setEnabled(mode);
    ui->cashRadioButton->setEnabled(mode);
    ui->monthlyRadioButton->setEnabled(mode);
    ui->monthlySpinBox->setEnabled(mode);
    ui->remarksFxtEdit->setEnabled(mode);
    ui->paidDSB->setEnabled(mode);
    ui->amountDSB->setEnabled(mode);
//    ui->balanceLineEdit->setEnabled(mode);
    ui->createDateEdit->setEnabled(mode);
    ui->createPeopleLineEdit->setEnabled(mode);
    ui->contractRadioButton->setEnabled(mode);
    ui->temporaryRadioButton->setEnabled(mode);
}

bool
ClientEditDialog::isClientExist(Client &client)
{
   return mDb->isClientExist(client);
}
