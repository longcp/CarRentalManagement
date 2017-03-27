#include "rentaldocument.h"
#include "datatype.h"

RentalDocument::RentalDocument()
{

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
