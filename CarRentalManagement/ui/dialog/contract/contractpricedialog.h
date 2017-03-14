#ifndef CONTRACTPRICEDIALOG_H
#define CONTRACTPRICEDIALOG_H

#include <QDialog>
#include <datatype.h>

class DataBase;

namespace Ui {
class ContractPriceDialog;
}

class ContractPriceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContractPriceDialog(QWidget *parent = 0);
    ~ContractPriceDialog();

private slots:
    void            openWindow(OpenType type, QString contractNumber);

    void            on_ensureBtn_clicked();

    void            on_cancelBtn_clicked();

signals:
    void            addPriceItemSignal(CONTRACT_PRICE &);

private:
    /**
     * @brief 初始化泵式
     */
    void            setPumpTypeView();
    /**
     * @brief 清理界面
     */
    void            clean();
    /**
     * @brief 关闭窗口
     */
    void            closeEvent(QCloseEvent *event);

    DataBase*       mDb;
    QString         mContractNumber;
    OpenType        mOpenType;
    Ui::ContractPriceDialog *ui;
};

#endif // CONTRACTPRICEDIALOG_H
