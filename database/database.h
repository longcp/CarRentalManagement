#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>

class User;

#define DEBUG_VERSION

//#ifdef DEBUG_VERSION
//#define
#define DB_PATH_SUFFIX  "./dataBase/CarRentalManagement.db"
#define DB_NAME         "carrentalmanagement"

class QMutex;
class Client;
class Car;
template <class Key, class T> class QMap;

class DataBase
{
public:
    DataBase();
    ~DataBase();
    static DataBase*    mInstance;
    static DataBase*    getInstance();

    int                 errorno;
    QString             lastError();

    // client 表
    int                 insertClientTable(Client &client);
    int                 getClientInNumber(QString clientNum,
                                          Client &client);
    int                 getAllClientsNumber(QList<QString> &numbers);
    int                 getAllClientData(QList<Client> &clients);
    int                 updateClientTableItem(Client &client);
    int                 clearClientTable();
    bool                isClientExist(Client &client);
    int                 deleteClientInNumber(QString clientNum);

    // user表
    int                 insertUserTable(User &user);
    int                 updateUserTableData(User &user);
    int                 getUserTableData(User &user, QString name);
    int                 getAllUserTableData(QList<User> &users);
    int                 getUserCount();
    int                 deleteUserTabledata(QString uName);

    // car表
    int                 insertCarTable(Car &car);
    int                 updateCarTableData(Car &car);
    int                 getAllCarData(QList<Car> &cars);
    int                 clearCarTable();
    int                 deleteCarDataInNumber(QString number);
    int                 getCarInNumber(QString number,
                                          Car &car);

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
