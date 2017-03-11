#ifndef CONTRACT_H
#define CONTRACT_H

#include <QObject>
#include <QDate>
#include <datatype.h>

class QDate;

class Contract : public QObject
{
    Q_OBJECT
public:
    explicit Contract(QObject *parent = 0);

    Contract& operator=(const Contract& c) {
        number = c.number;
        clientName = c.clientName;
        clientNumber = c.clientNumber;
        projectName = c.projectName;
        projectAddress = c.projectAddress;
        requireMent = c.requireMent;
        supplement = c.supplement;
        remarks = c.remarks;
        creator = c.creator;

        deliverySizes = c.deliverySizes;
        structureLevel = c.structureLevel;
        taxRate = c.taxRate;

        signedDate = c.signedDate;
        beginDate = c.beginDate;
        endDate = c.endDate;
        creatDate = c.creatDate;

        isIncludeTax = c.isIncludeTax;

        for (int i = 0; i < c.prices.size(); i++)
           prices.push_back(c.prices.at(i));
    }

    Contract(const Contract& c) {
        number = c.number;
        clientName = c.clientName;
        clientNumber = c.clientNumber;
        projectName = c.projectName;
        projectAddress = c.projectAddress;
        requireMent = c.requireMent;
        supplement = c.supplement;
        remarks = c.remarks;
        creator = c.creator;

        deliverySizes = c.deliverySizes;
        structureLevel = c.structureLevel;
        taxRate = c.taxRate;

        signedDate = c.signedDate;
        beginDate = c.beginDate;
        endDate = c.endDate;
        creatDate = c.creatDate;

        isIncludeTax = c.isIncludeTax;

        for (int i = 0; i < c.prices.size(); i++)
           prices.push_back(c.prices.at(i));
    }

    void copy(const Contract &src, Contract* target) {
        target->number = src.number;
        target->clientName = src.clientName;
        target->clientNumber = src.clientNumber;
        target->projectName = src.projectName;
        target->projectAddress = src.projectAddress;
        target->requireMent = src.requireMent;
        target->supplement = src.supplement;
        target->remarks = src.remarks;
        target->creator = src.creator;

        target->deliverySizes = src.deliverySizes;
        target->structureLevel = src.structureLevel;
        target->taxRate = src.taxRate;

        target->signedDate = src.signedDate;
        target->beginDate = src.beginDate;
        target->endDate = src.endDate;
        target->creatDate = src.creatDate;

        target->isIncludeTax = src.isIncludeTax;

        if (!target->prices.isEmpty())
            target->prices.clear();
        for (int i = 0; i < src.prices.size(); i++)
            target->prices.push_back(src.prices.at(i));
    }

    void            dump();

    QString number;
    QString clientName;
    QString clientNumber;
    QString projectName;
    QString projectAddress;
    QString requireMent;
    QString supplement;
    QString remarks;
    QString creator;

    float   deliverySizes;
    float   structureLevel;
    float   taxRate;

    QDate   signedDate;
    QDate   beginDate;
    QDate   endDate;
    QDate   creatDate;

    bool    isIncludeTax;
    QList<CONTRACT_PRICE> prices;

signals:

public slots:
};

#endif // CONTRACT_H
