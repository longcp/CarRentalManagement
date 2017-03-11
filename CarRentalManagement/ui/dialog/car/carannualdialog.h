#ifndef CARANNUALDIALOG_H
#define CARANNUALDIALOG_H

#include <QDialog>
#include <datatype.h>

class DataBase;

namespace Ui {
class CarAnnualDialog;
}

class CarAnnualDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarAnnualDialog(QWidget *parent = 0);
    ~CarAnnualDialog();

private slots:
    void        openWindow();

    void        on_ensureBtn_clicked();

    void        on_cancelBtn_clicked();

signals:
    void        addAnnualItemSignal(ANNUALFEE_RECORD &record);

private:
    void        closeEvent(QCloseEvent *event);
    void        clean();

    DataBase*   mDb;

    Ui::CarAnnualDialog *ui;
};

#endif // CARANNUALDIALOG_H
