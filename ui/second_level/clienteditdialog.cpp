#include "clienteditdialog.h"
#include "ui_clienteditdialog.h"
#include <QToolBar>
#include <QMessageBox>
#include <regularexpression.h>
#include <client.h>
#include <datatype.h>
#include <QDebug>
#include "database/database.h"

#define LOG_TAG                 "CLIENT_EDIT_DIALOG"
#include "utils/Log.h"

ClientEditDialog::ClientEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientEditDialog)
{
    ui->setupUi(this);
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

    mToolBar = new QToolBar(tr("clientEditToolBar"), this);
    this->configToolBar();
    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActPrev);
    mToolBar->addAction(mActNext);
    mToolBar->addAction(mActCancel);
    mToolBar->addAction(mActExit);

    ui->toolBarVerticalLayout->addWidget(mToolBar);
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
    ui->paidLineEdit->setMaxLength(32);
    ui->emailLineEdit->setMaxLength(64);
    ui->amountLineEdit->setMaxLength(32);
    ui->addressLineEdit->setMaxLength(64);
    ui->balanceLineEdit->setMaxLength(16);
    ui->contractLineEdit->setMaxLength(16);
    ui->clientNameLineEdit->setMaxLength(16);
    ui->createPeopleLineEdit->setMaxLength(16);

    /* 输入格式限制 */
    QValidator *letterAndValidator = new QRegExpValidator(
                                        RegularExpression::getLetterAndNumRegExp(),
                                        this);                          //英文字母+数字
    ui->clientNumLineEdit->setValidator(letterAndValidator);

    QValidator *emailValidator = new QRegExpValidator(
                                        RegularExpression::getEmailRegExp(),
                                        this);                          //email
    ui->emailLineEdit->setValidator(emailValidator);

    QValidator *nameValidator = new QRegExpValidator(
                                        RegularExpression::getNameRegExp(),
                                        this);                          //英文字母+数字+中文
    ui->clientNameLineEdit->setValidator(nameValidator);
    ui->addressLineEdit->setValidator(nameValidator);
    ui->contractLineEdit->setValidator(nameValidator);
    ui->createPeopleLineEdit->setValidator(nameValidator);

    QValidator *telValidator = new QRegExpValidator(
                                        RegularExpression::getTelRegExp(),
                                        this);                          //手机+固话+传真
    ui->telLineEdit->setValidator(telValidator);
    ui->faxLineEdit->setValidator(telValidator);

    QValidator *moneyValidator = new QRegExpValidator(
                                        RegularExpression::getMoneyRegExp(),
                                        this);                          //金额
    ui->paidLineEdit->setValidator(moneyValidator);
    ui->amountLineEdit->setValidator(moneyValidator);
    ui->balanceLineEdit->setValidator(moneyValidator);
}

