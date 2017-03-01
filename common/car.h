#ifndef CAR_H
#define CAR_H

#include <QObject>
#include <datatype.h>

class Car : public QObject
{
    Q_OBJECT
public:
    explicit Car(QObject *parent = 0);

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
    QString         createDate;

    QList<INSURANCE_RECORD> insuranceRecords;
    QList<ANNUALFEE_RECORD> annualFeeRecords;
    QList<PROJECT_RECORD> projectRecords;
};

#endif // CAR_H
