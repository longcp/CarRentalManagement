#ifndef CLIENTMANAGERMENTWIDGET_H
#define CLIENTMANAGERMENTWIDGET_H

#include <QWidget>

class QToolBar;
class QTableWidget;
class ClientEditDialog;

namespace Ui {
class ClientManagermentWidget;
}

class ClientManagermentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientManagermentWidget(QWidget *parent = 0);
    ~ClientManagermentWidget();

private slots:
    void            cellDoubleClickedSlot(int a,int b);

signals:
    void            openClientEditDialogSignal();

private:
    Ui::ClientManagermentWidget *ui;
    QToolBar        *mToolBar;
    QTableWidget    *mClientWidget;
    // 编辑客户资料窗口
    ClientEditDialog    *mClientEditDialog;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;
};

#endif // CLIENTMANAGERMENTWIDGET_H
