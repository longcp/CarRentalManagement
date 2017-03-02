#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

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

private:
    Ui::InsuranceDialog *ui;
};

#endif // DIALOG_H
