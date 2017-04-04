#include "car.h"

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
    ALOGD("number = %s\n"
          "owner = %s\n"
          "bankAccount = %s\n"
          "carNumber = %s\n"
          "carBrand = %s\n"
          "chassisBrand = %s\n"
          "fuelCarNumber = %s\n"
          "frameNumber = %s\n"
          "identificationNumber = %s\n"
          "engineNumber = %s\n"
          "insuranceCardNumber = %s\n"
          "factoryCode = %s\n"
          "productNumber = %s\n"
          "dimensions = %s\n"
          "operator1 = %s\n"
          "operator2 = %s\n"
          "operator3 = %s\n"
          "operator4 = %s\n"
          "operator5 = %s\n"
          "remarks = %s\n"
          "creator = %s\n"
          "pumpedSquare = %f\n"
          "pumpedTimes = %f\n"
          "milage = %f\n"
          "worth = %f\n"
          "enginePower = %f\n"
          "maxDeliverySizes = %f\n"
          "maxOutputPressure = %f\n"
          "boomVerticalLen = %f\n"
          "boomHorizontalLen = %f\n"
          "totalWeight = %f\n"
          "equipmentWeight = %f\n"
          "pumptype = %d\n",
          "pumptype = %d\n",
          "pumptype = %d\n",
          "pumptype = %d\n",
          "drivingLicenseDate",
          "createDate",
          "productionDate",
          number.toStdString().data(),
          owner.toStdString().data(),
          bankAccount.toStdString().data(),
          carNumber.toStdString().data(),
          carBrand.toStdString().data(),
          chassisBrand.toStdString().data(),
          fuelCarNumber.toStdString().data(),
          frameNumber.toStdString().data(),
          identificationNumber.toStdString().data(),
          engineNumber.toStdString().data(),
          insuranceCardNumber.toStdString().data(),
          factoryCode.toStdString().data(),
          productNumber.toStdString().data(),
          dimensions.toStdString().data(),
          operator1.toStdString().data(),
          operator2.toStdString().data(),
          operator3.toStdString().data(),
          operator4.toStdString().data(),
          operator5.toStdString().data(),
          remarks.toStdString().data(),
          creator.toStdString().data(),
          pumpedSquare,
          pumpedTimes,
          milage,
          worth,
          enginePower,
          maxDeliverySizes,
          maxOutputPressure,
          boomVerticalLen,
          boomHorizontalLen,
          totalWeight,
          equipmentWeight,
          pumptype,
          drivingLicenseDate.toString(DATE_FORMAT_STR).toStdString().data(),
          createDate.toString(DATE_FORMAT_STR).toStdString().data(),
          productionDate.toString(DATE_FORMAT_STR).toStdString().data());
}
