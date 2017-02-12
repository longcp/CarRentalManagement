#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>

//#define DEBUG_VERSION
#define DB_PATH_SUFFIX  "./dataBase/ElecProtectManager.db"
#define DB_NAME         "carrentalmanagement"

class DataBase
{
public:
    DataBase();
    ~DataBase();
    static DataBase*    mInstance;
    static DataBase*    getInstance();
    QSqlQuery*          getDataBaseQuery();

private:
    bool                openDataBase();
    bool                closeDataBase();
    QSqlDatabase        mDb;
    QSqlDatabase        mutex;
    QString             mDbPath;
};

#endif // DATABASE_H
