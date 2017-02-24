#include "adduserdialog.h"
#include "ui_adduserdialog.h"
#include <QMessageBox>
#include <database/database.h>
#include <user.h>

AddUserDialog::AddUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUserDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("添加用户");
    this->setWindowFlags(Qt::CustomizeWindowHint
                         | Qt::WindowCloseButtonHint);                  //只留关闭按钮
    this->setFixedSize(350, 300);                                       //禁止窗口伸缩

    //密码，数字或英文，不超过12位
    QRegExp passwardRegx("[a-zA-Z0-9]+$");
    ui->passwardLEdit->setEchoMode(QLineEdit::Password);
    ui->passwardLEdit->setValidator(
                new QRegExpValidator(passwardRegx,
                                     ui->passwardLEdit));
    ui->passwardLEdit->setMaxLength(12);
    ui->ensurePasswardLEdit->setEchoMode(QLineEdit::Password);
    ui->ensurePasswardLEdit->setValidator(
                new QRegExpValidator(passwardRegx,
                                     ui->ensurePasswardLEdit));
    ui->ensurePasswardLEdit->setMaxLength(12);

    mDb = DataBase::getInstance();
}

AddUserDialog::~AddUserDialog()
{
    delete ui;
}

void
AddUserDialog::openWindow()
{
    this->exec();
}

void AddUserDialog::on_ensureBtn_clicked()
{
    int ret;
    QString uName;
    QString firPassward;
    QString secPassward;
    User user;

    QMessageBox errInfoMsgBox;
    errInfoMsgBox.setIcon(QMessageBox::Critical);
    errInfoMsgBox.setWindowTitle("添加失败");
    errInfoMsgBox.setText("输入不能为空");
    errInfoMsgBox.setStandardButtons(QMessageBox::Ok);
    errInfoMsgBox.setButtonText(QMessageBox::Ok,
                                QString("确 定"));

    uName = ui->uNameLEdit->text();
    firPassward = ui->passwardLEdit->text();
    secPassward = ui->ensurePasswardLEdit->text();

    if ((uName != NULL) && (firPassward != NULL) && (secPassward != NULL)) {
        //正确输入
        if (!firPassward.compare(secPassward)) {
            //密码一致
            user.name     = uName;
            user.passward = firPassward;
            if (ui->userRightCBbox->checkState() == Qt::Checked)
                user.right = UserRight::RIGHT_ROOT;
            else
                user.right = UserRight::RIGHT_NORMAL;
            ret = mDb->insertUserTable(user);
            if (!ret) {
                //添加成功
                emit addrUserSuccessSignal(uName);
                errInfoMsgBox.setWindowTitle("添加成功");
                errInfoMsgBox.setIcon(QMessageBox::Information);
                errInfoMsgBox.setText("添加成功！");
                errInfoMsgBox.exec();
                clearInput();
                this->close();
            } else {
                //添加失败
                errInfoMsgBox.setText("该用户已存在");
                errInfoMsgBox.exec();
                clearInput();
            }
        } else {
            //密码不一致
            errInfoMsgBox.setText("密码不一致");
            errInfoMsgBox.exec();
            clearInput();
        }
    } else {
        errInfoMsgBox.exec();
        clearInput();
    }
}

void
AddUserDialog::clearInput()
{
    ui->uNameLEdit->clear();
    ui->passwardLEdit->clear();
    ui->ensurePasswardLEdit->clear();
    ui->userRightCBbox->setChecked(Qt::Unchecked);
}

void
AddUserDialog::closeEvent(QCloseEvent *event)
{
    clearInput();
}

void AddUserDialog::on_cancelBtn_clicked()
{
    clearInput();
    this->close();
}
