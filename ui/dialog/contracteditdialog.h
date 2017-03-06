#ifndef CONTRACTEDITDIALOG_H
#define CONTRACTEDITDIALOG_H

#include <QDialog>

namespace Ui {
class ContractEditDialog;
}

class ContractEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContractEditDialog(QWidget *parent = 0);
    ~ContractEditDialog();

private:
    Ui::ContractEditDialog *ui;
};

#endif // CONTRACTEDITDIALOG_H
