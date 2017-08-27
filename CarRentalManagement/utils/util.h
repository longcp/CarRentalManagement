#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <datatype.h>

class Util
{
public:
    Util();

    const static QString doubleToDecimal2String(double value);
    const static bool isDoubleEqual(double a, double b);
    const static QString makeNumber(DataBaseTable table);
    
private:
    QString makeCarMaxNum();
    QString makeClientMaxNum();
//    QString makeUserMaxNum();
    QString makeContractMaxNum();
    QString makeRentalDocMaxNum();
    QString makeAnnualFeeMaxNum();
    QString makeBusInsuranceMaxNum();
    QString makeContractPriceMaxNum();
    QString makeProRecordMaxNum();
    QString makeInsuranceMaxNum();
};

#endif // UTIL_H
