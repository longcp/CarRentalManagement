#ifndef RECEIPTEDITDIALOG_H
#define RECEIPTEDITDIALOG_H

#include <QDialog>

namespace Ui {
class ReceiptEditDialog;
}

class ReceiptEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiptEditDialog(QWidget *parent = 0);
    ~ReceiptEditDialog();

private slots:
    /**
     * @brief 打开本窗口
     */
    void                openReceiptEditDialogSlot();

private:
    Ui::ReceiptEditDialog *ui;
};

#endif // RECEIPTEDITDIALOG_H
