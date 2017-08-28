#include "util.h"
#include <database/database.h>

#define DATABASE_NUMBER_FIELD_STR       "number"
#define RENTAL_DOC_NUM_PREFIX           "NO:"

#define LOG_TAG                         "UTIL"
#include "utils/Log.h"

Util::Util()
{

}

const QString
Util::doubleToDecimal2String(double value)
{
    char buf[64];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%0.2lf", value);
    return QString(buf);
}

const bool
Util::isDoubleEqual(double a, double b)
{
    return doubleToDecimal2String(a) == doubleToDecimal2String(b) ? true : false;
}

const QString
Util::makeNumber(DataBaseTable table)
{
    ALOGDTRACE();

    switch (table) {
    case CAR_TABLE:
        return makeSerialNumber(CAR_TABLE);

    case CLIENT_TABLE:
        return makeSerialNumber(CLIENT_TABLE);

    case CONTRACT_TABLE:
        return makeSerialNumber(CONTRACT_TABLE);

    case RENTALDOCUMENT_TABLE:
        return makeRentalDocMaxNum();

    case ANNUAL_FEE_RECORD_TABLE:
        return makeSerialNumber(ANNUAL_FEE_RECORD_TABLE);

    case BUSINESS_INSURANCE_RECORD_TABLE:
        return makeSerialNumber(BUSINESS_INSURANCE_RECORD_TABLE);

    case CONTRACT_PRICE_TABLE:
        return makeSerialNumber(CONTRACT_PRICE_TABLE);

    case PROJECT_RECORD_TABLE:
        return makeSerialNumber(PROJECT_RECORD_TABLE);

    case INSURANCE_RECORD_TABLE:
        return makeSerialNumber(INSURANCE_RECORD_TABLE);

    case USER_TABLE:;
    default:
        break;
    }

    return "";
}

const QString
Util::makeRentalDocMaxNum()
{
    return RENTAL_DOC_NUM_PREFIX+makeSerialNumber(RENTALDOCUMENT_TABLE);
}

const QString
Util::makeSerialNumber(DataBaseTable table)
{
    int num;
    int maxVal = pow(10, mSerialNumLen);
    QString tableName = DataBase::getInstance()->getTableName(table);
    QString number = DataBase::getInstance()->getMaxNumber(tableName, DATABASE_NUMBER_FIELD_STR);

    if (number == "")
        num = 0;
    else
        num = number.right(mSerialNumLen).toInt();

    if (num >= maxVal)
        num %= (maxVal-1);

    QString tailSuffix = QString("%1").arg(num+1);
    QString tailPrefix = "";
    for (int i = 0; i < (mSerialNumLen-tailSuffix.length()); i++) {
        //不满SERIAL_NUM_LEN位数，补0
        tailPrefix.append("0");
    }

    return tailPrefix.append(tailSuffix);
}
