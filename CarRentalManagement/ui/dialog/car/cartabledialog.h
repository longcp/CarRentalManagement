#ifndef CARTABLEDIALOG_H
#define CARTABLEDIALOG_H

#include <QDialog>

class Car;
class TableModel;
class DataBase;

namespace Ui {
class CarTableDialog;
}

class CarTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarTableDialog(QWidget *parent = 0);
    ~CarTableDialog();

private slots:
    void            openWindow();

    void            on_carTableview_doubleClicked(const QModelIndex &index);

signals:
    void            selectedCar(QString number);

private:
    /**
     * @brief 初始化界面
     */
    void            initView();
    /**
     * @brief 初始化表，插入已有数据
     */
    void            initTableView();
    /**
     * @brief 添加条目
     */
    void            addCarItem(Car &car);

    TableModel*     mModel;
    DataBase*       mDb;
    Ui::CarTableDialog *ui;
};

#endif // CARTABLEDIALOG_H
