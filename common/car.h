#ifndef CAR_H
#define CAR_H

#include <QObject>
#include <datatype.h>

class Car : public QObject
{
    Q_OBJECT
public:
    explicit Car(QObject *parent = 0);

    Car& operator=(const Car& c) {
        number = c.number;
        carNumber = c.carNumber;
        bankAccount = c.bankAccount;
        owner = c.owner;
        carBrand = c.carBrand;
        chassisBrand = c.chassisBrand;
        fuelCarNumber = c.fuelCarNumber;
        frameNumber = c.frameNumber;
        identificationNumber = c.identificationNumber;
        productNumber = c.productNumber;
        insuranceCardNumber = c.insuranceCardNumber;
        engineNumber = c.engineNumber;
        dimensions = c.dimensions;
        factoryCode = c.factoryCode;
        operator1 = c.operator1;
        operator2 = c.operator2;
        operator3 = c.operator3;
        operator4 = c.operator4;
        operator5 = c.operator5;
        remarks = c.remarks;
        creator = c.creator;

        productionDate = c.productionDate;
        drivingLicenseDate = c.drivingLicenseDate;
        createDate = c.createDate;

        pumpedSquare = c.pumpedSquare;
        pumpedTimes = c.pumpedTimes;
        milage = c.milage;
        worth = c.worth;
        enginePower = c.enginePower;
        maxDeliverySizes = c.maxDeliverySizes;
        maxOutputPressure = c.maxOutputPressure;
        boomVerticalLen = c.boomVerticalLen;
        boomHorizontalLen = c.boomHorizontalLen;
        totalWeight = c.totalWeight;
        equipmentWeight = c.equipmentWeight;
        pumptype = c.pumptype;

        for (int i = 0; i < c.insuranceRecords.size(); i++)
            insuranceRecords.push_back(c.insuranceRecords.at(i));

        for (int i = 0; i < c.businessInsuranceRecords.size(); i++)
            businessInsuranceRecords.push_back(c.businessInsuranceRecords.at(i));

        for (int i = 0; i < c.annualFeeRecords.size(); i++)
            annualFeeRecords.push_back(c.annualFeeRecords.at(i));

        for (int i = 0; i < c.projectRecords.size(); i++)
            projectRecords.push_back(c.projectRecords.at(i));

        return *this;
    }

    Car(const Car& c) {
        number = c.number;
        carNumber = c.carNumber;
        bankAccount = c.bankAccount;
        owner = c.owner;
        carBrand = c.carBrand;
        chassisBrand = c.chassisBrand;
        fuelCarNumber = c.fuelCarNumber;
        frameNumber = c.frameNumber;
        identificationNumber = c.identificationNumber;
        productNumber = c.productNumber;
        insuranceCardNumber = c.insuranceCardNumber;
        engineNumber = c.engineNumber;
        dimensions = c.dimensions;
        factoryCode = c.factoryCode;
        operator1 = c.operator1;
        operator2 = c.operator2;
        operator3 = c.operator3;
        operator4 = c.operator4;
        operator5 = c.operator5;
        remarks = c.remarks;
        creator = c.creator;

        productionDate = c.productionDate;
        drivingLicenseDate = c.drivingLicenseDate;
        createDate = c.createDate;

        pumpedSquare = c.pumpedSquare;
        pumpedTimes = c.pumpedTimes;
        milage = c.milage;
        worth = c.worth;
        enginePower = c.enginePower;
        maxDeliverySizes = c.maxDeliverySizes;
        maxOutputPressure = c.maxOutputPressure;
        boomVerticalLen = c.boomVerticalLen;
        boomHorizontalLen = c.boomHorizontalLen;
        totalWeight = c.totalWeight;
        equipmentWeight = c.equipmentWeight;
        pumptype = c.pumptype;

        for (int i = 0; i < c.insuranceRecords.size(); i++)
            insuranceRecords.push_back(c.insuranceRecords.at(i));

        for (int i = 0; i < c.businessInsuranceRecords.size(); i++)
            businessInsuranceRecords.push_back(c.businessInsuranceRecords.at(i));

        for (int i = 0; i < c.annualFeeRecords.size(); i++)
            annualFeeRecords.push_back(c.annualFeeRecords.at(i));

        for (int i = 0; i < c.projectRecords.size(); i++)
            projectRecords.push_back(c.projectRecords.at(i));
    }

