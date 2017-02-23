#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{

}

Client::~Client()
{

}

Client::ClientType
Client::getClientType(QString type)
{
    if (type == CLIENT_TYPE_TEMPORARY_STR)
        return TEMPORARY;
    else if (type == CLIENT_TYPE_CONTRACT_STR)
        return CONTRACT;

    return UNKNOWN_CLIENTTYPE;
}

Client::PayType Client::getPayType(QString type)
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
