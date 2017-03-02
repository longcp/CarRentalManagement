#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>
#include <QDate>

#define USER_RIGHT_ROOT_STR     "管理员"
#define USER_RIGHT_NORMAL_STR   "普通用户"

#define PAY_TYPE_MONTHLY_STR    "月结"
#define PAY_TYPE_CASH_STR       "现金"
#define CLIENT_TYPE_CONTRACT_STR    "合同"
#define CLIENT_TYPE_TEMPORARY_STR   "临时"

#define PUMP_TYPE_37M1_STR      "37M1#"
#define PUMP_TYPE_48M_STR       "48M"
#define PUMP_TYPE_52M_STR       "52M"
#define PUMP_TYPE_56M_STR       "56M"
#define PUMP_TYPE_60M_STR       "60M"
#define PUMP_TYPE_CAR_PUMP_STR  "车载泵"
#define PUMP_TYPE_UNKNOWN_STR   "UNKNOWN_PUMPTYPE"

// 错误号
#define OK                          0
#define SUCCESS                     0
#define GET_DATABASE_FAIL           (-1001)
#define INSERT_CLIENT_TABLE_FAIL    (-1002)
#define DELETE_TABLE_FAIL           (-1003)
#define DATABASE_ITEM_EXIST         (-1004)
#define DATABASE_ITEM_NOT_EXIST     (-1004)
#define SELECT_DATABASE_FAIL        (-1006)
#define UPDATE_CLIENT_ITEM_FAIL     (-1007)
#define DELETE_CLIENT_ITEM_FAIL     (-1008)
#define INSERT_USER_ITEM_FAIL       (-1009)

enum OpenType {CREATEITEM=0, SHOWITEM, UNKNOWN_OPENTYPE};

enum UserRight {RIGHT_ROOT=0, RIGHT_NORMAL, UNKNOWN_RIGHT};

enum PayType {CASH=0, MONTHLY, UNKNOWN_PAYTYPE};                                    // 结账方式
enum ClientType {CONTRACT=0, TEMPORARY, UNKNOWN_CLIENTTYPE};

enum PumpType {TYPE_37M1,
               TYPE_48M,
               TYPE_52M,
               TYPE_56M,
               TYPE_60M,
               TYPE_CAR_PUMP,
               UNKNOWN_PUMPTYPE};

typedef struct InsuranceRecord {
    QString number;
    QString carNumber;
    QDate date;
    float fee;
    QString company;
    QString remarks;
}INSURANCE_RECORD;

typedef struct AnnualFeeRecord {
    QString number;
    QString carNumber;
    QDate date;
    float annualFee;
    float travelExpenses;
    QString remarks;
}ANNUALFEE_RECORD;

typedef struct ProjectRecord {
    QString number;
    QString carNumber;
    QDate date;
    QString contractNum;
    QString clientNum;
    QString clientName;
    float amount;
    QString remarks;
}PROJECT_RECORD;

#endif // DATATYPE_H
