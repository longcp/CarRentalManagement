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
    const static QString makeRentalDocMaxNum();
    const static QString makeSerialNumber(DataBaseTable table);

    const static int mSerialNumLen = 8;
};

#endif // UTIL_H
