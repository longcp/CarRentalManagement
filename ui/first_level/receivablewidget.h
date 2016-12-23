#ifndef RECEIVABLEWIDGET_H
#define RECEIVABLEWIDGET_H

#include <QWidget>

class QToolBar;

namespace Ui {
class ReceivableWidget;
}

class ReceivableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReceivableWidget(QWidget *parent = 0);
    ~ReceivableWidget();

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();

    Ui::ReceivableWidget *ui;
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

#endif // RECEIVABLEWIDGET_H
