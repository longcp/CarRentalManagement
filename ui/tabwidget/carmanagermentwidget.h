#ifndef CARMANAGERMENT_H
#define CARMANAGERMENT_H

#include <QWidget>
#include <datatype.h>

class QToolBar;
class QTableWidget;
class CarEditDialog;
class TableModel;
class Car;

namespace Ui {
class CarManagermentWidget;
}

class CarManagermentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarManagermentWidget(QWidget *parent = 0);
    ~CarManagermentWidget();

private slots:
    void            cellDoubleClickedSlot(const QModelIndex &index);
    void            addCarSlot();

signals:
    void            openCarEditDialogSignal(OpenType type,
                                            Car &car);

private:
    /**
     * @brief 配置toolbar
     */
    void            configToolBar();
    /**
     * @brief 初始化界面
     */
    void            initView();

    Ui::CarManagermentWidget *ui;
    QToolBar        *mToolBar;

    // 编辑车辆档案窗口
    CarEditDialog    *mCarEditDialog;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;

    TableModel*     mModel;
};

#endif // CARMANAGERMENT_H
