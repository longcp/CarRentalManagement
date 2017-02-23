#ifndef RENTALDOCUMENTEDITDIALOG_H
#define RENTALDOCUMENTEDITDIALOG_H

#include <QDialog>

class QToolBar;

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
    void            openRentalDocEditDialogSlot();

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();

    Ui::RentalDocumentEditDialog *ui;
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
};

#endif // RENTALDOCUMENTEDITDIALOG_H
