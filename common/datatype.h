#ifndef DATATYPE_H
#define DATATYPE_H

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

enum OpenType{CREATEITEM=0, SHOWITEM};

#endif // DATATYPE_H
