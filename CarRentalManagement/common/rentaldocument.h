#ifndef RENTALDOCUMENT_H
#define RENTALDOCUMENT_H

#include <QObject>
#include <datatype.h>

class Date;
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
        contructPlace = r.contructPlace;
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

        date = r.date;
        workTime = r.workTime;
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
        contructPlace = r.contructPlace;
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

        date = r.date;
        workTime = r.workTime;
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
        contructPlace = r.contructPlace;
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

        date = r.date;
        workTime = r.workTime;
        arrivalDateTime = r.arrivalDateTime;
        leaveDateTime = r.leaveDateTime;

        rentalDocState = r.rentalDocState;
        pumpType = r.pumpType;
    }

    void    dump();

    QString number;
    QString clientName;
    QString clientNumber;
    QString contractNumber;
    QString carNumber;
    QString carPlateNumber;
    QString contructPlace;
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

    Date date;
    QTime workTime;
    QDateTime arrivalDateTime;
    QDateTime leaveDateTime;

    RentalDocState rentalDocState;
    PumpType pumpType;
};

#endif // RENTALDOCUMENT_H
