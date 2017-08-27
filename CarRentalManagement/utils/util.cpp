#include "util.h"
#include <database/database.h>

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
        return makeCarMaxNum();

    case CLIENT_TABLE:
        return makeClientMaxNum();

    case CONTRACT_TABLE:
        return makeContractMaxNum();

    case RENTALDOCUMENT_TABLE:
        return makeRentalDocMaxNum();

    case ANNUAL_FEE_RECORD_TABLE:
        return makeAnnualFeeMaxNum();

    case BUSINESS_INSURANCE_RECORD_TABLE:
        return makeBusInsuranceMaxNum();

    case CONTRACT_PRICE_TABLE:
        return makeContractPriceMaxNum();

    case PROJECT_RECORD_TABLE:
        return makeProRecordMaxNum();

    case INSURANCE_RECORD_TABLE:
        return makeInsuranceMaxNum();

    case USER_TABLE:;
    default:
        break;
    }

    return "";
}
