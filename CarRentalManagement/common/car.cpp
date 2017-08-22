#include "car.h"
#include "util.h"

#define LOG_TAG                 "CAR"
#include "utils/Log.h"

Car::Car(QObject *parent) : QObject(parent)
{

}

const QString
Car::getPumpTypeStr(PumpType type)
{
    switch (type) {
    case TYPE_37M1:
        return PUMP_TYPE_37M1_STR;

    case TYPE_48M:
        return PUMP_TYPE_48M_STR;

    case TYPE_52M:
        return PUMP_TYPE_52M_STR;

    case TYPE_56M:
        return PUMP_TYPE_56M_STR;

    case TYPE_60M:
        return PUMP_TYPE_60M_STR;

    case TYPE_CAR_PUMP:
        return PUMP_TYPE_CAR_PUMP_STR;

    default:
        break;
    }

    return PUMP_TYPE_UNKNOWN_STR;
}


const PumpType
Car::getPumpType(QString typeStr)
{
    if (typeStr == PUMP_TYPE_37M1_STR)
        return TYPE_37M1;

    else if (typeStr == PUMP_TYPE_48M_STR)
        return TYPE_48M;

    else if (typeStr == PUMP_TYPE_52M_STR)
        return TYPE_52M;

    else if (typeStr == PUMP_TYPE_56M_STR)
        return TYPE_56M;

    else if (typeStr == PUMP_TYPE_60M_STR)
        return TYPE_60M;

    else if (typeStr == PUMP_TYPE_CAR_PUMP_STR)
        return TYPE_CAR_PUMP;

    else
        return UNKNOWN_PUMPTYPE;
}

const int
Car::getPumpTypePosition(PumpType type)
{

    switch (type) {
    case TYPE_37M1:
       return PUMP_TYPE_37M1_COLUMN;

    case TYPE_48M:
       return PUMP_TYPE_48M_COLUMN;

    case TYPE_52M:
       return PUMP_TYPE_52M_COLUMN;

    case TYPE_56M:
       return PUMP_TYPE_56M_COLUMN;

    case TYPE_60M:
       return PUMP_TYPE_60M_COLUMN;

    case TYPE_CAR_PUMP:
       return PUMP_TYPE_CAR_PUMP_COLUMN;

    default:
        break;
    }

    return PUMP_TYPE_37M1_COLUMN;
}

const PumpType
Car::getPumpType(int type)
{
    switch (type) {
    case 0:
        return TYPE_37M1;

    case 1:
        return TYPE_48M;

    case 2:
        return TYPE_52M;

    case 3:
        return TYPE_56M;

    case 4:
        return TYPE_60M;

    case 5:
        return TYPE_CAR_PUMP;

    default:
        break;
    }

    return UNKNOWN_PUMPTYPE;
}

void
Car::dump()
{
    ALOGD("number = %s", number.toStdString().data());
    ALOGD("owner = %s", owner.toStdString().data());
    ALOGD("bankAccount = %s", bankAccount.toStdString().data());
    ALOGD("carNumber = %s", carNumber.toStdString().data());
    ALOGD("carBrand = %s", carBrand.toStdString().data());
    ALOGD("chassisBrand = %s", chassisBrand.toStdString().data());
    ALOGD("fuelCarNumber = %s", fuelCarNumber.toStdString().data());
    ALOGD("frameNumber = %s", frameNumber.toStdString().data());
    ALOGD("identificationNumber = %s", identificationNumber.toStdString().data());
    ALOGD("engineNumber = %s", engineNumber.toStdString().data());
    ALOGD("insuranceCardNumber = %s", insuranceCardNumber.toStdString().data());
    ALOGD("factoryCode = %s", factoryCode.toStdString().data());
    ALOGD("productNumber = %s", productNumber.toStdString().data());
    ALOGD("dimensions = %s", dimensions.toStdString().data());
    ALOGD("operator1 = %s", operator1.toStdString().data());
    ALOGD("operator2 = %s", operator2.toStdString().data());
    ALOGD("operator3 = %s", operator3.toStdString().data());
    ALOGD("operator4 = %s", operator4.toStdString().data());
    ALOGD("operator5 = %s", operator5.toStdString().data());
    ALOGD("remarks = %s", remarks.toStdString().data());
    ALOGD("creator = %s", creator.toStdString().data());
    ALOGD("pumpedSquare = %lf", pumpedSquare);
    ALOGD("pumpedTimes = %lf", pumpedTimes);
    ALOGD("milage = %lf", milage);
    ALOGD("worth = %lf", worth);
    ALOGD("enginePower = %lf", enginePower);
    ALOGD("maxDeliverySizes = %lf", maxDeliverySizes);
    ALOGD("maxOutputPressure = %lf", maxOutputPressure);
    ALOGD("boomVerticalLen = %lf", boomVerticalLen);
    ALOGD("boomHorizontalLen = %f", boomHorizontalLen);
    ALOGD("totalWeight = %lf", totalWeight);
    ALOGD("equipmentWeight = %lf", equipmentWeight);
    ALOGD("pumptype = %d\n", pumptype);
    ALOGD("drivingLicenseDate = ", drivingLicenseDate.toString(DATE_FORMAT_STR).toStdString().data());
    ALOGD("createDate = ", createDate.toString(DATE_FORMAT_STR).toStdString().data());
    ALOGD("productionDate = ", productionDate.toString(DATE_FORMAT_STR).toStdString().data());
}

