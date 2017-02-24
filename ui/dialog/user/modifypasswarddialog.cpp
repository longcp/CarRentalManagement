#include "modifypasswarddialog.h"
#include "ui_modifypasswarddialog.h"
#include <QMessageBox>
#include <database/database.h>
#include <user.h>

ModifyPasswardDialog::ModifyPasswardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyPasswardDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("修改密码");
    this->setWindowFlags(Qt::CustomizeWindowHint
                         | Qt::WindowCloseButtonHint);                  //只留关闭按钮
    this->setFixedSize(350, 330);                                       //禁止窗口伸缩

    //密码，数字或英文，不超过12位
    QRegExp passwardRegx("[a-zA-Z0-9]+$");
    ui->oldPasswardLEdit->setEchoMode(QLineEdit::Password);
    ui->oldPasswardLEdit->setValidator(
                new QRegExpValidator(passwardRegx,
                                     ui->oldPasswardLEdit));
    ui->oldPasswardLEdit->setMaxLength(12);
    ui->oldPasswardLEdit->setFocus();

    ui->newPasswardLEdit->setEchoMode(QLineEdit::Password);
    ui->newPasswardLEdit->setValidator(
                new QRegExpValidator(passwardRegx,
                                     ui->newPasswardLEdit));
    ui->newPasswardLEdit->setMaxLength(12);

    ui->ensurePasswardLEdit->setEchoMode(QLineEdit::Password);
    ui->ensurePasswardLEdit->setValidator(
                new QRegExpValidator(passwardRegx,
                                     ui->ensurePasswardLEdit));
    ui->ensurePasswardLEdit->setMaxLength(12);

    mDb = DataBase::getInstance();
}

ModifyPasswardDialog::~ModifyPasswardDialog()
{
    delete ui;
}

void
ModifyPasswardDialog::on_ensureBtn_clicked()
{  
    int ret;
    QString uName;
    QString oldPassward;
    QString firPassward;
    QString secPassward;
    User user;

    QMessageBox msgBox;
    msgBox.setWindowTitle("温馨提示");
    msgBox.setText("输入不能为空!");
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, "确定");

    uName = ui->uNameLEdit->text();
    oldPassward = ui->oldPasswardLEdit->text();
    firPassward = ui->newPasswardLEdit->text();
    secPassward = ui->ensurePasswardLEdit->text();

    if ((oldPassward != NULL) &&
            (firPassward != NULL) &&
            (secPassward != NULL)) {
        if (!firPassward.compare(secPassward)) {
            //两次密码一致
            ret = mDb->getUserTableData(user, uName);
            if (!ret) {
                //输入旧密码正确
                if (!oldPassward.compare(user.passward)) {
                    user.name = uName;
                    user.passward = firPassward;

                    ret = mDb->updateUserTableData(user);
                    if (!ret) {
                        //修改成功
                        clearInput();
                        emit updateTableContentSignal(user);
                        msgBox.setIcon(QMessageBox::Information);
                        msgBox.setText("修改成功!");
                        msgBox.exec();
                        this->close();
                    } else {
                        msgBox.setText("未知错误!");
                        msgBox.exec();
                        clearInput();
                    }
                } else {
                    msgBox.setText("原始密码不正确!");
                    msgBox.exec();
                    clearInput();
                }
            }
        } else {
            //两次密码不一致
            msgBox.setText("新密码不一致!");
            msgBox.exec();
            clearInput();
        }
    } else {
        msgBox.exec();
        clearInput();
    }
}

void
ModifyPasswardDialog::on_cancelBtn_clicked()
{
    clearInput();
    this->close();
}

void
ModifyPasswardDialog::openWindow(QString uName)
{
    ui->uNameLEdit->setText(uName);
    ui->uNameLEdit->setEnabled(false);                                  //禁止编辑
    this->exec();
}

void
ModifyPasswardDialog::clearInput()
{
    ui->oldPasswardLEdit->clear();
    ui->newPasswardLEdit->clear();
    ui->ensurePasswardLEdit->clear();
}
