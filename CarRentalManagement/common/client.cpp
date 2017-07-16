#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{

}

Client::~Client()
{

}

ClientType
Client::getClientType(QString type)
{
    if (type == CLIENT_TYPE_TEMPORARY_STR)
        return TEMPORARY;
    else if (type == CLIENT_TYPE_CONTRACT_STR)
        return CONTRACT;

    return UNKNOWN_CLIENTTYPE;
}

PayType Client::getPayType(QString type)
{
    if (type == PAY_TYPE_MONTHLY_STR)
        return MONTHLY;
    else if (type == PAY_TYPE_CASH_STR)
        return CASH;
    else
        return UNKNOWN_PAYTYPE;
}

QString
Client::getClientTypeStr(ClientType type)
{
    switch (type) {
        case CONTRACT:
           return CLIENT_TYPE_CONTRACT_STR;

        case TEMPORARY:
            return CLIENT_TYPE_TEMPORARY_STR;

        default:
            break;
    }

    return "";
}

QString
Client::getPayTypeStr(PayType type)
{
    switch (type) {
        case CASH:
            return PAY_TYPE_CASH_STR;

        case MONTHLY:
            return PAY_TYPE_MONTHLY_STR;

        default:
            break;
    }

    return "";
}

bool
Client::isValueEqual(Client &c)
{
    if (name == c.name &&
        number == c.number &&
        telephone == c.telephone &&
        address == c.address &&
        email == c.email &&
        fax == c.fax &&
        contract == c.contract &&
        remarks == c.remarks &&
        creator == c.creator &&
        clienttype == c.clienttype &&
        paytype == c.paytype &&
        createDate == c.createDate &&
        monthly == c.monthly &&
        amount == c.amount &&
        paid == c.paid)
        return true;

    return false;
}
