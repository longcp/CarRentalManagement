#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{

}

Client::~Client()
{

}

QString
Client::getClientTypeStr(ClientType type)
{
    QString ret = "临时";

    switch (type) {
        case CONTACT:
            ret = "合同";
            break;

        case TEMPORARY:
            ret = "临时";
            break;

        default:
            break;
    }

    return ret;
}

QString
Client::getPayTypeStr(PayType type)
{
    QString ret = "月结";

    switch (type) {
        case CASH:
            ret = "现金";
            break;

        case MONTHLY:
            ret = "月结";
            break;

        default:
            break;
    }

    return ret;
}
