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
    void            updateProjectSumSectionWidth(int logicalIndex,
                                                int /* oldSize */,
                                                int newSize);
    void            updateAnnualSumSectionWidth(int logicalIndex,
                                                int /* oldSize */,
                                                int newSize);
    void            updateBusinessSumSectionWidth(int logicalIndex,
                                                int /* oldSize */,
                                                int newSize);
    void            updatePaymentSumSectionWidth(int logicalIndex,
                                                int /* oldSize */,
                                                int newSize);

    void            updateAnnualTableviewScrollBar(int to);

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
    /**
     * @brief initAnnualSumTableview
     */
    void            initAnnualSumTableview();
    /**
     * @brief initBusinessSumTableview
     */
    void            initBusinessSumTableview();
    /**
     * @brief initPaymentSumTableview
     */
    void            initPaymentSumTableview();
    /**
     * @brief initProjectSumTableview
     */
    void            initProjectSumTableview();
    /**
     * @brief 设置编辑模式
     */
    void            setEditMode();
    /**
     * @brief 设置查看模式
     */
    void            setViewMode();
    /**
     * @brief 设置模式 编辑/查看
     */
    void            setMode(bool mode);

    void            setPumpTypeView();
    /**
     * @brief setOriginCar
     */
    void            setOriginCar(Car &car);
    /**
     * @brief setView
     */
    void            setView(Car &car);
    /**
     * @brief getPumpTypePosition
     */
    int             getPumpTypePosition(PumpType type);

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
    TableModel*     mProjectSumModel;
    TableModel*     mAnnualModel;
    TableModel*     mAnnualSumModel;
    TableModel*     mBusinessModel;
    TableModel*     mBusinessSumModel;
    TableModel*     mPaymentModel;
    TableModel*     mPaymentSumModel;

    OpenType        mOpenType;
    Car*            mOriginCar;
};

#endif // CAREDITDIALOG_H
