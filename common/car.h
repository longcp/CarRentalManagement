#ifndef CAR_H
#define CAR_H

#include <QObject>

#include <QDate>

class Car : public QObject
{
    Q_OBJECT
public:
    explicit Car(QObject *parent = 0);
    enum PumpType {TYPE_37M1,
                   TYPE_48M,
                   TYPE_52M,
                   TYPE_56M,
                   TYPE_60M,
                   TYPE_CAR_PUMP};

    class InsuranceRecord {
    public:
        InsuranceRecord(QDate date,
                        float fee,
                        QString company,
                        QString remarks) {
            this->date = date;
            this->fee = fee;
            this->company = company;
            this->remarks = remarks;
        }

        QDate date;
        float fee;
        QString company;
        QString remarks;
    };

    class AnnualFeeRecord {
    public:
        AnnualFeeRecord(QDate date,
                        float annualFee,
                        float travelExpenses,
                        QString remarks) {
            this->date = date;
            this->annualFee = annualFee;
            this->travelExpenses = travelExpenses;
            this->remarks = remarks;
        }

        QDate date;
        float annualFee;
        float travelExpenses;
        QString remarks;
    };

    class ProjectRecord {
    public:
        ProjectRecord(QDate date,
                      QString clientNum,
                      QString clientName,
                      float amount,
                      QString remarks,
                      QString number) {
            this->date = date;
            this->clientNum = clientNum;
            this->clientName = clientName;
            this->amount = amount;
            this->remarks = remarks;
            this->number = number;
        }

        QDate date;
        QString clientNum;
        QString clientName;
        float amount;
        QString remarks;
        QString number;
    };

signals:

public slots:

private:
    QString         number;
    QString         owner;
    PumpType        pumptype;
    float           pumpedSquare;
    float           pumpedTime;
    float           milage;
    QString         carBrand;
    QString         chassisBrand;
    QDate           DrivingLicenseDate;
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
};

#endif // CAR_H
