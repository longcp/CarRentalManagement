#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>

#define DEBUG_VERSION

//#ifdef DEBUG_VERSION
//#define
#define DB_PATH_SUFFIX  "./dataBase/CarRentalManagement.db"
#define DB_NAME         "carrentalmanagement"

class QMutex;
class Client;
template <class Key, class T> class QMap;

class DataBase
{
public:
    DataBase();
    ~DataBase();
    static DataBase*    mInstance;
    static DataBase*    getInstance();

    // client 表
    int                 insertClientTable(Client &client);
    int                 clearClientTable();

private:
    bool                openDataBase();
    bool                closeDataBase();
    QSqlQuery*          getDataBaseQuery();

    QSqlDatabase            mDb;
    QMutex*                 pmMutex;
    QMap<int, QSqlQuery*>*  pmQuery;

    int                     mId;
    QString                 mDbPath;
};

#endif // DATABASE_H
