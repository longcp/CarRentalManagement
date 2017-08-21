#include "util.h"

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
