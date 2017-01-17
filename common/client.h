#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    enum    PayType {CASH, MONTHLY};                                    // 结账方式
    enum    ClientType {CONTACT, TEMPORARY};                            // 客户类型

    /**
     * 获取余额
     */
    int         getBalance();

private:
    QString     name;
    QString     number;
    QString     telephone;
    QString     address;
    QString     fax;
    QString     contract;
    QString     remarks;
    ClientType  clienttype;
    PayType     paytype;

    int         monthly;                                                // 月结日
    int         amount;                                                 // 工程款额
    int         paid;                                                   // 已付金额

signals:

public slots:
};

#endif // CLIENT_H