void
ClientEditDialog::openClientEditDialogSlot(OpenType opentype,
                                           const Client & client)
{
    if (opentype == OpenType::CREATEITEM) {
        //以创建条目方式打开
        mActSave->setDisabled(true);
        mActEdit->setDisabled(true);
        mActPrev->setDisabled(true);
        mActNext->setDisabled(true);
        mActCancel->setDisabled(true);
        ui->monthlyRadioButton->setChecked(true);
        ui->temporaryRadioButton->setChecked(true);
        ui->createDateEdit->setDate(QDate::currentDate());
    } else {
        //以查看内容方式打开
        mActSave->setEnabled(true);
        mActPrev->setEnabled(true);
        mActNext->setEnabled(true);
        setViewMode();                                                  //默认为查看模式
        resetView();
    }

    mOpenType = opentype;
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
ClientEditDialog::resetView()
{

}

void
ClientEditDialog::closeEvent(QCloseEvent *event)
{
    ALOGD("closeEvent");
    if (!mIsInternalClose) {
        mIsInternalClose = false;
        closeDialog();
    }
}

void
ClientEditDialog::closeDialog()
{
    ALOGD("closeDialog...");
    if (mOpenType == CREATEITEM) {
        // 添加条目
        ALOGD("CREATEITEM");
    } else if (isModified()) {
        // 有内容发生修改
        ALOGD("isModified");
        int ret = QMessageBox::warning(this, tr("温馨提示"),
                                       tr("是否保存修改？\n"),
                                       QMessageBox::Yes |
                                       QMessageBox::No |
                                       QMessageBox::Cancel,
                                       QMessageBox::Yes);
        switch(ret) {
            case QMessageBox::Yes:
                ALOGD("save");
                break;
            case QMessageBox::No:
                ALOGD("Discard");
                break;
            case QMessageBox::Cancel:
                ALOGD("Cancel");
                break;
            default:
                break;
        }
    }
    clean();
    mIsInternalClose = true;
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
    ui->paidLineEdit->setText("");
    ui->emailLineEdit->setText("");
    ui->remarksFxtEdit->setText("");
    ui->amountLineEdit->setText("");
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
    if (ui->telLineEdit->isModified() ||
            ui->faxLineEdit->isModified() ||
            ui->paidLineEdit->isModified() ||
            ui->emailLineEdit->isModified() ||
            ui->monthlySpinBox->isWindowModified() ||
            ui->remarksFxtEdit->isWindowModified() ||
            ui->amountLineEdit->isModified() ||
            ui->addressLineEdit->isModified() ||
            ui->cashRadioButton->isWindowModified() ||
            ui->balanceLineEdit->isModified() ||
            ui->contractLineEdit->isModified() ||
            ui->clientNumLineEdit->isModified() ||
            ui->clientNameLineEdit->isModified() ||
            ui->monthlyRadioButton->isWindowModified() ||
            ui->contractRadioButton->isWindowModified() ||
            ui->temporaryRadioButton->isWindowModified() ||
            ui->createPeopleLineEdit->isWindowModified()) {
        ALOGD("is modified!!!!");
        return true;
    }

    return false;
}

void
ClientEditDialog::saveAndExitEvent()
{
    if (ui->clientNumLineEdit->text().isEmpty() ||
            ui->clientNameLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("温馨提示"),
                             tr("客户编号与客户名称不能为空！\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    bool ok;
    Client client;
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
                ui->createDateEdit->text(), "yyyy-MM-dd");
    client.amount   = ui->amountLineEdit->text().toFloat(&ok);
    client.paid     = ui->paidLineEdit->text().toFloat(&ok);
    client.monthly = ui->monthlySpinBox->text().toInt(&ok, 10);
    if (ui->cashRadioButton->isChecked())
        client.paytype = Client::CASH;
    else
        client.paytype = Client::MONTHLY;

    if (ui->contractRadioButton->isChecked())
        client.clienttype = Client::CONTACT;
    else
        client.clienttype = Client::TEMPORARY;

    ALOGD("name = %s, number = %s, telephone = %s, \n"
          "address = %s, email = %s, fax = %s, \n"
          "contract = %s, remarks = %s, creator = %s, \n"
          "createDate = %s, paytype = %d, monthly = %d, \n"
          "clienttype = %d\n",
          client.name.toStdString().data(),
          client.number.toStdString().data(),
          client.telephone.toStdString().data(),
          client.address.toStdString().data(),
          client.email.toStdString().data(),
          client.fax.toStdString().data(),
          client.contract.toStdString().data(),
          client.remarks.toStdString().data(),
          client.creator.toStdString().data(),
          client.createDate.toString("yyyy-MM-dd").toStdString().data(),
          client.paytype,
          client.monthly, client.clienttype);

    if (mDb->insertClientTable(client))
        ALOGE("insertClientTable failed!");
//    saveChange();

    addClientItemSignal(client);
    QMessageBox::information(this,
                             tr("温馨提示"),
                             tr("添加成功\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    closeDialog();
}

void
ClientEditDialog::saveEvent()
{
    // TODO 先插入数据库
    resetView();
}

void
ClientEditDialog::editEvent()
{
    setEditMode();
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
    mActEdit->setDisabled(true);
    mActCancel->setEnabled(true);
    setMode(true);
}

void
ClientEditDialog::setViewMode()
{
    mActCancel->setDisabled(true);
    mActEdit->setEnabled(true);
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
    ui->paidLineEdit->setEnabled(mode);
    ui->amountLineEdit->setEnabled(mode);
//    ui->balanceLineEdit->setEnabled(mode);
    ui->createDateEdit->setEnabled(mode);
    ui->createPeopleLineEdit->setEnabled(mode);
    ui->contractRadioButton->setEnabled(mode);
    ui->temporaryRadioButton->setEnabled(mode);
}
