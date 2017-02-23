#ifndef RECEIPTWIDGET_H
#define RECEIPTWIDGET_H

#include <QWidget>

class QToolBar;
class ReceiptEditDialog;

namespace Ui {
class ReceiptWidget;
}

class ReceiptWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptWidget(QWidget *parent = 0);
    ~ReceiptWidget();

private slots:
    /**
     * @brief 双击列表响应
     * @param a
     * @param b
     */
    void            cellDoubleClickedSlot(int a,int b);

signals:
    /**
     * @brief 打开收入编辑窗口
     */
    void            openReceiptEditDialogSignal();

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();

    Ui::ReceiptWidget *ui;
    QToolBar        *mToolBar;

    // 编辑客户资料窗口
    ReceiptEditDialog    *mReceiptEditDialog;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;
};

#endif // RECEIPTWIDGET_H
