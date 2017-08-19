#include "floatoperator.h"
#include <stdio.h>

FloatOperator::FloatOperator(QObject *parent) : QObject(parent)
{

}

float
FloatOperator::add2(float f1, float f2)
{
    char buf1[64], buf2[64];

    memset(buf1, 0, sizeof(buf1));
    memset(buf2, 0, sizeof(buf2));

    sprintf(buf1, "%f", f1);
    sprintf(buf2, "%f", f2);

    QStringList strList1 = QString(buf1).split(".");
    QString integer1 = strList1[0];
    QString decimal1 = strList1[1];

    QStringList strList2 = QString(buf2).split(".");
    QString integer2 = strList2[0];
    QString decimal2 = strList2[1];
}
