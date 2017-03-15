#include "contract.h"
#include <math.h>

#define LOG_TAG                 "CONTRACT"
#include "utils/Log.h"

Contract::Contract(QObject *parent)
    : QObject(parent)
{

}

void
Contract::dump()
{
    ALOGD("number = %s\n"
          "clientName = %s\n"
          "clientNumber = %s\n"
          "projectName = %s\n"
          "projectAddress = %s\n"
          "requirement = %s\n"
          "supplement = %s\n"
          "remarks = %s\n"
          "creator = %s\n"
          "deliverySizes = %f\n"
          "structureLevel = %f\n"
          "taxRate = %f\n"
          "signedDate = %s\n"
          "beginDate = %s\n"
          "endDate = %s\n"
          "creatDate = %s\n"
          "isIncludeTax = %d\n",
          number.toStdString().data(),
          clientName.toStdString().data(),
          clientNumber.toStdString().data(),
          projectName.toStdString().data(),
          projectAddress.toStdString().data(),
          requirement.toStdString().data(),
          supplement.toStdString().data(),
          remarks.toStdString().data(),
          creator.toStdString().data(),
          deliverySizes,
          structureLevel,
          taxRate,
          signedDate.toString("yyyy-MM-dd").toStdString().data(),
          beginDate.toString("yyyy-MM-dd").toStdString().data(),
          endDate.toString("yyyy-MM-dd").toStdString().data(),
          creatDate.toString("yyyy-MM-dd").toStdString().data(),
          isIncludeTax);
}

const QString
Contract::makeNewestContractNumber(int count)
{
    //合同号由 "C" + "yyyyMMdd" + "xxxxxx"组成
    //其中C为contract缩写，yyyyMMdd为年月日，xxxxxx为6尾数
    int tailLen = 6;
    int maxVal = pow(10, 6);
    QString header = "C";
    QString date = QDate::currentDate().toString("yyyyMMdd");

    int tailNum = count+1;
    if (tailNum >= maxVal)
        tailNum %= (maxVal-1);

    QString tailSuffix = QString("%1").arg(tailNum);
    QString tailPrefix = "";
    for (int i = 0; i < (tailLen-tailSuffix.length()); i++) {
        //不满6位数，补0
        tailPrefix.append("0");
    }
    QString tail = tailPrefix.append(tailSuffix);

    return header.append(date).append(tail);
}
