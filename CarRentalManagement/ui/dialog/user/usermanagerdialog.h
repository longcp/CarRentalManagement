#ifndef USERMANAGERDIALOG_H
#define USERMANAGERDIALOG_H

#include <QDialog>
#include "dataType.h"

class AddUserDialog;
class DataBase;
class ModifyPassward;
class TableModel;
class User;
class ModifyPasswardDialog;

namespace Ui {
class UserManagerDialog;
}

class UserManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserManagerDialog(QWidget *parent = 0);
    ~UserManagerDialog();

private slots:

    /**
     * @brief open_user_manager_window:打开用户信息管理窗口
     */
    void                 openWindow();

    /**
     * @brief on_closeBtn_clicked:关闭窗口
     */
    void                 on_closeBtn_clicked();

    /**
     * @brief on_addUserBtn_clicked:点击添加按钮
     */
    void                 on_addUserBtn_clicked();

    /**
     * @brief on_delUserBtn_clicked:点击删除按钮
     */
    void                 on_delUserBtn_clicked();

    /**
     * @brief on_modifyBtn_clicked:点击修改密码按钮
     */
    void                 on_modifyBtn_clicked();

    /**
     * @brief appendRow:更新用户列表
     * @param uName
     */
    void                 appendRow(QString uName);

    /**
     * @brief update_table_content:更新表内容
     * @param userInfo
     */
    void                 updateTableContent(User &user);

    void                 setWindow(QString userName);

signals:

    /**
     * @brief openAddUserWindowSignal:打开添加用户窗口
     */
    void                 openAddUserWindowSignal();

    /**
     * @brief open_modify_passward_window:打开修改密码窗口
     * @param uname
     */
    void                 openMdifyPasswardWindowSignal(QString uname);

    /**
     * @brief add_operate_record_signal:添加操作记录
     */
    void                 add_user_operRec_signal(int type, QString content);

private:

    /**
     * @brief init_userManager_window:初始化窗口信息
     */
    void                 initView();

    int                  mCurrentRow;                                   //当前选中行
    QStringList          mHeaderList;                                   //表头
    AddUserDialog       *mAddUserDialog;                                //添加用户
    ModifyPasswardDialog      *mModifyPasswardDialog;                   //修改密码
    DataBase            *mDb;
    TableModel          *mModel;

    UserRight           mCurUserRight;
    QString             mCurUserName;

    Ui::UserManagerDialog     *ui;
};

#endif // USERMANAGERDIALOG_H
