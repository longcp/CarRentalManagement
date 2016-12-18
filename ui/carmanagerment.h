#ifndef CARMANAGERMENT_H
#define CARMANAGERMENT_H

#include <QWidget>
class QToolBar;
class QTableWidget;

namespace Ui {
class CarManagerment;
}

class CarManagerment : public QWidget
{
    Q_OBJECT

public:
    explicit CarManagerment(QWidget *parent = 0);
    ~CarManagerment();

private:
    Ui::CarManagerment *ui;
    QToolBar        *mToolBar;
    QTableWidget    *mCarWidget;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
};

#endif // CARMANAGERMENT_H
