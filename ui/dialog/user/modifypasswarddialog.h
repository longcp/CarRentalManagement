#ifndef MODIFYPASSWARD_H
#define MODIFYPASSWARD_H

#include <QDialog>
#include "dataType.h"

class DataBase;
class User;

namespace Ui {
class ModifyPasswardDialog;
}

class ModifyPasswardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyPasswardDialog(QWidget *parent = 0);
    ~ModifyPasswardDialog();

private slots:

    /**
     * @brief on_ensureBtn_clicked:点击确认按钮
     */
    void                on_ensureBtn_clicked();

    /**
     * @brief on_cancelBtn_clicked:点击取消按钮
     */
    void                on_cancelBtn_clicked();

    /**
     * @brief open_modify_passward_window:打开修改密码窗口
     * @param uName
     */
    void                openWindow(QString uName);

signals:
    void                updateTableContentSignal(User &user);

private:
    /**
     * @brief 清空输入
     */
    void                clearInput();
    DataBase           *mDb;
    Ui::ModifyPasswardDialog *ui;
};

#endif // MODIFYPASSWARD_H
