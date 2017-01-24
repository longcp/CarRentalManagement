#include "clienteditdialog.h"
#include "ui_clienteditdialog.h"
#include <QToolBar>
#include <QMessageBox>
#include <regularexpression.h>

#define LOG_TAG                 "CLIENT_EDIT_DIALOG"
#include "utils/Log.h"

ClientEditDialog::ClientEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientEditDialog)
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
    ui->paidLineEdit->setMaxLength(16);
    ui->emailLineEdit->setMaxLength(64);
    ui->amountLineEdit->setMaxLength(16);
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

    QValidator *telValidator = new QRegExpValidator(
                                        RegularExpression::getTelRegExp(),
                                        this);                          //手机+固话+传真
    ui->telLineEdit->setValidator(telValidator);
}

void
ClientEditDialog::openClientEditDialogSlot(bool isAddClient)
{
    if (isAddClient) {
        mActSave->setDisabled(true);
        mActEdit->setDisabled(true);
        mActPrev->setDisabled(true);
        mActNext->setDisabled(true);
        mActCancel->setDisabled(true);
    } else {
        mActSave->setEnabled(true);
        mActPrev->setEnabled(true);
        mActNext->setEnabled(true);
        setViewMode();                                                  //默认为查看模式
        resetView();
    }

    mIsAddClient = isAddClient;
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
    mToolBar->setGeometry(0, 0, 0, 0);
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
    if (this->mIsAddClient) {
        // 添加条目
        ALOGD("mIsAddClient");
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
    ui->createDateLineEdit->setText("");
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
            ui->createDateLineEdit->isModified() ||
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
    //saveChange();
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
    ui->balanceLineEdit->setEnabled(mode);
    ui->createDateLineEdit->setEnabled(mode);
    ui->createPeopleLineEdit->setEnabled(mode);
    ui->contractRadioButton->setEnabled(mode);
    ui->temporaryRadioButton->setEnabled(mode);
}
