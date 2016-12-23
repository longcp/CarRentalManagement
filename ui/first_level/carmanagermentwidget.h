#ifndef CARMANAGERMENT_H
#define CARMANAGERMENT_H

#include <QWidget>
class QToolBar;
class QTableWidget;

namespace Ui {
class CarManagermentWidget;
}

class CarManagermentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarManagermentWidget(QWidget *parent = 0);
    ~CarManagermentWidget();

private:
    /**
     * @brief 配置toolbar
     */
    void            configToolBar();

    Ui::CarManagermentWidget *ui;
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

#endif // CARMANAGERMENT_H
