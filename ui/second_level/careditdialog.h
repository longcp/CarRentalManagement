#ifndef CAREDITDIALOG_H
#define CAREDITDIALOG_H

#include <QDialog>
#include <datatype.h>

class QToolBar;
class Car;
class TableModel;

namespace Ui {
class CarEditDialog;
}

class CarEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarEditDialog(QWidget *parent = 0);
    ~CarEditDialog();

private slots:
    /**
     * @brief 打开窗口
     */
    void            openCarEditDialogSlot(OpenType type,
                                          Car &car);

private:
    void            initView();
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    /**
     * @brief initProjectTableView
     */
    void            initProjectTableview();
    /**
     * @brief initAnnualTableview
     */
    void            initAnnualTableview();
    /**
     * @brief initPaymentTableview
     */
    void            initPaymentTableview();
    /**
     * @brief initBusinessTableview
     */
    void            initBusinessTableview();

    Ui::CarEditDialog *ui;
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

    TableModel*     mProjectModel;
    TableModel*     mAnnualModel;
    TableModel*     mBusinessModel;
    TableModel*     mPaymentModel;
};

#endif // CAREDITDIALOG_H
