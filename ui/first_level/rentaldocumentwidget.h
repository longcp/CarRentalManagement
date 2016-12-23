#ifndef RENTALDOCUMENTWIDGET_H
#define RENTALDOCUMENTWIDGET_H

#include <QWidget>
class QToolBar;
class QTableWidget;

namespace Ui {
class RentalDocumentWidget;
}

class RentalDocumentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RentalDocumentWidget(QWidget *parent = 0);
    ~RentalDocumentWidget();

private:
    /**
     * @brief 配置工具栏
     */
    void            configToolBar();

    Ui::RentalDocumentWidget *ui;
    QToolBar        *mToolBar;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
    QAction         *mActExport;
    QAction         *mActImport;
};

#endif // RENTALDOCUMENTWIDGET_H
