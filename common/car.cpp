#include "car.h"

Car::Car(QObject *parent) : QObject(parent)
{

}

QString
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


PumpType
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

PumpType
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
