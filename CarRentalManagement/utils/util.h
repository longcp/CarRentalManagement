#ifndef UTIL_H
#define UTIL_H

#include <QObject>

class Util
{
public:
    Util();

    const static QString doubleToDecimal2String(double value);
    const static bool isDoubleEqual(double a, double b);
};

#endif // UTIL_H
