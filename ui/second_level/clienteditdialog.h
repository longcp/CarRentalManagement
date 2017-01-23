#ifndef CLIENTEDITDIALOG_H
#define CLIENTEDITDIALOG_H

#include <QDialog>

class QToolBar;

namespace Ui {
class ClientEditDialog;
}

class ClientEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientEditDialog(QWidget *parent = 0);
    ~ClientEditDialog();

private slots:
    /**
     * @brief 打开窗口
     */
    void            openClientEditDialogSlot(bool isAddClient);
    /**
     * @brief 关闭窗口
     */
    void            closeDialog();
    /**
     * @brief 保存退出
     */
    void            saveAndExitEvent();
    /**
     * @brief 保存
     */
    void            saveEvent();
    /**
     * @brief 窗口关闭事件
     */
    void            closeEvent(QCloseEvent *event);
    /**
     * @brief 编辑事件
     */
    void            editEvent();
    /**
     * @brief 取消事件
     */
    void            cancelEvent();

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    /**
     * @brief 设置模式 编辑/查看
     */
    void            setMode(bool mode);
    /**
     * @brief 清理资源
     */
    void            clean();
    /**
     * @brief 清理内容
     */
    void            cleanContent();
    /**
     * @brief
     */
    bool            isModified();
    /**
     * @brief 初始化相关参数
     */
    void            initParam();
    /**
     * @brief 重置界面相关内容
     */
    void            resetView();
    /**
     * @brief 设置编辑模式
     */
    void            setEditMode();
    /**
     * @brief 设置查看模式
     */
    void            setViewMode();

    Ui::ClientEditDialog *ui;
    // 工具栏
    QToolBar        *mToolBar;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActSaveExit;
    QAction         *mActSave;
    QAction         *mActExit;
    QAction         *mActEdit;
    QAction         *mActPrev;
    QAction         *mActNext;
    QAction         *mActCancel;

    bool            mIsAddClient;                                       //是否已添加客户模式打开
    bool            mIsInternalClose;                                   //是否内部关闭
};

#endif // CLIENTEDITDIALOG_H
