#ifndef RECEIPTWIDGET_H
#define RECEIPTWIDGET_H

#include <QWidget>

class QToolBar;

namespace Ui {
class ReceiptWidget;
}

class ReceiptWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptWidget(QWidget *parent = 0);
    ~ReceiptWidget();

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    Ui::ReceiptWidget *ui;
    QToolBar        *mToolBar;

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
