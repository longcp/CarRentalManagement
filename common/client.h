#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDate>
#include "datatype.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    enum PayType {CASH, MONTHLY};                                    // 结账方式
    enum ClientType {CONTACT, TEMPORARY};                            // 客户类型

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
