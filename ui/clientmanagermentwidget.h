#ifndef CLIENTMANAGERMENTWIDGET_H
#define CLIENTMANAGERMENTWIDGET_H

#include <QWidget>

class QToolBar;
class QTableWidget;

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

private:
    Ui::ClientManagermentWidget *ui;
    QToolBar        *mToolBar;
    QTableWidget    *mClientWidget;

    // 工具栏动作
    QAction         *mActAdd;
    QAction         *mActDelete;
    QAction         *mActEdit;
    QAction         *mActSearch;
    QAction         *mActPrinter;
};

#endif // CLIENTMANAGERMENTWIDGET_H
