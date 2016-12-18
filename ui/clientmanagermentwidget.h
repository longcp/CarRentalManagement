#ifndef CLIENTMANAGERMENTWIDGET_H
#define CLIENTMANAGERMENTWIDGET_H

#include <QWidget>

class QToolBar;

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
    /**
     * @brief 打开窗口
     */
    void            openClientWidgetSlot();

private:
    Ui::ClientManagermentWidget *ui;
    QToolBar        *mToolBar;
};

#endif // CLIENTMANAGERMENTWIDGET_H
