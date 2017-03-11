#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPixmap>
#include <QtMath>
#include <database/database.h>
#include <user.h>
#include <datatype.h>
#include <client.h>

#define LOG_TAG                         "LOGIN  "
#include "utils/Log.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("用户登陆");
    this->setWindowOpacity(1);

    this->setWindowFlags(Qt::FramelessWindowHint|
                         Qt::WindowSystemMenuHint); //无边框
    mDb = DataBase::getInstance();

    //最大最小化图标
    QPixmap minPix  = style()->standardPixmap(QStyle
                                              ::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle
                                               ::SP_TitleBarCloseButton);
    ui->minimumBtn->setIcon(minPix);
    ui->closeBtn->setIcon(closePix);
    //提示信息
    ui->minimumBtn->setToolTip(tr("最小化"));
    ui->closeBtn->setToolTip(tr("关闭"));

    mDrag = false;                                                          //一开始，鼠标为“释放”状态
    //密码，数字或英文，不超过12位
    QRegExp passwardRegx("[a-zA-Z0-9]+$");
    ui->passwardLEdit->setEchoMode(QLineEdit::Password);
    ui->passwardLEdit->setValidator(
                new QRegExpValidator(passwardRegx,
                                     ui->passwardLEdit));
    ui->passwardLEdit->setMaxLength(12);
    ui->userNameLEdit->setFocus();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void
LoginDialog::on_loginBtn_clicked()
{
    QString uName;
    QString passward;
    User user;
    int ret;

    uName = ui->userNameLEdit->text();
    passward = ui->passwardLEdit->text();

    if ((uName == NULL) || (passward == NULL)) {
        //有空输入
        ui->userNameLEdit->clear();
        ui->passwardLEdit->clear();
        QMessageBox errInfoMsgBox(QMessageBox::Critical,
                                  tr("登陆失败"),
                                  tr("输入不能为空"));
        errInfoMsgBox.setStandardButtons(QMessageBox::Ok);
        errInfoMsgBox.setButtonText(QMessageBox::Ok,
                                    QString("确 定"));
        errInfoMsgBox.exec();
    } else {
        ret = mDb->getUserTableData(user, uName);
        if (ret) {
            ui->userNameLEdit->clear();
            ui->passwardLEdit->clear();
            QMessageBox errunKnownMsgBox(QMessageBox::Critical,
                                         tr("登陆失败"),
                                         tr("未知出错，请重试"));
            errunKnownMsgBox.setStandardButtons(QMessageBox::Ok);
            errunKnownMsgBox.setButtonText(QMessageBox::Ok,
                                           QString("确 定"));
            errunKnownMsgBox.exec();
        } else {
            if (uName != user.name) {
                ui->userNameLEdit->clear();
                ui->passwardLEdit->clear();
                QMessageBox errUnameMsgBox(QMessageBox::Critical,
                                           tr("登陆失败"),
                                           tr("没有找到该用户"));
                errUnameMsgBox.setStandardButtons(QMessageBox::Ok);
                errUnameMsgBox.setButtonText(QMessageBox::Ok,
                                             QString("确 定"));
                errUnameMsgBox.exec();
            } else if (passward != user.passward) {
                //密码不一致
                ui->passwardLEdit->clear();
                QMessageBox errPasswardMsgBox(QMessageBox::Critical,
                                              tr("登陆失败"),
                                              tr("输入密码不正确！"));
                errPasswardMsgBox.setStandardButtons(QMessageBox::Ok);
                errPasswardMsgBox.setButtonText(QMessageBox::Ok,
                                                QString("确 定"));
                errPasswardMsgBox.exec();
            } else {
                //成功登陆
                this->hide();
                emit userLoginSignal(user.name);
            }
        }
    }
}

void
LoginDialog::closeEvent(QCloseEvent *event)
{
    //关闭软件
    exit(0);
}

void
LoginDialog::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Enter
            || event->key() == Qt::Key_Return) {
        //捕捉enter键盘事件
        ALOGD("enter!");
        on_loginBtn_clicked();
    }
}

void
LoginDialog::reject()
{
    //不做任何操作，防止按下ESC按键直接跳过登录框
}

void
LoginDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mDrag = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void
LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (mDrag && (event->buttons() && Qt::LeftButton)) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void
LoginDialog::mouseReleaseEvent(QMouseEvent *)
{
    mDrag = false;
}

void
LoginDialog::on_minimumBtn_clicked()
{
    this->showMinimized();
}

void
LoginDialog::on_closeBtn_clicked()
{
    exit(0);
}

//绘制实现窗体阴影
void
LoginDialog::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(5, 5, this->width()-10, this->height()-10);


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));
}
