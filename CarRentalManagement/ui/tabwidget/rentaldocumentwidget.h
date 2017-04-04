#ifndef RENTALDOCUMENTWIDGET_H
#define RENTALDOCUMENTWIDGET_H

#include <QWidget>
#include <datatype.h>

class QToolBar;
class QTableWidget;
class RentalDocumentEditDialog;
class TableModel;
class RentalDocument;

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
    void            addRentalDoc(RentalDocument &doc);
    void            updateDocItemSlot(RentalDocument &doc);
    void            on_docTableview_clicked(const QModelIndex &index);

signals:
    void            openRentalEditDialogSignal(OpenType, RentalDocument &,
                                               QString clientName, QString clientNumber);

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    /**
     * @brief 初始化界面
     */
    void            initView();
    void            addRentalDocRows(QList<RentalDocument> &docs);
    void            addRentalDocTableRow(RentalDocument &doc);

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

    int              mCurRow;
    TableModel*      mModel;
};

#endif // RENTALDOCUMENTWIDGET_H