    copy(const Car &src, Car* target) {
        target->number = src.number;
        target->carNumber = src.carNumber;
        target->bankAccount = src.bankAccount;
        target->owner = src.owner;
        target->carBrand = src.carBrand;
        target->chassisBrand = src.chassisBrand;
        target->fuelCarNumber = src.fuelCarNumber;
        target->frameNumber = src.frameNumber;
        target->identificationNumber = src.identificationNumber;
        target->productNumber = src.productNumber;
        target->insuranceCardNumber = src.insuranceCardNumber;
        target->engineNumber = src.engineNumber;
        target->dimensions = src.dimensions;
        target->factoryCode = src.factoryCode;
        target->operator1 = src.operator1;
        target->operator2 = src.operator2;
        target->operator3 = src.operator3;
        target->operator4 = src.operator4;
        target->operator5 = src.operator5;
        target->remarks = src.remarks;
        target->creator = src.creator;

        target->productionDate = src.productionDate;
        target->drivingLicenseDate = src.drivingLicenseDate;
        target->createDate = src.createDate;

        target->pumpedSquare = src.pumpedSquare;
        target->pumpedTimes = src.pumpedTimes;
        target->milage = src.milage;
        target->worth = src.worth;
        target->enginePower = src.enginePower;
        target->maxDeliverySizes = src.maxDeliverySizes;
        target->maxOutputPressure = src.maxOutputPressure;
        target->boomVerticalLen = src.boomVerticalLen;
        target->boomHorizontalLen = src.boomHorizontalLen;
        target->totalWeight = src.totalWeight;
        target->equipmentWeight = src.equipmentWeight;
        target->pumptype = src.pumptype;

        if (!target->insuranceRecords.isEmpty())
            target->insuranceRecords.clear();
        for (int i = 0; i < src.insuranceRecords.size(); i++)
            target->insuranceRecords.push_back(src.insuranceRecords.at(i));

        if (!target->businessInsuranceRecords.isEmpty())
            target->businessInsuranceRecords.clear();
        for (int i = 0; i < src.businessInsuranceRecords.size(); i++)
            target->businessInsuranceRecords.push_back(src.businessInsuranceRecords.at(i));

        if (!target->annualFeeRecords.isEmpty())
            target->annualFeeRecords.clear();
        for (int i = 0; i < src.annualFeeRecords.size(); i++)
            target->annualFeeRecords.push_back(src.annualFeeRecords.at(i));

        if (!target->projectRecords.isEmpty())
            target->projectRecords.clear();
        for (int i = 0; i < src.projectRecords.size(); i++)
            target->projectRecords.push_back(src.projectRecords.at(i));
    }

    /**
     * @brief getPumpTypeStr
     */
    QString         getPumpTypeStr(PumpType type);
    /**
     * @brief getPumpType
     */
    PumpType        getPumpType(QString typeStr);
    PumpType        getPumpType(int type);

    QString         number;
    QString         carNumber;
    QString         bankAccount;
    QString         owner;
    QString         carBrand;
    QString         chassisBrand;
    QString         fuelCarNumber;
    QString         frameNumber;
    QString         identificationNumber;
    QString         productNumber;
    QString         insuranceCardNumber;
    QString         engineNumber;
    QString         dimensions;
    QString         factoryCode;
    QString         operator1;
    QString         operator2;
    QString         operator3;
    QString         operator4;
    QString         operator5;
    QString         remarks;
    QString         creator;

    QDate           productionDate;
    QDate           drivingLicenseDate;
    QDate           createDate;

    float           pumpedSquare;
    float           pumpedTimes;
    float           milage;
    float           worth;
    float           enginePower;
    float           maxDeliverySizes;
    float           maxOutputPressure;
    float           boomVerticalLen;
    float           boomHorizontalLen;
    float           totalWeight;
    float           equipmentWeight;

    PumpType        pumptype;
    QList<INSURANCE_RECORD> insuranceRecords;
    QList<INSURANCE_RECORD> businessInsuranceRecords;
    QList<ANNUALFEE_RECORD> annualFeeRecords;
    QList<PROJECT_RECORD>   projectRecords;

signals:

public slots:

private:
};

#endif // CAR_H
