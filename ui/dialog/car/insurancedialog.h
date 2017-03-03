#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <datatype.h>

class DataBase;

namespace Ui {
class InsuranceDialog;
}

class InsuranceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsuranceDialog(QWidget *parent = 0);
    ~InsuranceDialog();

private slots:
    void        openWindow();

    void on_ensureBtn_clicked();

    void on_cancelBtn_clicked();

signals:
    void        addInsuranceItemSignal(INSURANCE_RECORD &record);
    void        addBusinessInsuranceItemSignal(INSURANCE_RECORD &record);

private:
    void        closeEvent(QCloseEvent *);
    void        clean();

    Ui::InsuranceDialog *ui;
    DataBase*   mDb;
};

#endif // DIALOG_H
