#ifndef WARNMODEL_H
#define WARNMODEL_H

#include <QStandardItemModel>

/**
 * @brief The WarnModel class:委托，设置居中显示
 */
class WarnModel : public QStandardItemModel
{
    Q_OBJECT
public:
    WarnModel(QObject *parent=NULL) : QStandardItemModel(parent) { }
    WarnModel(int row, int column, QObject *parent=NULL)
        : QStandardItemModel(row, column, parent) { }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if( Qt::TextAlignmentRole == role )
            return Qt::AlignCenter;
        return QStandardItemModel::data(index, role);
    }
};

#endif // WARNMODEL_H