bool
Car::isValueEqualWithoutRecords(Car &c)
{
    return isValueEqual(c, true);
}

bool
Car::isValueEqual(Car &c, bool isWithoutRecords)
{
    if (number != c.number ||
        carNumber != c.carNumber ||
        bankAccount != c.bankAccount ||
        owner != c.owner ||
        carBrand != c.carBrand ||
        chassisBrand != c.chassisBrand ||
        fuelCarNumber != c.fuelCarNumber ||
        frameNumber != c.frameNumber ||
        identificationNumber != c.identificationNumber ||
        productNumber != c.productNumber ||
        insuranceCardNumber != c.insuranceCardNumber ||
        engineNumber != c.engineNumber ||
        dimensions != c.dimensions ||
        factoryCode != c.factoryCode ||
        operator1 != c.operator1 ||
        operator2 != c.operator2 ||
        operator3 != c.operator3 ||
        operator4 != c.operator4 ||
        operator5 != c.operator5 ||
        remarks != c.remarks ||
        creator != c.creator ||
        productionDate != c.productionDate ||
        drivingLicenseDate != c.drivingLicenseDate ||
        createDate != c.createDate ||
        !Util::isDoubleEqual(this->pumpedSquare, c.pumpedSquare) ||
        !Util::isDoubleEqual(this->pumpedTimes, c.pumpedTimes) ||
        !Util::isDoubleEqual(this->milage, c.milage) ||
        !Util::isDoubleEqual(this->worth, c.worth) ||
        !Util::isDoubleEqual(this->enginePower, c.enginePower) ||
        !Util::isDoubleEqual(this->maxDeliverySizes, c.maxDeliverySizes) ||
        !Util::isDoubleEqual(this->maxOutputPressure, c.maxOutputPressure) ||
        !Util::isDoubleEqual(this->boomVerticalLen, c.boomVerticalLen) ||
        !Util::isDoubleEqual(this->boomHorizontalLen, c.boomHorizontalLen) ||
        !Util::isDoubleEqual(this->totalWeight, c.totalWeight) ||
        !Util::isDoubleEqual(this->equipmentWeight, c.equipmentWeight) ||
        pumptype != c.pumptype) {
        return false;
    }

    if (isWithoutRecords)
        return true;

    if (insuranceRecords.size() != c.insuranceRecords.size() ||
        businessInsuranceRecords.size() != c.businessInsuranceRecords.size() ||
        annualFeeRecords.size() != c.annualFeeRecords.size() ||
        projectRecords.size() != c.projectRecords.size())
        return false;

    for (int i = 0; i < c.insuranceRecords.size(); i++) {
        if (insuranceRecords.at(i).number != c.insuranceRecords.at(i).number ||
            insuranceRecords.at(i).carNumber != c.insuranceRecords.at(i).carNumber ||
            insuranceRecords.at(i).fee != c.insuranceRecords.at(i).fee ||
            insuranceRecords.at(i).company != c.insuranceRecords.at(i).company ||
            insuranceRecords.at(i).remarks != c.insuranceRecords.at(i).remarks ||
            insuranceRecords.at(i).date != c.insuranceRecords.at(i).date)
            return false;
    }

    for (int i = 0; i < c.businessInsuranceRecords.size(); i++) {
        if (businessInsuranceRecords.at(i).number != c.businessInsuranceRecords.at(i).number ||
            businessInsuranceRecords.at(i).carNumber != c.businessInsuranceRecords.at(i).carNumber ||
            businessInsuranceRecords.at(i).fee != c.businessInsuranceRecords.at(i).fee ||
            businessInsuranceRecords.at(i).company != c.businessInsuranceRecords.at(i).company ||
            businessInsuranceRecords.at(i).remarks != c.businessInsuranceRecords.at(i).remarks ||
            businessInsuranceRecords.at(i).date != c.businessInsuranceRecords.at(i).date)
            return false;
    }

    for (int i = 0; i < c.annualFeeRecords.size(); i++) {
        if (annualFeeRecords.at(i).number != c.annualFeeRecords.at(i).number ||
            annualFeeRecords.at(i).carNumber != c.annualFeeRecords.at(i).carNumber ||
            annualFeeRecords.at(i).annualFee != c.annualFeeRecords.at(i).annualFee ||
            annualFeeRecords.at(i).travelExpenses != c.annualFeeRecords.at(i).travelExpenses ||
            annualFeeRecords.at(i).remarks != c.annualFeeRecords.at(i).remarks ||
            annualFeeRecords.at(i).date != c.annualFeeRecords.at(i).date)
            return false;
    }

    for (int i = 0; i < c.projectRecords.size(); i++) {
        if (projectRecords.at(i).number != c.projectRecords.at(i).number ||
            projectRecords.at(i).carNumber != c.projectRecords.at(i).carNumber ||
            projectRecords.at(i).rentalDocNum != c.projectRecords.at(i).rentalDocNum ||
            projectRecords.at(i).contractNum != c.projectRecords.at(i).contractNum ||
            projectRecords.at(i).clientNum != c.projectRecords.at(i).clientNum ||
            projectRecords.at(i).clientName != c.projectRecords.at(i).clientName ||
            projectRecords.at(i).amount != c.projectRecords.at(i).amount ||
            projectRecords.at(i).remarks != c.projectRecords.at(i).remarks ||
            projectRecords.at(i).date != c.projectRecords.at(i).date)
            return false;
    }
    return true;
}
