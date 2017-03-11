#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>

class DataBase;

namespace Ui {
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit            AddUserDialog(QWidget *parent = 0);
    ~AddUserDialog();

private slots:
    /**
     * @brief 打开窗口
     */
    void                openWindow();
    /**
     * @brief 点击确定
     */
    void                on_ensureBtn_clicked();
    /**
     * @brief 点击取消
     */
    void                on_cancelBtn_clicked();

signals:
    /**
     * @brief 添加用户成功，刷新设备管理列表
     */
    void                addrUserSuccessSignal(QString uName);

private:
    /**
     * @brief clear_input:清空输入
     */
    void                clearInput();
    /**
     * @brief 关闭
     */
    void                closeEvent(QCloseEvent *event);
    DataBase            *mDb;
    Ui::AddUserDialog *ui;
};

#endif // ADDUSERDIALOG_H
