#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>
#include <QDate>

#define DATE_FORMAT_STR         "yyyy-MM-dd"
#define DATETIME_FORMAT_STR     "yyyy-MM-dd hh:mm:ss"
#define DATETIME_SERIAL_NUM_FORMAT_STR     "yyyyMMddhhmmsszzz"

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

#define RENTAL_DOC_STATE_RESERVATION_STR    "预约"
#define RENTAL_DOC_STATE_CONFIRMED_STR      "已确认"
#define RENTAL_DOC_STATE_UNCONFIRMED_STR    "未确认"
#define RENTAL_DOC_STATE_UNKNOWN_STR        "未确认"

#define MENU_TEXT_SYSTEM_SETTING        "系统设置"
#define MENU_TEXT_LEASING_MANAGERMENT   "租赁管理"
#define MENU_TEXT_CAR_MANAGERMENT       "车辆管理"
#define MENU_TEXT_FINANCIAL_STATISTICS  "财务统计"
#define MENU_TEXT_UPLOAD_IMAGE          "上传图片"
#define MENU_TEXT_USER_MANAGERMENT      "用户管理"
#define MENU_TEXT_HELP                  "帮助"

#define ACTION_TEXT_EXIT_SYSTEM         "退出系统"
#define ACTION_TEXT_CLIENT_MANAGERMENT  "客户资料"
#define ACTION_TEXT_CONTRACT            "租赁合同"
#define ACTION_TEXT_RENTALDOCUMENT      "泵送签证单"
#define ACTION_TEXT_CAR_INFORMATION     "车辆档案"
#define ACTION_TEXT_RECEIVABLE          "应收账款"
#define ACTION_TEXT_CONTRACT_RECEIPT    "合同收款"
#define ACTION_TEXT_USER_MANAGERMENT    "用户管理"
#define ACTION_TEXT_ABOUT               "关于"

/* tab title */
#define TAB_TITLE_MAIN_PAGE         "主页"
#define TAB_TITLE_CLIENTMANAGERMENT ACTION_TEXT_CLIENT_MANAGERMENT
#define TAB_TITLE_CONTRACT          ACTION_TEXT_CONTRACT
#define TAB_TITLE_RENTALDOCUMENT    ACTION_TEXT_RENTALDOCUMENT
#define TAB_TITLE_CAR_INFOMATION    ACTION_TEXT_CAR_INFORMATION
#define TAB_TITLE_RECEIVABLE        ACTION_TEXT_RECEIVABLE
#define TAB_TITLE_RECEIPT           ACTION_TEXT_CONTRACT_RECEIPT

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
#define INSERT_DOC_ITEM_FAIL        (-1025)
#define UPDATE_DOC_ITEM_FAIL        (-1026)
#define DELETE_DOC_ITEM_FAIL        (-1027)

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

enum RentalDocState {RESERVATION_STATE,
                     CONFIRMED_STATE,
                     UNCONFIRMED_STATE,
                     UNKNOWN_STATE};

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
    int number;
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
    QString remarks;
}CONTRACT_PRICE;

typedef struct ReceivableDetail {
    QString concreateLabel;
    QString projectName;
    QString projectAddress;
    QString constructPlace;
    QString Payee;
    QString remarks;
    QString principal;
    QString principalTel;
    QString contractNumber;

    float pumpSquare;
    float squareUnitPrice;
    float pumpTimes;
    float pumpTimeUnitPrice;
    float totalAmount;
    float receivedAmount;

    PumpType pumptype;
} RECEIVABLE_DETAIL;

typedef struct ReceiptFilter {
    QDate fromDate;
    QDate toDate;
    QString rentalDocNumber;
    QString contractNumber;
    QString clientName;
    QString carNumber;
    ClientType clientType;
    PumpType pumpType;
    bool    isAccountPositive;//应收大于0
} RECEIPT_FILTER;

typedef struct ReceivableTotal {
    QString contractNumber;
    QString clientName;
} RECEIVABLE_TOTAL;

#endif // DATATYPE_H
