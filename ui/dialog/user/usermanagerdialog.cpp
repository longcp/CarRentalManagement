#include "usermanagerdialog.h"
#include "ui_usermanagerdialog.h"
#include <QStandardItem>
#include <QMessageBox>
#include "tablemodel.h"
#include "database/database.h"
#include "adduserdialog.h"
#include "modifypasswarddialog.h"
#include <user.h>

#define LOG_TAG                         "USERMANAGER"
#include "utils/Log.h"

#define USER_TABLE_COLUMN_COUNT 1
#define USER_NAME_COLUMN        0
#define USER_RIGHT_COLUMN       1

#define ADMIN_USER_NAME                 "admin"

UserManagerDialog::UserManagerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManagerDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("用户信息管理");
    this->setWindowFlags(Qt::CustomizeWindowHint
                         | Qt::WindowCloseButtonHint);                  //只留关闭按钮
    this->setFixedSize(500, 220);                                       //禁止窗口伸缩

    mDb       = DataBase::getInstance();
    mAddUserDialog = new AddUserDialog();
    mModifyPasswardDialog = new ModifyPasswardDialog();

    //初始化告警界面
    mModel = new TableModel(0, USER_TABLE_COLUMN_COUNT);
    ui->userInfoTableview->setModel(mModel);

    mHeaderList << "用户名" << "用户权限";
    mModel->setHorizontalHeaderLabels(mHeaderList);
    ui->userInfoTableview->verticalHeader()->setVisible(false);
    ui->userInfoTableview->horizontalHeader()->setStretchLastSection(true);

    //设备单元格不可编辑,单击选中一行且只能选中一行
    ui->userInfoTableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->userInfoTableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->userInfoTableview->setSelectionMode(QAbstractItemView::SingleSelection);

    /**
     * 访问网管板超时，弹出提示窗口
     */
    connect(this,    SIGNAL(openAddUserWindowSignal()),
            mAddUserDialog, SLOT(openWindow()));
    /**
     * 添加用户成功，列表更新
     */
    connect(mAddUserDialog, SIGNAL(addrUserSuccessSignal(QString)),
            this, SLOT(appendRow(QString)));
    /**
     * 打开修改密码窗口
     */
    connect(this,    SIGNAL(openMdifyPasswardWindowSignal(QString)),
            mModifyPasswardDialog, SLOT(openWindow(QString)));
    /**
     * 用户修改密码成功，列表更新
     */
    connect(mModifyPasswardDialog,    SIGNAL(updateTableContentSignal(User &)),
            this, SLOT(updateTableContent(User &)));
}

UserManagerDialog::~UserManagerDialog()
{
    delete ui;
}

void
UserManagerDialog::openWindow()
{
    this->exec();
}

void
UserManagerDialog::setWindow(QString userName)
{
    User user;

    if (mDb->getUserTableData(user, userName))
        return;

    ALOGD("%s, name = %s, passwd = %s, right = %d",
          __FUNCTION__,
          user.name.toStdString().data(),
          user.passward.toStdString().data(),
          user.right);
    mCurUserRight = user.right;
    initView();                                                     //初始化用户信息管理表

    if (user.right == UserRight::RIGHT_ROOT) {
        return;
    } else {
        ui->addUserBtn->setEnabled(false);
        ui->delUserBtn->setEnabled(false);
    }
}

void
UserManagerDialog::initView()
{
    int i;
    int ret;
    int size;
    User user;
    QList<User>users;

    if (mCurUserRight != UserRight::RIGHT_ROOT)
        return;

    ret = mDb->getAllUserTableData(users);
    if ((!ret) && (!users.isEmpty())) {
        size = users.size();
        for (i = 0; i < size; i++) {
            user = users.at(i);
            QStandardItem *nameItem =  new QStandardItem(user.name);
            QString rightStr = user.getUserRightStr(user.right);
            QStandardItem *rightItem    =  new QStandardItem(rightStr);
            QList<QStandardItem*> items;
            items << nameItem << rightItem ;
            mModel->appendRow(items);                                   //插入条目表中
        }
    }
}

void
UserManagerDialog::on_closeBtn_clicked()
{
    this->close();
}

void
UserManagerDialog::on_addUserBtn_clicked()
{
    emit openAddUserWindowSignal();
}

void
UserManagerDialog::on_delUserBtn_clicked()
{
    int ret;
    int currentRow;
    QModelIndex modelIndex;
    QString uName;

    QMessageBox msgBox;
    msgBox.setWindowTitle("删除用户");
    msgBox.setText("确定要删除用户？");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Yes, "确定");
    msgBox.setButtonText(QMessageBox::Cancel, "取消");

    QModelIndexList modelList
            = ui->userInfoTableview->selectionModel()->selectedRows();
    if (modelList.size() != 0) {
        ret = msgBox.exec();
        if (ret == QMessageBox::Cancel) {
            return;
        }
        modelIndex = modelList[0];
        uName = modelIndex.data().toString();
        if (uName == ADMIN_USER_NAME) {
            //admin用户无法删除
            msgBox.setText("admin用户无法删除");
            msgBox.setStandardButtons(QMessageBox::Yes);
            msgBox.setButtonText(QMessageBox::Yes, "确定");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        } else {
            ret = mDb->deleteUserTabledata(uName);                          //数据库删除条目
            if (!ret) {
                currentRow = modelList[0].row();
                mModel->takeRow(currentRow);                                //删除该行数据
                msgBox.setText("用户已删除");
                msgBox.setStandardButtons(QMessageBox::Yes);
                msgBox.setButtonText(QMessageBox::Yes, "确定");
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
            }
        }
    } else {
        QMessageBox::critical(this, tr("温馨提示"),
                             tr("请选择要删除的用户.\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    }
}

void
UserManagerDialog::on_modifyBtn_clicked()
{
    QString uName;
    QModelIndex modelIndex;

    QModelIndexList modelList
            = ui->userInfoTableview->selectionModel()->selectedRows();
    if (modelList.size() != 0) {
        modelIndex  = modelList[0];
        mCurrentRow = modelList[0].row();
        uName = modelIndex.data().toString();
        emit openMdifyPasswardWindowSignal(uName);
    } else {
        QMessageBox::critical(this, tr("温馨提示"),
                             tr("请选择要修改的用户.\n"),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    }
}

void
UserManagerDialog::appendRow(QString uName)
{
    User user;

    if (!mDb->getUserTableData(user, uName)) {
        QStandardItem *nameItem = new QStandardItem(user.name);
        QString rightStr = user.getUserRightStr(user.right);
        QStandardItem *rightItem =  new QStandardItem(rightStr);
        QList<QStandardItem*> items;
        items << nameItem << rightItem;
        mModel->appendRow(items);                                   //插入条目表中
    }
}

void
UserManagerDialog::updateTableContent(User &user)
{
    QStandardItem *nameItem     = new QStandardItem(user.name);
    QStandardItem *rightItem    = new QStandardItem(user
                                                    .getUserRightStr(user.right));
    mModel->setItem(mCurrentRow, USER_NAME_COLUMN, nameItem);
    mModel->setItem(mCurrentRow, USER_RIGHT_COLUMN, rightItem);
}
