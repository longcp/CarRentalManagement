#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>

#define USER_RIGHT_ROOT_STR     "管理员"
#define USER_RIGHT_NORMAL_STR   "普通用户"

#define PAY_TYPE_MONTHLY_STR    "月结"
#define PAY_TYPE_CASH_STR       "现金"
#define CLIENT_TYPE_CONTRACT_STR    "合同"
#define CLIENT_TYPE_TEMPORARY_STR   "临时"

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

#endif // DATATYPE_H
