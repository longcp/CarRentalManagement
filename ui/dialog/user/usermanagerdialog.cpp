#include "usermanagerdialog.h"
#include "ui_usermanagerdialog.h"
#include <QStandardItem>
#include <QMessageBox>
#include "tablemodel.h"
#include "database/database.h"
//#include "adduser.h"
#include "modifypasswarddialog.h"
#include <user.h>

#define LOG_TAG                         "USERMANAGER"
#include "utils/Log.h"

#define USER_TABLE_COLUMN_COUNT 1
#define USER_NAME_COLUMN        0
#define USER_RIGHT_COLUMN       1

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
//    mAddUser = new AddUser();
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

#if 0
    /**
     * 访问网管板超时，弹出提示窗口
     */
    connect(this,    SIGNAL(open_addUser_window_signal()),
            mAddUser, SLOT(open_addUser_window()));

    /**
     * 添加用户成功，列表更新
     */
    connect(mAddUser,    SIGNAL(addr_user_success_signal(QString)),
            this, SLOT(table_view_append_row(QString)));

#endif
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
//    emit open_addUser_window_signal();
}

void
UserManagerDialog::on_delUserBtn_clicked()
{
#if 0
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

    QModelIndexList modelList = ui->userInfoTableview->selectionModel()->selectedRows();
    if (modelList.size() != 0) {
        ret = msgBox.exec();
        if (ret == QMessageBox::Cancel) {
            return;
        }
        modelIndex = modelList[0];
        uName = modelIndex.data().toString();
        ret = mDb->get_user_count();
        if (ret == 1) {
            //最后一个用户无法删除
            msgBox.setText("最后一个用户无法删除");
            msgBox.setStandardButtons(QMessageBox::Yes);
            msgBox.setButtonText(QMessageBox::Yes, "确定");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        } else {
            ret = mDb->delete_user_table_data(uName);                       //数据库删除条目
            if (!ret) {
                currentRow = modelList[0].row();
                mModel->takeRow(currentRow);                                //删除该行数据
                send_del_user_record(uName);
                msgBox.setText("用户已删除");
                msgBox.setStandardButtons(QMessageBox::Yes);
                msgBox.setButtonText(QMessageBox::Yes, "确定");
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
            }
        }
    }
#endif
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
    }
}

void
UserManagerDialog::table_view_append_row(QString uName)
{
#if 0
    int ret;
    USER_INFO userInfo;

    ret = mDb->get_user_table_data(&userInfo, uName);
    if (!ret) {
        QStandardItem *nameItem     = new QStandardItem(userInfo.name);
        QString rightStr = GET_RIGHT_STR(userInfo.rightLevel);
        QStandardItem *rightItem    =  new QStandardItem(rightStr);
        QList<QStandardItem*> items;
        items << nameItem << rightItem;
        mModel->appendRow(items);                                   //插入条目表中
    }

    QString content = "添加用户: " + uName;
    send_oper_record(content);
#endif
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

void
UserManagerDialog::send_del_user_record(QString userName)
{
    QString content = "删除用户: " + userName;
//    emit add_user_operRec_signal(OPERATE_TYPE_USERINFO, content);
}

void
UserManagerDialog::send_oper_record(QString content)
{
//    emit add_user_operRec_signal(OPERATE_TYPE_USERINFO, content);
}
