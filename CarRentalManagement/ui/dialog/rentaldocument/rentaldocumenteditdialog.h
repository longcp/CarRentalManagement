#ifndef RENTALDOCUMENTEDITDIALOG_H
#define RENTALDOCUMENTEDITDIALOG_H

#include <QDialog>
#include <datatype.h>

class QToolBar;
class DataBase;
class RentalDocument;
class CarTableDialog;
class ContractTableDialog;
class ClientTableDialog;
class PriceTableDialog;

namespace Ui {
class RentalDocumentEditDialog;
}

class RentalDocumentEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RentalDocumentEditDialog(QWidget *parent = 0);
    ~RentalDocumentEditDialog();

private slots:
    /**
     * @brief 打开本窗口
     */
    void            openWindow(OpenType type, RentalDocument &doc, QString clientName, QString clientNumber);
    /**
     * @brief 工具栏编辑按钮
     */
    void            editEvent();
    /**
     * @brief 工具栏保存退出按钮
     */
    void            saveAndExitEvent();
    /**
     * @brief 工具栏保存按钮
     */
    void            saveEvent();
    /**
     * @brief 工具栏取消按钮
     */
    void            cancelEvent();
    /**
     * @brief 工具栏退出按钮
     */
    void            closeDialog();
    /**
     * @brief 选择car
     */
    void            getCar(QString number);
    /**
     * @brief 选择contract
     */
    void            getContract(QString number);

    void            getPrice(CONTRACT_PRICE price);

    void            on_contractNumToolButton_clicked();

    void            on_carNumToolButton_clicked();

    void on_pickPriceBtn_clicked();

signals:
    void            addRentalDocSignal(RentalDocument &doc);
    void            updateDocItemSignal(RentalDocument &doc);
    void            openCarTableDialogSignal();
    void            openContractTableDialogSignal(QString clientNumber);
    void            openPriceWindow(QString contractNumber);

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    QString         makeRentalDocNumber();
    void            initView();
    void            setPumpTypeView();
    int             getPumpTypePosition(PumpType type);
    void            setEditMode();
    void            setEditModePre();
    void            setEditModeSupplement();
    void            setViewMode();
    void            setViewModePre();
    void            setViewModeSupplement();
    void            setMode(bool mode);
    void            setOriginRentalDocument(RentalDocument &doc);
    void            setView(RentalDocument &doc);
    bool            isModified();
    void            saveUiContent(RentalDocument &doc);
    void            resetView(RentalDocument &doc);
    void            resetView();
    void            closeEvent(QCloseEvent *);
    void            clean();
    void            cleanContent();
    void            setWindowSize();

    Ui::RentalDocumentEditDialog *ui;
    // 工具栏
    QToolBar*       mToolBar;
    OpenType        mOpenType;
    DataBase*       mDb;
    RentalDocument* mRentalDocument;
    QString         mCarNumber;
    QString         mClientNumber;
    bool            isSetWindowSize;
    CarTableDialog* mCarTableDialog;
    ContractTableDialog* mContractTableDialog;
    PriceTableDialog*    mPriceTableDialog;

    // 工具栏动作
    QAction*        mActAdd;
    QAction*        mActSaveExit;
    QAction*        mActSave;
    QAction*        mActExit;
    QAction*        mActEdit;
    QAction*        mActPrev;
    QAction*        mActNext;
    QAction*        mActCancel;
};

#endif // RENTALDOCUMENTEDITDIALOG_H
