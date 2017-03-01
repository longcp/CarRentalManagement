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
        owner = c.owner;
        pumptype = c.pumptype;
        pumpedSquare = c.pumpedSquare;
        pumpedTimes = c.pumpedTimes;
        milage = c.milage;
        carBrand = c.carBrand;
        chassisBrand = c.chassisBrand;
        drivingLicenseDate = c.drivingLicenseDate;
        fuelCarNumber = c.fuelCarNumber;
        frameNumber = c.frameNumber;
        identificationNumber = c.identificationNumber;
        productNumber = c.productNumber;
        insuranceCardNumber = c.insuranceCardNumber;
        engineNumber = c.engineNumber;
        worth = c.worth;
        enginePower = c.enginePower;
        maxDeliverySizes = c.maxDeliverySizes;
        maxOutputPressure = c.maxOutputPressure;
        dimensions = c.dimensions;
        boomVerticalLen = c.boomVerticalLen;
        boomHorizontalLen = c.boomHorizontalLen;
        totalWeight = c.totalWeight;
        equipmentWeight = c.equipmentWeight;
        productionDate = c.productionDate;
        factoryCode = c.factoryCode;
        isBuyInsurance = c.isBuyInsurance;
        insuranceFee = c.insuranceFee;
        insuranceCompany = c.insuranceCompany;
        feeInsuranceDate = c.feeInsuranceDate;
        isBuyBusinessInsurance = c.isBuyBusinessInsurance;
        businessInsuranceFee = c.businessInsuranceFee;
        businessInsuranceCompany = c.businessInsuranceCompany;
        feeBusinessInsuranceDate = c.feeBusinessInsuranceDate;
        annualFee = c.annualFee;
        travelExpenses = c.travelExpenses;
        annualReviewDate = c.annualReviewDate;
        operator1 = c.operator1;
        operator2 = c.operator2;
        operator3 = c.operator3;
        operator4 = c.operator4;
        operator5 = c.operator5;
        remarks = c.remarks;
        creator = c.creator;
        createDate = c.createDate;

        for (int i = 0; i < c.insuranceRecords.size(); i++)
            insuranceRecords.push_back(c.insuranceRecords.at(i));

        for (int i = 0; i < c.annualFeeRecords.size(); i++)
            annualFeeRecords.push_back(c.annualFeeRecords.at(i));

        for (int i = 0; i < c.projectRecords.size(); i++)
            projectRecords.push_back(c.projectRecords.at(i));

        return *this;
    }

    Car(const Car& c) {
        number = c.number;
        owner = c.owner;
        pumptype = c.pumptype;
        pumpedSquare = c.pumpedSquare;
        pumpedTimes = c.pumpedTimes;
        milage = c.milage;
        carBrand = c.carBrand;
        chassisBrand = c.chassisBrand;
        drivingLicenseDate = c.drivingLicenseDate;
        fuelCarNumber = c.fuelCarNumber;
        frameNumber = c.frameNumber;
        identificationNumber = c.identificationNumber;
        productNumber = c.productNumber;
        insuranceCardNumber = c.insuranceCardNumber;
        engineNumber = c.engineNumber;
        worth = c.worth;
        enginePower = c.enginePower;
        maxDeliverySizes = c.maxDeliverySizes;
        maxOutputPressure = c.maxOutputPressure;
        dimensions = c.dimensions;
        boomVerticalLen = c.boomVerticalLen;
        boomHorizontalLen = c.boomHorizontalLen;
        totalWeight = c.totalWeight;
        equipmentWeight = c.equipmentWeight;
        productionDate = c.productionDate;
        factoryCode = c.factoryCode;
        isBuyInsurance = c.isBuyInsurance;
        insuranceFee = c.insuranceFee;
        insuranceCompany = c.insuranceCompany;
        feeInsuranceDate = c.feeInsuranceDate;
        isBuyBusinessInsurance = c.isBuyBusinessInsurance;
        businessInsuranceFee = c.businessInsuranceFee;
        businessInsuranceCompany = c.businessInsuranceCompany;
        feeBusinessInsuranceDate = c.feeBusinessInsuranceDate;
        annualFee = c.annualFee;
        travelExpenses = c.travelExpenses;
        annualReviewDate = c.annualReviewDate;
        operator1 = c.operator1;
        operator2 = c.operator2;
        operator3 = c.operator3;
        operator4 = c.operator4;
        operator5 = c.operator5;
        remarks = c.remarks;
        creator = c.creator;
        createDate = c.createDate;

        for (int i = 0; i < c.insuranceRecords.size(); i++)
            insuranceRecords.push_back(c.insuranceRecords.at(i));

        for (int i = 0; i < c.annualFeeRecords.size(); i++)
            annualFeeRecords.push_back(c.annualFeeRecords.at(i));

        for (int i = 0; i < c.projectRecords.size(); i++)
            projectRecords.push_back(c.projectRecords.at(i));
    }

signals:

public slots:

private:
    QString         number;
    QString         owner;
    PumpType        pumptype;
    float           pumpedSquare;
    float           pumpedTimes;
    float           milage;
    QString         carBrand;
    QString         chassisBrand;
    QDate           drivingLicenseDate;
    QString         fuelCarNumber;
    QString         frameNumber;
    QString         identificationNumber;
    QString         productNumber;
    QString         insuranceCardNumber;
    QString         engineNumber;
    float           worth;
    float           enginePower;
    float           maxDeliverySizes;
    float           maxOutputPressure;
    QString         dimensions;
    float           boomVerticalLen;
    float           boomHorizontalLen;
    float           totalWeight;
    float           equipmentWeight;
    QDate           productionDate;
    QString         factoryCode;
    bool            isBuyInsurance;
    float           insuranceFee;
    QString         insuranceCompany;
    QDate           feeInsuranceDate;
    bool            isBuyBusinessInsurance;
    float           businessInsuranceFee;
    QString         businessInsuranceCompany;
    QDate           feeBusinessInsuranceDate;
    float           annualFee;
    float           travelExpenses;
    QDate           annualReviewDate;
    QString         operator1;
    QString         operator2;
    QString         operator3;
    QString         operator4;
    QString         operator5;
    QString         remarks;
    QString         creator;
    QDate           createDate;

    QList<INSURANCE_RECORD> insuranceRecords;
    QList<ANNUALFEE_RECORD> annualFeeRecords;
    QList<PROJECT_RECORD> projectRecords;
};

#endif // CAR_H
