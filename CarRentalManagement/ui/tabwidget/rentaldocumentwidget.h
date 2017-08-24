#ifndef RENTALDOCUMENTWIDGET_H
#define RENTALDOCUMENTWIDGET_H

#include <QWidget>
#include <datatype.h>

class QToolBar;
class QTableWidget;
class RentalDocumentEditDialog;
class TableModel;
class RentalDocument;
class QTreeWidgetItem;
class DataBase;
class User;

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
    void            editRentalDocItemSlot();
    void            deleteRentalDocItemSlot();

    void            on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void            on_reservationRadio_toggled(bool checked);
    void            on_unconfirmedRadio_toggled(bool checked);
    void            on_confirmedRadio_toggled(bool checked);
    void            on_totalRadio_toggled(bool checked);

    void            initViewWithUser(User &user);

signals:
    void            openRentalEditDialogSignal(OpenType, RentalDocument &,
                                               QString clientName, QString clientNumber);
    void            deleteRentalDocumentSignal(QString  docNum);
    void            addRentalDocumentSignal(RentalDocument &doc);
    void            initViewWithUserSig(User &user);

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();
    /**
     * @brief 初始化界面
     */
    void            initView();
    void            initRentalDocTableView();
    void            initClientTreeWidget();
    void            updateTableView();
    void            addAllClientItem();
    void            addRentalDocRows(QList<RentalDocument> &docs);
    void            addRentalDocTableRow(RentalDocument &doc);
    void            clearRentalDocTable();
    void            editRowEvent(int row);

    Ui::RentalDocumentWidget *ui;
    QToolBar        *mToolBar;
    RentalDocumentEditDialog    *mRentalDocEditDialog;
    RentalDocState  mCurDocState;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;

    DataBase*       mDb;
    int             mCurRow;
    TableModel*     mModel;
    QTreeWidgetItem*    mRootItem;
    QString         mCurClientNumber;
    bool            mCurItemIsParentItem;
    const static int mClientNameColumn = 0;
    const static int mClientNumberColumn = 1;
};

#endif // RENTALDOCUMENTWIDGET_H
