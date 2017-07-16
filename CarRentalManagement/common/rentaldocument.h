#ifndef RENTALDOCUMENT_H
#define RENTALDOCUMENT_H

#include <QObject>
#include <datatype.h>

class QDate;
class QTime;
class QDateTime;

class RentalDocument
{
public:
    RentalDocument();

    RentalDocument& operator=(const RentalDocument& r) {
        number = r.number;
        clientName = r.clientName;
        clientNumber = r.clientNumber;
        contractNumber = r.contractNumber;
        carNumber = r.carNumber;
        carPlateNumber = r.carPlateNumber;
        constructPlace = r.constructPlace;
        concreteLable = r.concreteLable;
        principal = r.principal;
        principalTel = r.principalTel;
        driver1 = r.driver1;
        driver2 = r.driver2;
        driver3 = r.driver3;
        projectName = r.projectName;
        projectAddress = r.projectAddress;
        remarks = r.remarks;

        beginFuel = r.beginFuel;
        endFuel = r.endFuel;
        projectAmount = r.projectAmount;
        pumpSquare = r.pumpSquare;
        squareUnitPrice = r.squareUnitPrice;
        pumpTimes = r.pumpTimes;
        pumpTimeUnitPrice = r.pumpTimeUnitPrice;
        receivedAccounts = r.receivedAccounts;

        date = r.date;
        workingHours = r.workingHours;
        arrivalDateTime = r.arrivalDateTime;
        leaveDateTime = r.leaveDateTime;

        rentalDocState = r.rentalDocState;
        pumpType = r.pumpType;
    }

    RentalDocument(const RentalDocument& r) {
        number = r.number;
        clientName = r.clientName;
        clientNumber = r.clientNumber;
        contractNumber = r.contractNumber;
        carNumber = r.carNumber;
        carPlateNumber = r.carPlateNumber;
        constructPlace = r.constructPlace;
        concreteLable = r.concreteLable;
        principal = r.principal;
        principalTel = r.principalTel;
        driver1 = r.driver1;
        driver2 = r.driver2;
        driver3 = r.driver3;
        projectName = r.projectName;
        projectAddress = r.projectAddress;
        remarks = r.remarks;

        beginFuel = r.beginFuel;
        endFuel = r.endFuel;
        projectAmount = r.projectAmount;
        pumpSquare = r.pumpSquare;
        squareUnitPrice = r.squareUnitPrice;
        pumpTimes = r.pumpTimes;
        pumpTimeUnitPrice = r.pumpTimeUnitPrice;
        receivedAccounts = r.receivedAccounts;

        date = r.date;
        workingHours = r.workingHours;
        arrivalDateTime = r.arrivalDateTime;
        leaveDateTime = r.leaveDateTime;

        rentalDocState = r.rentalDocState;
        pumpType = r.pumpType;
    }

    void copy(const RentalDocument& r) {
        number = r.number;
        clientName = r.clientName;
        clientNumber = r.clientNumber;
        contractNumber = r.contractNumber;
        carNumber = r.carNumber;
        carPlateNumber = r.carPlateNumber;
        constructPlace = r.constructPlace;
        concreteLable = r.concreteLable;
        principal = r.principal;
        principalTel = r.principalTel;
        driver1 = r.driver1;
        driver2 = r.driver2;
        driver3 = r.driver3;
        projectName = r.projectName;
        projectAddress = r.projectAddress;
        remarks = r.remarks;

        beginFuel = r.beginFuel;
        endFuel = r.endFuel;
        projectAmount = r.projectAmount;
        pumpSquare = r.pumpSquare;
        squareUnitPrice = r.squareUnitPrice;
        pumpTimes = r.pumpTimes;
        pumpTimeUnitPrice = r.pumpTimeUnitPrice;
        receivedAccounts = r.receivedAccounts;

        date = r.date;
        workingHours = r.workingHours;
        arrivalDateTime = r.arrivalDateTime;
        leaveDateTime = r.leaveDateTime;

        rentalDocState = r.rentalDocState;
        pumpType = r.pumpType;
    }

    void    dump();
    bool    isValueEqual(RentalDocument &r);
    const static QString getRentalDocStatStr(RentalDocState stat);
    const static RentalDocState getRentalDocStat(QString stat);

    QString number;
    QString clientName;
    QString clientNumber;
    QString contractNumber;
    QString carNumber;
    QString carPlateNumber;
    QString constructPlace;
    QString concreteLable;
    QString principal;
    QString principalTel;
    QString driver1;
    QString driver2;
    QString driver3;
    QString projectName;
    QString projectAddress;
    QString remarks;

    float beginFuel;
    float endFuel;
    float projectAmount;
    float pumpSquare;
    float squareUnitPrice;
    float pumpTimes;
    float pumpTimeUnitPrice;
    float workingHours;
    float receivedAccounts;

    QDate date;
    QDateTime arrivalDateTime;
    QDateTime leaveDateTime;

    RentalDocState rentalDocState;
    PumpType pumpType;
};

#endif // RENTALDOCUMENT_H
