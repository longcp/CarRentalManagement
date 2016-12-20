#ifndef RENTALDOCUMENTWINDOW_H
#define RENTALDOCUMENTWINDOW_H

#include <QMainWindow>

class QTableWidget;

namespace Ui {
class RentalDocumentWindow;
}

class RentalDocumentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RentalDocumentWindow(QWidget *parent = 0);
    ~RentalDocumentWindow();

private:
    Ui::RentalDocumentWindow *ui;
    QToolBar        *mToolBar;
    QTableWidget    *mRentalDocWidget;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;
};

#endif // RENTALDOCUMENTWINDOW_H
