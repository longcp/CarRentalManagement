#include "contract.h"

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
