#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDate>
#include "datatype.h"

#define PAY_TYPE_MONTHLY_STR    "月结"
#define PAY_TYPE_CASH_STR       "现金"
#define CLIENT_TYPE_CONTRACT_STR    "合同"
#define CLIENT_TYPE_TEMPORARY_STR   "临时"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    enum PayType {CASH, MONTHLY, UNKNOWN_PAYTYPE};                                    // 结账方式
    enum ClientType {CONTRACT, TEMPORARY, UNKNOWN_CLIENTTYPE};                            // 客户类型

#if 0
    Client& operator=(const Client& c) {
        name = c.name;
        number = c.number;
        telephone = c.telephone;
        address = c.address;
        email = c.email;
        fax = c.fax;
        contract = c.contract;
        remarks = c.remarks;
        creator = c.creator;
        clienttype = c.clienttype;
        paytype = c.paytype;
        createDate = c.createDate;
        monthly = c.monthly;
        amount = c.amount;
        paid = c.paid;
        return *this;
    }
#endif
    /**
     * 获取余额
     */
    int         getBalance();
    /**
     * @brief enum转字符串
     */
    QString     getClientTypeStr(ClientType type);
    /**
     * @brief enum转字符串
     */
    QString     getPayTypeStr(PayType type);
    /**
     * @brief 字符串转ClientType
     */
    ClientType  getClientType(QString type);
    /**
     * @brief 字符串转PayType
     */
    PayType     getPayType(QString type);


    QString     name;
    QString     number;
    QString     telephone;
    QString     address;
    QString     email;
    QString     fax;
    QString     contract;
    QString     remarks;
    QString     creator;
    ClientType  clienttype;
    PayType     paytype;
    QDate       createDate;

    int         monthly;                                                // 月结日
    float       amount;                                                 // 工程款额
    float       paid;                                                   // 已付金额

signals:

public slots:
private:
};

#endif // CLIENT_H
