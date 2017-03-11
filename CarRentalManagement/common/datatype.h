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

#define PUMP_TYPE_37M1_COLUMN       0
#define PUMP_TYPE_48M_COLUMN        1
#define PUMP_TYPE_52M_COLUMN        2
#define PUMP_TYPE_56M_COLUMN        3
#define PUMP_TYPE_60M_COLUMN        4
#define PUMP_TYPE_CAR_PUMP_COLUMN   5

// 错误号
#define OK                          0
#define SUCCESS                     0
#define GET_DATABASE_FAIL           (-1001)
#define INSERT_CLIENT_TABLE_FAIL    (-1002)
#define DELETE_TABLE_FAIL           (-1003)
#define DATABASE_ITEM_EXIST         (-1004)
#define DATABASE_ITEM_NOT_EXIST     (-1005)
#define SELECT_DATABASE_FAIL        (-1006)
#define UPDATE_CLIENT_ITEM_FAIL     (-1007)
#define DELETE_CLIENT_ITEM_FAIL     (-1008)
#define INSERT_USER_ITEM_FAIL       (-1009)
#define INSERT_CAR_ITEM_FAIL        (-1010)
#define UPDATE_CAR_ITEM_FAIL        (-1011)
#define DELETE_CAR_ITEM_FAIL        (-1012)
#define INSERT_INSURANCE_ITEM_FAIL  (-1013)
#define DELETE_INSURANCE_ITEM_FAIL  (-1014)
#define INSERT_ANNUAL_ITEM_FAIL     (-1015)
#define DELETE_ANNUAL_ITEM_FAIL     (-1016)
#define INSERT_PROJECT_ITEM_FAIL    (-1017)
#define DELETE_PROJECT_ITEM_FAIL    (-1018)
#define INSERT_CONTRACT_ITEM_FAIL   (-1019)
#define DELETE_CONTRACT_ITEM_FAIL   (-1020)
#define UPDATE_CONTRACT_ITEM_FAIL   (-1021)
#define INSERT_PRICE_ITEM_FAIL      (-1022)
#define DELETE_PRICE_ITEM_FAIL      (-1023)
#define UPDATE_PRICE_ITEM_FAIL      (-1024)

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
    QString rentalDocNum;
    QDate date;
    QString contractNum;
    QString clientNum;
    QString clientName;
    float amount;
    QString remarks;
}PROJECT_RECORD;

typedef struct ContractPrice {
    QString number;
    QString contractNumber;
    PumpType pumpType;
    float squarePrice;
    float standardPrice;
    float within150MinPrice;
    float within240MinPrice;
    QString remarks;
}CONTRACT_PRICE;

#endif // DATATYPE_H