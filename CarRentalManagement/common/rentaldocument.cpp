#include "rentaldocument.h"
#include "datatype.h"
#include <util.h>

#define LOG_TAG                         "RENTALDOCUMENT"
#include "utils/Log.h"

RentalDocument::RentalDocument()
{

}


bool
RentalDocument::isValueEqual(RentalDocument &r)
{
    if (number == r.number &&
        clientName == r.clientName &&
        clientNumber == r.clientNumber &&
        contractNumber == r.contractNumber &&
        carNumber == r.carNumber &&
        carPlateNumber == r.carPlateNumber &&
        constructPlace == r.constructPlace &&
        concreteLable == r.concreteLable &&
        principal == r.principal &&
        principalTel == r.principalTel &&
        driver1 == r.driver1 &&
        driver2 == r.driver2 &&
        driver3 == r.driver3 &&
        projectName == r.projectName &&
        projectAddress == r.projectAddress &&
        remarks == r.remarks &&
        Util::isDoubleEqual(this->beginFuel, r.beginFuel) &&
        Util::isDoubleEqual(this->endFuel, r.endFuel) &&
        Util::isDoubleEqual(this->projectAmount, r.projectAmount) &&
        Util::isDoubleEqual(this->pumpSquare, r.pumpSquare) &&
        Util::isDoubleEqual(this->squareUnitPrice, r.squareUnitPrice) &&
        Util::isDoubleEqual(this->pumpTimes, r.pumpTimes) &&
        Util::isDoubleEqual(this->pumpTimeUnitPrice, r.pumpTimeUnitPrice) &&
        Util::isDoubleEqual(this->receivedAccounts, r.receivedAccounts) &&
        Util::isDoubleEqual(this->workingHours, r.workingHours) &&
        date == r.date &&
        arrivalDateTime == r.arrivalDateTime &&
        leaveDateTime == r.leaveDateTime &&
        state == r.state &&
        pumpType == r.pumpType &&
        calculateType == r.calculateType) {
        return true;
    }
    return false;
}

void
RentalDocument::dump()
{
    ALOGD("number = %s", number.toStdString().data());
    ALOGD("clientName = %s", clientName.toStdString().data());
    ALOGD("clientNumber = %s", clientNumber.toStdString().data());
    ALOGD("contractNumber = %s", contractNumber.toStdString().data());
    ALOGD("carNumber = %s", carNumber.toStdString().data());
    ALOGD("carPlateNumber = %s", carPlateNumber.toStdString().data());
    ALOGD("constructPlace = %s", constructPlace.toStdString().data());
    ALOGD("concreteLable = %s", concreteLable.toStdString().data());
    ALOGD("principal = %s", principal.toStdString().data());
    ALOGD("principalTel = %s", principalTel.toStdString().data());
    ALOGD("driver1 = %s", driver1.toStdString().data());
    ALOGD("driver2 = %s", driver2.toStdString().data());
    ALOGD("driver3 = %s", driver3.toStdString().data());
    ALOGD("projectName = %s", projectName.toStdString().data());
    ALOGD("projectAddress = %s", projectAddress.toStdString().data());
    ALOGD("remarks = %s", remarks.toStdString().data());
    ALOGD("beginFuel = %lf", beginFuel);
    ALOGD("endFuel = %lf", endFuel);
    ALOGD("projectAmount = %lf", projectAmount);
    ALOGD("pumpSquare = %lf", pumpSquare);
    ALOGD("squareUnitPrice = %lf", squareUnitPrice);
    ALOGD("pumpTimes = %lf", pumpTimes);
    ALOGD("pumpTimeUnitPrice = %lf", pumpTimeUnitPrice);
    ALOGD("receivedAccounts = %lf", receivedAccounts);
    ALOGD("workingHours = %lf", workingHours);
    ALOGD("date = %s", date.toString(DATE_FORMAT_STR).toStdString().data());
    ALOGD("arrivalDateTime = %s", arrivalDateTime.toString(DATETIME_FORMAT_STR).toStdString().data());
    ALOGD("leaveDateTime = %s", leaveDateTime.toString(DATETIME_FORMAT_STR).toStdString().data());
    ALOGD("state = %d", state);
    ALOGD("pumpType = %d", pumpType);
    ALOGD("calculType = %d", calculateType);
}

const QString
RentalDocument::getRentalDocStatStr(RentalDocState stat)
{
    switch (stat) {
    case RentalDocState::RESERVATION_STATE:
        return RENTAL_DOC_STATE_RESERVATION_STR;

    case RentalDocState::CONFIRMED_STATE:
        return RENTAL_DOC_STATE_CONFIRMED_STR;

    case RentalDocState::UNCONFIRMED_STATE:
        return RENTAL_DOC_STATE_UNCONFIRMED_STR;

    default:
        return RENTAL_DOC_STATE_UNKNOWN_STR;
    }
}

const RentalDocState
RentalDocument::getRentalDocStat(QString stat)
{
    if (stat == RENTAL_DOC_STATE_RESERVATION_STR)
        return RentalDocState::RESERVATION_STATE;

    else if (stat == RENTAL_DOC_STATE_CONFIRMED_STR)
        return RentalDocState::CONFIRMED_STATE;

    else if (stat == RENTAL_DOC_STATE_UNCONFIRMED_STR)
        return RentalDocState::UNCONFIRMED_STATE;

    return RentalDocState::UNKNOWN_STATE;
}
