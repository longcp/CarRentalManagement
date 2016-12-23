#ifndef CONTRACTWIDGET_H
#define CONTRACTWIDGET_H

#include <QWidget>

class QToolBar;

namespace Ui {
class ContractWidget;
}

class ContractWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContractWidget(QWidget *parent = 0);
    ~ContractWidget();

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();

    Ui::ContractWidget *ui;
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

#endif // CONTRACTWIDGET_H
