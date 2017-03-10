#ifndef CONTRACT_H
#define CONTRACT_H

#include <QObject>
#include <QDate>

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

signals:

public slots:
};

#endif // CONTRACT_H
