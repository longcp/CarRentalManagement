#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

class DataBase;
class User;
class Client;

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit            LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    /**
     * @brief on_loginBtn_clicked:登录按钮
     */
    void                on_loginBtn_clicked();
    /**
     * @brief 最小化
     */
    void                on_minimumBtn_clicked();
    /**
     * @brief 关闭
     */
    void                on_closeBtn_clicked();

signals:

    /**
     * @brief 关闭主窗口
     */
    void                close_mainWindow_signal();

    /**
     * @brief userLoginedSignal
     * @param uName
     */
    void                userLoginedSignal(User &user);

    /**
     * @brief add_operate_record_signal:添加操作记录
     * @param type
     * @param content
     */
    void                add_login_record_signal(int type, QString content);

private:
    /**
     * @brief closeEvent:窗口关闭事件
     * @param event
     */
    void                closeEvent(QCloseEvent *event);
    void                keyPressEvent(QKeyEvent * event);

    void                mousePressEvent(QMouseEvent *event);
    void                mouseMoveEvent(QMouseEvent *event);
    void                mouseReleaseEvent(QMouseEvent *);

    void                paintEvent(QPaintEvent *event);

    /**
     * @brief 重载ESC按键事件
     */
    void                reject();

    bool                mDrag;
    QPoint              dragPosition;
    DataBase            *mDb;
    QTimer              *mTimer;
    Ui::LoginDialog     *ui;
};

#endif // LOGIN_H
