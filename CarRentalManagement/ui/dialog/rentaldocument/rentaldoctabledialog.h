#ifndef RENTALDOCTABLEDIALOG_H
#define RENTALDOCTABLEDIALOG_H

#include <QDialog>

class TableModel;
class DataBase;
class RentalDocument;

namespace Ui {
class RentalDocTableDialog;
}

class RentalDocTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RentalDocTableDialog(QWidget *parent = 0);
    ~RentalDocTableDialog();

private slots:
    void openWindow();

    void on_rentablDocTableView_doubleClicked(const QModelIndex &index);

signals:
    void selectedDoc(QString number);

private:
    void initView();
    void initTableView();
    void addRentalDocItem(RentalDocument &doc);

    TableModel* mModel;
    DataBase*   mDb;
    Ui::RentalDocTableDialog *ui;
};

#endif // RENTALDOCTABLEDIALOG_H
