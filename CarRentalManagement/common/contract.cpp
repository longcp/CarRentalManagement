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
          "deliverySizes = %lf\n"
          "structureLevel = %lf\n"
          "taxRate = %d\n"
          "signedDate = %s\n"
          "beginDate = %s\n"
          "endDate = %s\n"
          "creatDate = %s\n"
          "isIncludeTax = %s\n",
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
          signedDate.toString(DATE_FORMAT_STR).toStdString().data(),
          beginDate.toString(DATE_FORMAT_STR).toStdString().data(),
          endDate.toString(DATE_FORMAT_STR).toStdString().data(),
          creatDate.toString(DATE_FORMAT_STR).toStdString().data(),
          QString::number(isIncludeTax));
}

bool
Contract::isValueEqual(Contract &c)
{
    if (number != c.number ||
        clientName != c.clientName ||
        clientNumber != c.clientNumber ||
        projectName != c.projectName ||
        projectAddress != c.projectAddress ||
        requirement != c.requirement ||
        supplement != c.supplement ||
        remarks != c.remarks ||
        creator != c.creator ||
        deliverySizes != c.deliverySizes ||
        structureLevel != c.structureLevel ||
        taxRate != c.taxRate ||
        signedDate != c.signedDate ||
        beginDate != c.beginDate ||
        endDate != c.endDate ||
        creatDate != c.creatDate ||
        isIncludeTax != c.isIncludeTax) {
        return false;
    }
    if (prices.size() != c.prices.size()) {
        return false;
    }

    for (int i = 0; i < c.prices.size(); i++) {
        if (prices.at(i).number != c.prices.at(i).number ||
                prices.at(i).contractNumber != c.prices.at(i).contractNumber ||
                prices.at(i).pumpType != c.prices.at(i).pumpType ||
                prices.at(i).squarePrice != c.prices.at(i).squarePrice ||
                prices.at(i).standardPrice != c.prices.at(i).standardPrice ||
                prices.at(i).remarks != c.prices.at(i).remarks) {
            return false;
        }
    }

    return true;
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
