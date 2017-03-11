#ifndef RENTALDOCUMENTWIDGET_H
#define RENTALDOCUMENTWIDGET_H

#include <QWidget>
class QToolBar;
class QTableWidget;
class RentalDocumentEditDialog;
class TableModel;

namespace Ui {
class RentalDocumentWidget;
}

class RentalDocumentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RentalDocumentWidget(QWidget *parent = 0);
    ~RentalDocumentWidget();

private slots:
    void            cellDoubleClickedSlot(const QModelIndex &index);
    void            addRentalDocSlot();

signals:
    void            openRentalEditDialogSignal();

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    /**
     * @brief 初始化界面
     */
    void            initView();

    Ui::RentalDocumentWidget *ui;
    QToolBar        *mToolBar;
    RentalDocumentEditDialog    *mRentalDocEditDialog;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;

    TableModel*      mModel;
};

#endif // RENTALDOCUMENTWIDGET_H
