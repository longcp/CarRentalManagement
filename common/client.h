#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDate>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    enum    PayType {CASH, MONTHLY};                                    // 结账方式
    enum    ClientType {CONTACT, TEMPORARY};                            // 客户类型

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
    int         amount;                                                 // 工程款额
    int         paid;                                                   // 已付金额

signals:

public slots:
private:
};

#endif // CLIENT_H
