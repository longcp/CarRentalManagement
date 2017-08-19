#include "database.h"
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>
#include <client.h>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <user.h>
#include <car.h>
#include <contract.h>
#include <rentaldocument.h>

#define LOG_TAG                         "DATABASE"
#include "utils/Log.h"

DataBase::DataBase() :
    pmMutex(new QMutex(QMutex::Recursive)),
    pmQuery(new QMap<int, QSqlQuery*>),
    errorno(0)
{
    mDbPath = QDir::currentPath().append("/").append(DB_PATH_SUFFIX);
    openDataBase();
}

DataBase::~DataBase()
{
    delete pmMutex;
}

DataBase*
DataBase::getInstance()
{
    if (!mInstance)
        mInstance = new DataBase;

    return mInstance;
}

bool
DataBase::openDataBase()
{
    if (!QFile::exists(mDbPath)) {
        qDebug()<< "database" << mDbPath << " is not existed!";
        if (!QFile::copy(mDbPath, mDbPath)) {
            ALOGE("copy database[%s] failed.", mDbPath
                  .toStdString()
                  .data());
            return false;
        }
    }

    if (mDb.isOpen())
        return true;

    if (QSqlDatabase::contains(DB_NAME))
        mDb = QSqlDatabase::database(DB_NAME);
    else
        mDb = QSqlDatabase::addDatabase("QSQLITE", DB_NAME);

    mDb.setDatabaseName(mDbPath);
    if (!mDb.open()) {
        ALOGE("open database[%s] failed!", mDbPath
              .toStdString()
              .data());
        return false;
    }

    return true;
}

bool
DataBase::closeDataBase()
{
    if (mDb.isOpen()) {
        mDb.close();
        mDb.removeDatabase(DB_NAME);
    }

    return true;
}

QSqlQuery*
DataBase::getDataBaseQuery()
{
    if (!openDataBase()) {
        ALOGE("getDbQuery, openDataBase failed!");
        return NULL;
    }

    if (pmQuery->contains(mId))
        return pmQuery->value(mId);

    QSqlQuery *query = new QSqlQuery(mDb);
    if (!query) {
        ALOGE("getDbQuery, new QSqlQuery failed.");
        return NULL;
    }

    if (!query->exec("PRAGMA foreign_keys = ON;")) {
        qDebug() << query->lastError();
        return NULL;
    }

    pmQuery->insert(mId, query);

    return query;
}

bool
DataBase::isClientExist(Client &client)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM client WHERE number=?");
    query->addBindValue(client.number);
    if (!query->exec()) {
        ALOGE("exec [SELECT * FROM client WHERE number=%s] failed!",
              client.number.toStdString().data());
        errorno = SELECT_DATABASE_FAIL;
        return false;
    }

    if (!query->next())
        return false;

    return true;
}

int
DataBase::insertClientTable(Client &client)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("INSERT INTO client "
                   "VALUES(:name, :number, :telephone, "
                   ":address, :email, :fax, :contract, "
                   ":remarks, :creator, :paytype, "
                   ":clienttype, :createDate, :monthly, "
                   ":amount, :paid)");
    query->bindValue(":name", client.name);
    query->bindValue(":number", client.number);
    query->bindValue(":telephone", client.telephone);
    query->bindValue(":address", client.address);
    query->bindValue(":email", client.email);
    query->bindValue(":fax", client.fax);
    query->bindValue(":contract", client.contract);
    query->bindValue(":remarks", client.remarks);
    query->bindValue(":creator", client.creator);
    query->bindValue(":paytype", client.paytype);
    query->bindValue(":clienttype", client.clienttype);
    query->bindValue(":createDate", client
                     .createDate.toString(DATE_FORMAT_STR));
    query->bindValue(":monthly", client.monthly);
    query->bindValue(":amount", client.amount);
    query->bindValue(":paid", client.paid);

    if (!query->exec()) {
        ALOGE("insertClientTable failed!");
        return INSERT_CLIENT_TABLE_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getClientInNumber(QString clientNum, Client &client)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM client WHERE number=?");
    query->addBindValue(clientNum);
    if (!query->exec()) {
        ALOGE("[%s]: select client table failed!");
        errorno = SELECT_DATABASE_FAIL;
        return SELECT_DATABASE_FAIL;
    }

    if (query->next()) {
        client.name = query->value(0).toString();
        client.number = query->value(1).toString();
        client.telephone = query->value(2).toString();
        client.address = query->value(3).toString();
        client.email = query->value(4).toString();
        client.fax = query->value(5).toString();
        client.contract = query->value(6).toString();
        client.remarks = query->value(7).toString();
        client.creator = query->value(8).toString();
        client.paytype = query->value(9).toInt() ?
                    PayType::MONTHLY : PayType::CASH;
        client.clienttype = query->value(10).toInt() ?
                    ClientType::TEMPORARY : ClientType::CONTRACT;
        client.createDate = QDate::fromString(query->value(11)
                                              .toString(), DATE_FORMAT_STR);
        client.monthly = query->value(12).toInt();
        client.amount = query->value(13).toDouble();
        client.paid = query->value(14).toDouble();

        return SUCCESS;
    }

    return DATABASE_ITEM_NOT_EXIST;
}

int
DataBase::getAllClientsNumber(QList<QString> &numbers)
{
    ALOGDTRACE();
    QString number;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM client");
    if (!query->exec()) {
        ALOGE("SELECT * FROM client failed!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        number = query->value(1).toString();
        numbers.push_back(number);                              //插入list
    }

    ALOGD("getAllClientsNumber SUCCESS");
    return SUCCESS;
}

int
DataBase::getClientInFilter(QList<Client> &clients, QString filter)
{
    ALOGDTRACE();
    QString number;
    Client client;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    QString sqlStr = "SELECT * FROM client " + filter;
    query->finish();
    query->prepare(sqlStr);
    if (!query->exec()) {
        ALOGE("%s", sqlStr.toStdString().data());
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        client.name = query->value(0).toString();
        client.number = query->value(1).toString();
        client.telephone = query->value(2).toString();
        client.address = query->value(3).toString();
        client.email = query->value(4).toString();
        client.fax = query->value(5).toString();
        client.contract = query->value(6).toString();
        client.remarks = query->value(7).toString();
        client.creator = query->value(8).toString();
        client.paytype = query->value(9).toInt() ?
                     PayType::MONTHLY : PayType::CASH;
        client.clienttype = query->value(10).toInt() ?
                    ClientType::TEMPORARY : ClientType::CONTRACT;
        client.createDate = QDate::fromString(query->value(11)
                                              .toString(), DATE_FORMAT_STR);
        client.monthly = query->value(12).toInt();
        client.amount = query->value(13).toDouble();
        client.paid = query->value(14).toDouble();

        clients.push_back(client);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::deleteClientInNumber(QString clientNum)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM client WHERE number=?");
    query->addBindValue(clientNum);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM client failed!", __FUNCTION__);
        errorno = DELETE_CLIENT_ITEM_FAIL;
        return DELETE_CLIENT_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getAllClientData(QList<Client> &clients)
{
    Client client;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM client");
    if (!query->exec()) {
        ALOGE("SELECT * FROM client!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        client.name = query->value(0).toString();
        client.number = query->value(1).toString();
        client.telephone = query->value(2).toString();
        client.address = query->value(3).toString();
        client.email = query->value(4).toString();
        client.fax = query->value(5).toString();
        client.contract = query->value(6).toString();
        client.remarks = query->value(7).toString();
        client.creator = query->value(8).toString();
        client.paytype = query->value(9).toInt() ?
                    PayType::MONTHLY : PayType::CASH;
        client.clienttype = query->value(10).toInt() ?
                    ClientType::TEMPORARY : ClientType::CONTRACT;
        client.createDate = QDate::fromString(query->value(11)
                                              .toString(), DATE_FORMAT_STR);
        client.monthly = query->value(12).toInt();
        client.amount = query->value(13).toDouble();
        client.paid = query->value(14).toDouble();

        clients.push_back(client);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::updateClientTableItem(Client &client)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("UPDATE client "
                   "SET name=?, "
                   "telephone=?, "
                   "address=?, "
                   "email=?, "
                   "fax=?, "
                   "contract=?, "
                   "remarks=?, "
                   "creator=?, "
                   "paytype=?, "
                   "clienttype=?, "
                   "createDate=?, "
                   "monthly=?, "
                   "amount=?, "
                   "paid=? "
                   "WHERE number=?;");
    query->addBindValue(client.name);
    query->addBindValue(client.telephone);
    query->addBindValue(client.address);
    query->addBindValue(client.email);
    query->addBindValue(client.fax);
    query->addBindValue(client.contract);
    query->addBindValue(client.remarks);
    query->addBindValue(client.creator);
    query->addBindValue(client.paytype);
    query->addBindValue(client.clienttype);
    query->addBindValue(client.createDate);
    query->addBindValue(client.monthly);
    query->addBindValue(client.amount);
    query->addBindValue(client.paid);
    query->addBindValue(client.number);
    if (!query->exec()) {
        ALOGE("updateClientTableItem fail");
        errorno = UPDATE_CLIENT_ITEM_FAIL;
        return UPDATE_CLIENT_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::clearClientTable()
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    //删除操作
    query->finish();
    query->prepare("DELETE FROM client;");
    if (!query->exec()) {
        ALOGE("clearClientTable fail!");
        errorno = DELETE_TABLE_FAIL;
        return DELETE_TABLE_FAIL;
    }

    ALOGV("clearClientTable success!");
    return SUCCESS;
}

int
DataBase::getUserTableData(User &user, QString name)
{
    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    query->finish();
    query->prepare("SELECT * FROM user WHERE user_Name=?");
    query->addBindValue(name);
    if (!query->exec()) {
        ALOGE("select user table fail!");
        errorno = SELECT_DATABASE_FAIL;
        return SELECT_DATABASE_FAIL;
    }

    if (query->next()) {
        user.name = query->value(0).toString();
        user.passward = query->value(1).toString();
        user.right = query->value(2).toInt() ?
                    UserRight::RIGHT_NORMAL : UserRight::RIGHT_ROOT;
    }

    return SUCCESS;
}

int
DataBase::deleteUserTabledata(QString uName)
{
    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    //删除操作
    query->finish();
    query->prepare("DELETE FROM user WHERE user_Name=?");
    query->addBindValue(uName);
    if (!query->exec()) {
        ALOGE("delete user table data fail!");
        errorno = SELECT_DATABASE_FAIL;
        return SELECT_DATABASE_FAIL;
    }

    return SUCCESS;
}

int
DataBase::insertUserTable(User &user)
{
    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    //插入操作
    query->finish();
    query->prepare("INSERT INTO user VALUES(:user_Name, :user_Passward, :right_level)");
    query->bindValue(":user_Name",      user.name);
    query->bindValue(":user_Passward",  user.passward);
    query->bindValue(":right_level",    user.right);
    if (!query->exec()) {
        ALOGE("insert to user table fail!");
        errorno = INSERT_USER_ITEM_FAIL;
        return INSERT_USER_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::updateUserTableData(User &user)
{
    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    query->finish();
    query->prepare("UPDATE user SET user_Passward=? WHERE user_Name=?;");
    query->addBindValue(user.passward);
    query->addBindValue(user.name);
    if (!query->exec()) {
        ALOGE("select user table fail!");
        errorno = SELECT_DATABASE_FAIL;
        return SELECT_DATABASE_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getAllUserTableData(QList<User> &users)
{
    User user;

    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    query->finish();
    query->prepare("SELECT * FROM user");
    if (!query->exec()) {
        ALOGE("select user table fail!");
        errorno = SELECT_DATABASE_FAIL;
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        user.name = query->value(0).toString();
        user.passward = query->value(1).toString();
        user.right = query->value(2).toInt() ?
                    UserRight::RIGHT_NORMAL : UserRight::RIGHT_ROOT;

        users.push_back(user);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getUserCount()
{
    int count = 0;

    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    query->finish();
    query->prepare("SELECT * FROM user");
    if (!query->exec()) {
        ALOGE("select user table fail!");
        errorno = SELECT_DATABASE_FAIL;
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        count++;
    }

    return count;
}

QString
DataBase::lastError()
{
    switch(errorno) {
    case GET_DATABASE_FAIL:
        return "GET_DATABASE_FAIL";

    case INSERT_CLIENT_TABLE_FAIL:
        return "INSERT_CLIENT_TABLE_FAIL";

    case DELETE_TABLE_FAIL:
        return "DELETE_TABLE_FAIL";

    case DATABASE_ITEM_EXIST:
        return "DATABASE_ITEM_EXIST";

    case DATABASE_ITEM_NOT_EXIST:
        return "DATABASE_ITEM_NOT_EXIST";

    case SELECT_DATABASE_FAIL:
        return "SELECT_DATABASE_FAIL";

    case UPDATE_CLIENT_ITEM_FAIL:
        return "UPDATE_CLIENT_ITEM_FAIL";

    case DELETE_CLIENT_ITEM_FAIL:
        return "DELETE_CLIENT_ITEM_FAIL";

    case INSERT_USER_ITEM_FAIL:
        return "INSERT_USER_ITEM_FAIL";

    case INSERT_CAR_ITEM_FAIL:
        return "INSERT_CAR_ITEM_FAIL";

    case UPDATE_CAR_ITEM_FAIL:
        return "UPDATE_CAR_ITEM_FAIL";

    case DELETE_CAR_ITEM_FAIL:
        return "DELETE_CAR_ITEM_FAIL";

    case INSERT_CONTRACT_ITEM_FAIL:
        return "INSERT_CONTRACT_ITEM_FAIL";

    case DELETE_CONTRACT_ITEM_FAIL:
        return "DELETE_CONTRACT_ITEM_FAIL";

    case UPDATE_CONTRACT_ITEM_FAIL:
        return "UPDATE_CONTRACT_ITEM_FAIL";

    case INSERT_PRICE_ITEM_FAIL:
        return "INSERT_PRICE_ITEM_FAIL";

    case DELETE_PRICE_ITEM_FAIL:
        return "DELETE_PRICE_ITEM_FAIL";

    case UPDATE_PRICE_ITEM_FAIL:
        return "UPDATE_PRICE_ITEM_FAIL";

    case INSERT_DOC_ITEM_FAIL:
        return "INSERT_DOC_ITEM_FAIL";

    case UPDATE_DOC_ITEM_FAIL:
        return "UPDATE_DOC_ITEM_FAIL";

    case DELETE_DOC_ITEM_FAIL:
        return "DELETE_DOC_ITEM_FAIL";

    case OK:
    default:
        return "Ok";
        break;
    }

    return "Ok";
}

int
DataBase::insertCarTable(Car &car)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("INSERT INTO car "
                   "VALUES(:number, :carNumber, :bankAccount, "
                   ":owner, :carBrand, :chassisBrand, :fuelCarNumber, "
                   ":frameNumber, :identificationNumber, :productNumber, "
                   ":insuranceCardNumber, :engineNumber, :dimensions, "
                   ":factoryCode, :operator1, :operator2, "
                   ":operator3, :operator4, :operator5, "
                   ":remarks, :creator, :productionDate, "
                   ":drivingLicenseDate, :createDate, :pumpedSquare, "
                   ":pumpedTimes, :milage, :worth, "
                   ":enginePower, :maxDeliverySizes, :maxOutputPressure, "
                   ":boomVerticalLen, :boomHorizontalLen, :totalWeight, "
                   ":equipmentWeight, :pumptype)");
    query->bindValue(":number", car.number);
    query->bindValue(":carNumber", car.carNumber);
    query->bindValue(":bankAccount", car.bankAccount);
    query->bindValue(":owner", car.owner);
    query->bindValue(":carBrand", car.carBrand);
    query->bindValue(":chassisBrand", car.chassisBrand);
    query->bindValue(":fuelCarNumber", car.fuelCarNumber);
    query->bindValue(":frameNumber", car.frameNumber);
    query->bindValue(":identificationNumber", car.identificationNumber);
    query->bindValue(":productNumber", car.productNumber);
    query->bindValue(":insuranceCardNumber", car.insuranceCardNumber);
    query->bindValue(":engineNumber", car.engineNumber);
    query->bindValue(":dimensions", car.dimensions);
    query->bindValue(":factoryCode", car.factoryCode);
    query->bindValue(":operator1", car.operator1);
    query->bindValue(":operator2", car.operator2);
    query->bindValue(":operator3", car.operator3);
    query->bindValue(":operator4", car.operator4);
    query->bindValue(":operator5", car.operator5);
    query->bindValue(":remarks", car.remarks);
    query->bindValue(":creator", car.creator);

    query->bindValue(":productionDate",
                     car.productionDate.toString(DATE_FORMAT_STR));
    query->bindValue(":drivingLicenseDate",
                     car.drivingLicenseDate.toString(DATE_FORMAT_STR));
    query->bindValue(":createDate",
                     car.createDate.toString(DATE_FORMAT_STR));

    query->bindValue(":pumpedSquare", car.pumpedSquare);
    query->bindValue(":pumpedTimes", car.pumpedTimes);
    query->bindValue(":milage", car.milage);
    query->bindValue(":worth", car.worth);
    query->bindValue(":enginePower", car.enginePower);
    query->bindValue(":maxDeliverySizes", car.maxDeliverySizes);
    query->bindValue(":maxOutputPressure", car.maxOutputPressure);
    query->bindValue(":boomVerticalLen", car.boomVerticalLen);
    query->bindValue(":boomHorizontalLen", car.boomHorizontalLen);
    query->bindValue(":totalWeight", car.totalWeight);
    query->bindValue(":equipmentWeight", car.equipmentWeight);
    query->bindValue(":pumptype", car.pumptype);

    if (!query->exec()) {
        ALOGE("%s failed!", __FUNCTION__);
        errorno = INSERT_CAR_ITEM_FAIL;
        return INSERT_CAR_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::updateCarTableData(Car &car)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("UPDATE car "
                   "SET carNumber=?, "
                   "bankAccount=?, "
                   "owner=?, "
                   "carBrand=?, "
                   "chassisBrand=?, "
                   "fuelCarNumber=?, "
                   "frameNumber=?, "
                   "identificationNumber=?, "
                   "productNumber=?, "
                   "insuranceCardNumber=?, "
                   "engineNumber=?, "
                   "dimensions=?, "
                   "factoryCode=?, "
                   "operator1=?, "
                   "operator2=?, "
                   "operator3=?, "
                   "operator4=?, "
                   "operator5=?, "
                   "remarks=?, "
                   "creator=?, "
                   "productionDate=?, "
                   "drivingLicenseDate=?, "
                   "createDate=?, "
                   "pumpedSquare=?, "
                   "pumpedTimes=?, "
                   "milage=?, "
                   "worth=?, "
                   "enginePower=?, "
                   "maxDeliverySizes=?, "
                   "maxOutputPressure=?, "
                   "boomVerticalLen=?, "
                   "boomHorizontalLen=?, "
                   "totalWeight=?, "
                   "equipmentWeight=?, "
                   "pumptype=? "
                   "WHERE number=?;");
    query->addBindValue(car.carNumber);
    query->addBindValue(car.bankAccount);
    query->addBindValue(car.owner);
    query->addBindValue(car.carBrand);
    query->addBindValue(car.chassisBrand);
    query->addBindValue(car.fuelCarNumber);
    query->addBindValue(car.frameNumber);
    query->addBindValue(car.identificationNumber);
    query->addBindValue(car.productNumber);
    query->addBindValue(car.insuranceCardNumber);
    query->addBindValue(car.engineNumber);
    query->addBindValue(car.dimensions);
    query->addBindValue(car.factoryCode);
    query->addBindValue(car.operator1);
    query->addBindValue(car.operator2);
    query->addBindValue(car.operator3);
    query->addBindValue(car.operator4);
    query->addBindValue(car.operator5);
    query->addBindValue(car.remarks);
    query->addBindValue(car.creator);
    query->addBindValue(car.productionDate);
    query->addBindValue(car.drivingLicenseDate);
    query->addBindValue(car.createDate);
    query->addBindValue(car.pumpedSquare);
    query->addBindValue(car.pumpedTimes);
    query->addBindValue(car.milage);
    query->addBindValue(car.worth);
    query->addBindValue(car.enginePower);
    query->addBindValue(car.maxDeliverySizes);
    query->addBindValue(car.maxOutputPressure);
    query->addBindValue(car.boomVerticalLen);
    query->addBindValue(car.boomHorizontalLen);
    query->addBindValue(car.totalWeight);
    query->addBindValue(car.equipmentWeight);
    query->addBindValue(car.pumptype);
    query->addBindValue(car.number);
    if (!query->exec()) {
        ALOGE("updateClientTableItem fail");
        errorno = UPDATE_CAR_ITEM_FAIL;
        return UPDATE_CAR_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getAllCarData(QList<Car> &cars)
{
    Car car;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM car");
    if (!query->exec()) {
        ALOGE("SELECT * FROM car!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        car.number = query->value(0).toString();
        car.carNumber = query->value(1).toString();
        car.bankAccount = query->value(2).toString();
        car.owner = query->value(3).toString();
        car.carBrand = query->value(4).toString();
        car.chassisBrand = query->value(5).toString();
        car.fuelCarNumber = query->value(6).toString();
        car.frameNumber = query->value(7).toString();
        car.identificationNumber = query->value(8).toString();
        car.productNumber = query->value(9).toString();
        car.insuranceCardNumber = query->value(10).toString();
        car.engineNumber = query->value(11).toString();
        car.dimensions = query->value(12).toString();
        car.factoryCode = query->value(13).toString();
        car.operator1 = query->value(14).toString();
        car.operator2 = query->value(15).toString();
        car.operator3 = query->value(16).toString();
        car.operator4 = query->value(17).toString();
        car.operator5 = query->value(18).toString();
        car.remarks = query->value(19).toString();
        car.creator = query->value(20).toString();

        car.productionDate = QDate::fromString(query->value(21).toString(),
                                               DATE_FORMAT_STR);
        car.drivingLicenseDate = QDate::fromString(query->value(22).toString(),
                                                   DATE_FORMAT_STR);
        car.createDate = QDate::fromString(query->value(23).toString(),
                                           DATE_FORMAT_STR);

        car.pumpedSquare = query->value(24).toDouble();
        car.pumpedTimes = query->value(25).toDouble();
        car.milage = query->value(26).toDouble();
        car.worth = query->value(27).toDouble();
        car.enginePower = query->value(28).toDouble();
        car.maxDeliverySizes = query->value(29).toDouble();
        car.maxOutputPressure = query->value(30).toDouble();
        car.boomVerticalLen = query->value(31).toDouble();
        car.boomHorizontalLen = query->value(32).toDouble();
        car.totalWeight = query->value(33).toDouble();
        car.equipmentWeight = query->value(34).toDouble();

        car.pumptype = car.getPumpType(query->value(35).toInt());

        cars.push_back(car);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getCarInNumber(QString number, Car &car)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM car WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGE("[%s]: select car table failed!", __FUNCTION__);
        errorno = SELECT_DATABASE_FAIL;
        return SELECT_DATABASE_FAIL;
    }

    if (query->next()) {
        car.number = query->value(0).toString();
        car.carNumber = query->value(1).toString();
        car.bankAccount = query->value(2).toString();
        car.owner = query->value(3).toString();
        car.carBrand = query->value(4).toString();
        car.chassisBrand = query->value(5).toString();
        car.fuelCarNumber = query->value(6).toString();
        car.frameNumber = query->value(7).toString();
        car.identificationNumber = query->value(8).toString();
        car.productNumber = query->value(9).toString();
        car.insuranceCardNumber = query->value(10).toString();
        car.engineNumber = query->value(11).toString();
        car.dimensions = query->value(12).toString();
        car.factoryCode = query->value(13).toString();
        car.operator1 = query->value(14).toString();
        car.operator2 = query->value(15).toString();
        car.operator3 = query->value(16).toString();
        car.operator4 = query->value(17).toString();
        car.operator5 = query->value(18).toString();
        car.remarks = query->value(19).toString();
        car.creator = query->value(20).toString();

        car.productionDate = QDate::fromString(query->value(21).toString(),
                                               DATE_FORMAT_STR);
        car.drivingLicenseDate = QDate::fromString(query->value(22).toString(),
                                                   DATE_FORMAT_STR);
        car.createDate = QDate::fromString(query->value(23).toString(),
                                           DATE_FORMAT_STR);

        car.pumpedSquare = query->value(24).toDouble();
        car.pumpedTimes = query->value(25).toDouble();
        car.milage = query->value(26).toDouble();
        car.worth = query->value(27).toDouble();
        car.enginePower = query->value(28).toDouble();
        car.maxDeliverySizes = query->value(29).toDouble();
        car.maxOutputPressure = query->value(30).toDouble();
        car.boomVerticalLen = query->value(31).toDouble();
        car.boomHorizontalLen = query->value(32).toDouble();
        car.totalWeight = query->value(33).toDouble();
        car.equipmentWeight = query->value(34).toDouble();

        car.pumptype = car.getPumpType(query->value(35).toInt());

        return SUCCESS;
    }

    return DATABASE_ITEM_NOT_EXIST;
}

int
DataBase::clearCarTable()
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    //删除操作
    query->finish();
    query->prepare("DELETE FROM car;");
    if (!query->exec()) {
        ALOGE("%s fail!", __FUNCTION__);
        errorno = DELETE_TABLE_FAIL;
        return DELETE_TABLE_FAIL;
    }

    ALOGV("%s success!", __FUNCTION__);
    return SUCCESS;
}


int
DataBase::deleteCarDataInNumber(QString number)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM car WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM client failed!", __FUNCTION__);
        return DELETE_CAR_ITEM_FAIL;
    }

    return SUCCESS;
}

bool
DataBase::isCarExist(Car &car)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM car WHERE number=?");
    query->addBindValue(car.number);
    if (!query->exec()) {
        ALOGE("exec [SELECT * FROM car WHERE number=%s] failed!",
              car.number.toStdString().data());
        errorno = SELECT_DATABASE_FAIL;
        return false;
    }

    if (!query->next())
        return false;

    return true;
}

int
DataBase::insertInsuranceTable(INSURANCE_RECORD &record)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("INSERT INTO insurance_record "
                   "VALUES(:number, :carNumber, :company, "
                   ":remarks, :date, :fee)");
    query->bindValue(":number", record.number);
    query->bindValue(":carNumber", record.carNumber);
    query->bindValue(":company", record.company);
    query->bindValue(":remarks", record.remarks);
    query->bindValue(":date", record.date.toString(DATE_FORMAT_STR));
    query->bindValue(":fee", record.fee);

    if (!query->exec()) {
        ALOGE("%s failed!", __FUNCTION__);
        errorno = INSERT_INSURANCE_ITEM_FAIL;
        return INSERT_INSURANCE_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getAllInsuranceData(QList<INSURANCE_RECORD> &records)
{
    INSURANCE_RECORD record;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM insurance_record");
    if (!query->exec()) {
        ALOGE("SELECT * FROM insurance_record!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        record.number = query->value(0).toString();
        record.carNumber = query->value(1).toString();
        record.company = query->value(2).toString();
        record.remarks = query->value(3).toString();
        record.date = QDate::fromString(query->value(4).toString(),
                                               DATE_FORMAT_STR);
        record.fee = query->value(5).toDouble();

        records.push_back(record);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getInsuranceDataInCarNumber(QString carNumber, QList<INSURANCE_RECORD> &records)
{
    INSURANCE_RECORD record;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM insurance_record WHERE carNumber=?");
    query->addBindValue(carNumber);
    if (!query->exec()) {
        ALOGE("SELECT * FROM insurance_record!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        record.number = query->value(0).toString();
        record.carNumber = query->value(1).toString();
        record.company = query->value(2).toString();
        record.remarks = query->value(3).toString();
        record.date = QDate::fromString(query->value(4).toString(),
                                               DATE_FORMAT_STR);
        record.fee = query->value(5).toDouble();

        records.push_back(record);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::delInsuranceDataInNumber(QString number)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM insurance_record WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM insurance_record failed!", __FUNCTION__);
        return DELETE_INSURANCE_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::insertBusinessInsuranceTable(INSURANCE_RECORD &record)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("INSERT INTO business_insurance_record "
                   "VALUES(:number, :carNumber, :company, "
                   ":remarks, :date, :fee)");
    query->bindValue(":number", record.number);
    query->bindValue(":carNumber", record.carNumber);
    query->bindValue(":company", record.company);
    query->bindValue(":remarks", record.remarks);
    query->bindValue(":date", record.date.toString(DATE_FORMAT_STR));
    query->bindValue(":fee", record.fee);

    if (!query->exec()) {
        ALOGE("%s failed!", __FUNCTION__);
        errorno = INSERT_INSURANCE_ITEM_FAIL;
        return INSERT_INSURANCE_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getAllBusinessInsuranceData(QList<INSURANCE_RECORD> &records)
{
    INSURANCE_RECORD record;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM business_insurance_record");
    if (!query->exec()) {
        ALOGE("SELECT * FROM business_insurance_record!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        record.number = query->value(0).toString();
        record.carNumber = query->value(1).toString();
        record.company = query->value(2).toString();
        record.remarks = query->value(3).toString();
        record.date = QDate::fromString(query->value(4).toString(),
                                               DATE_FORMAT_STR);
        record.fee = query->value(5).toDouble();

        records.push_back(record);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getBusinessInsuranceDataInCarNumber(QString carNumber, QList<INSURANCE_RECORD> &records)
{
    INSURANCE_RECORD record;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM business_insurance_record WHERE carNumber=?");
    query->addBindValue(carNumber);
    if (!query->exec()) {
        ALOGE("SELECT * FROM business_insurance_record!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        record.number = query->value(0).toString();
        record.carNumber = query->value(1).toString();
        record.company = query->value(2).toString();
        record.remarks = query->value(3).toString();
        record.date = QDate::fromString(query->value(4).toString(),
                                               DATE_FORMAT_STR);
        record.fee = query->value(5).toDouble();

        records.push_back(record);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::delBusinessInsuranceDataInNumber(QString number)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM business_insurance_record WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM business_insurance_record failed!",
              __FUNCTION__);
        return DELETE_INSURANCE_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::insertAnnualTable(ANNUALFEE_RECORD &record)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("INSERT INTO annual_fee_record "
                   "VALUES(:number, :carNumber, :remarks, "
                   ":date, :annualFee, :travelExpenses)");
    query->bindValue(":number", record.number);
    query->bindValue(":carNumber", record.carNumber);
    query->bindValue(":remarks", record.remarks);
    query->bindValue(":date", record.date.toString(DATE_FORMAT_STR));
    query->bindValue(":annualFee", record.annualFee);
    query->bindValue(":travelExpenses", record.travelExpenses);

    if (!query->exec()) {
        ALOGE("%s failed!", __FUNCTION__);
        errorno = INSERT_ANNUAL_ITEM_FAIL;
        return INSERT_ANNUAL_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getAllAnnualData(QList<ANNUALFEE_RECORD> &records)
{
    ANNUALFEE_RECORD record;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM annual_fee_record");
    if (!query->exec()) {
        ALOGE("SELECT * FROM annual_fee_record!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        record.number = query->value(0).toString();
        record.carNumber = query->value(1).toString();
        record.remarks = query->value(2).toString();
        record.date = QDate::fromString(query->value(3).toString(),
                                               DATE_FORMAT_STR);
        record.annualFee = query->value(4).toDouble();
        record.travelExpenses = query->value(5).toDouble();

        records.push_back(record);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getAnnualDataInCarNumber(QString carNumber, QList<ANNUALFEE_RECORD> &records)
{
    ANNUALFEE_RECORD record;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM annual_fee_record WHERE carNumber=?");
    query->addBindValue(carNumber);
    if (!query->exec()) {
        ALOGE("SELECT * FROM annual_fee_record!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        record.number = query->value(0).toString();
        record.carNumber = query->value(1).toString();
        record.remarks = query->value(2).toString();
        record.date = QDate::fromString(query->value(3).toString(),
                                               DATE_FORMAT_STR);
        record.annualFee = query->value(4).toDouble();
        record.travelExpenses = query->value(5).toDouble();

        records.push_back(record);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::delAnnualDataInNumber(QString number)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM annual_fee_record WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM business_insurance_record failed!",
              __FUNCTION__);
        return DELETE_ANNUAL_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::insertProjectTable(PROJECT_RECORD &record)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    QString sqlStr = "INSERT INTO project_record "
                        "(number, carNumber, clientNum, rentalDocNum, "
                        "contractNum, clientName, remarks, date, amount) VALUES(null, ";
    sqlStr += "'" + record.carNumber + "', ";
    sqlStr += "'" + record.clientNum + "', ";
    sqlStr += "'" + record.rentalDocNum + "', ";
    sqlStr += "'" + record.contractNum + "', ";
    sqlStr += "'" + record.clientName + "', ";
    sqlStr += "'" + record.remarks + "', ";
    sqlStr += "'" + record.date.toString(DATE_FORMAT_STR) + "', ";
    sqlStr += QString("%1").arg(record.amount) + ")";
    query->finish();
    ALOGDTRACE();
    ALOGD("sqlStr = %s", sqlStr.toStdString().data());
    query->prepare(sqlStr);
//    query->prepare("INSERT INTO project_record "
//                   "VALUES(:number, :carNumber, :clientNum, "
//                   ":rentalDocNum, :contractNum, :clientName, "
//                   ":remarks, :date, :amount)");
//    query->bindValue(":number", record.number);
//    query->bindValue(":carNumber", record.carNumber);
//    query->bindValue(":clientNum", record.clientNum);
//    query->bindValue(":rentalDocNum", record.rentalDocNum);
//    query->bindValue(":contractNum", record.contractNum);
//    query->bindValue(":clientName", record.clientName);
//    query->bindValue(":remarks", record.remarks);
//    query->bindValue(":date", record.date.toString(DATE_FORMAT_STR));
//    query->bindValue(":amount", record.amount);

    if (!query->exec()) {
        ALOGE("%s failed!", __FUNCTION__);
        errorno = INSERT_PROJECT_ITEM_FAIL;
        return INSERT_PROJECT_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getAllProjectData(QList<PROJECT_RECORD> &records)
{
    PROJECT_RECORD record;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM project_record");
    if (!query->exec()) {
        ALOGE("SELECT * FROM project_record!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        record.number = query->value(0).toInt();
        record.carNumber = query->value(1).toString();
        record.clientNum = query->value(2).toString();
        record.rentalDocNum = query->value(3).toString();
        record.contractNum = query->value(4).toString();
        record.clientName = query->value(5).toString();
        record.remarks = query->value(6).toString();
        record.date = QDate::fromString(query->value(7).toString(),
                                               DATE_FORMAT_STR);
        record.amount = query->value(8).toDouble();

        records.push_back(record);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getProjectDataInCarNumber(QString carNumber, QList<PROJECT_RECORD> &records)
{
    PROJECT_RECORD record;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM project_record WHERE carNumber=?");
    query->addBindValue(carNumber);
    if (!query->exec()) {
        ALOGE("SELECT * FROM project_record!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        record.number = query->value(0).toInt();
        record.carNumber = query->value(1).toString();
        record.clientNum = query->value(2).toString();
        record.rentalDocNum = query->value(3).toString();
        record.contractNum = query->value(4).toString();
        record.clientName = query->value(5).toString();
        record.remarks = query->value(6).toString();
        record.date = QDate::fromString(query->value(7).toString(),
                                               DATE_FORMAT_STR);
        record.amount = query->value(8).toDouble();

        records.push_back(record);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::delProjectDataInNumber(QString number)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM project_record WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM project_record failed!",
              __FUNCTION__);
        return DELETE_PROJECT_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::insertContractTable(Contract &contract)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("INSERT INTO contract "
                   "VALUES(:number, :clientName, :clientNumber, "
                   ":projectName, :projectAddress, :requirement, "
                   ":supplement, :remarks, :creator, "
                   ":deliverySizes, :structureLevel, :taxRate, "
                   ":signedDate, :beginDate, :endDate, "
                   ":creatDate, :isIncludeTax)");
    query->bindValue(":number", contract.number);
    query->bindValue(":clientName", contract.clientName);
    query->bindValue(":clientNumber", contract.clientNumber);
    query->bindValue(":projectName", contract.projectName);
    query->bindValue(":projectAddress", contract.projectAddress);
    query->bindValue(":requirement", contract.requirement);
    query->bindValue(":supplement", contract.supplement);
    query->bindValue(":remarks", contract.remarks);
    query->bindValue(":creator", contract.creator);
    query->bindValue(":deliverySizes", contract.deliverySizes);
    query->bindValue(":structureLevel", contract.structureLevel);
    query->bindValue(":taxRate", contract.taxRate);

    query->bindValue(":signedDate",
                     contract.signedDate.toString(DATE_FORMAT_STR));
    query->bindValue(":beginDate",
                     contract.beginDate.toString(DATE_FORMAT_STR));
    query->bindValue(":endDate",
                     contract.beginDate.toString(DATE_FORMAT_STR));
    query->bindValue(":creatDate",
                     contract.creatDate.toString(DATE_FORMAT_STR));

    query->bindValue(":isIncludeTax", contract.isIncludeTax);

    if (!query->exec()) {
        ALOGE("%s failed!", __FUNCTION__);
        errorno = INSERT_CONTRACT_ITEM_FAIL;
        return INSERT_CONTRACT_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::updateContractTableData(Contract &contract)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("UPDATE contract "
                   "SET clientName=?, "
                   "clientNumber=?, "
                   "projectName=?, "
                   "projectAddress=?, "
                   "requirement=?, "
                   "supplement=?, "
                   "remarks=?, "
                   "creator=?, "
                   "deliverySizes=?, "
                   "structureLevel=?, "
                   "taxRate=?, "
                   "signedDate=?, "
                   "beginDate=?, "
                   "endDate=?, "
                   "creatDate=?, "
                   "isIncludeTax=? "
                   "WHERE number=?;");
    query->addBindValue(contract.clientName);
    query->addBindValue(contract.clientNumber);
    query->addBindValue(contract.projectName);
    query->addBindValue(contract.projectAddress);
    query->addBindValue(contract.requirement);
    query->addBindValue(contract.supplement);
    query->addBindValue(contract.remarks);
    query->addBindValue(contract.creator);
    query->addBindValue(contract.deliverySizes);
    query->addBindValue(contract.structureLevel);
    query->addBindValue(contract.taxRate);
    query->addBindValue(contract.signedDate);
    query->addBindValue(contract.beginDate);
    query->addBindValue(contract.endDate);
    query->addBindValue(contract.creatDate);
    query->addBindValue(contract.isIncludeTax);
    query->addBindValue(contract.number);
    if (!query->exec()) {
        ALOGE("%s fail", __FUNCTION__);
        errorno = UPDATE_CONTRACT_ITEM_FAIL;
        return UPDATE_CONTRACT_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getAllContractData(QList<Contract> &contracts)
{
    Contract contract;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM contract");
    if (!query->exec()) {
        ALOGE("SELECT * FROM contract!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        contract.number = query->value(0).toString();
        contract.clientName = query->value(1).toString();
        contract.clientNumber = query->value(2).toString();
        contract.projectName = query->value(3).toString();
        contract.projectAddress = query->value(4).toString();
        contract.requirement = query->value(5).toString();
        contract.supplement = query->value(6).toString();
        contract.remarks = query->value(7).toString();
        contract.creator = query->value(8).toString();
        contract.deliverySizes = query->value(9).toDouble();
        contract.structureLevel = query->value(10).toDouble();
        contract.taxRate = query->value(11).toInt();

        contract.signedDate = QDate::fromString(query->value(12).toString(),
                                               DATE_FORMAT_STR);
        contract.beginDate = QDate::fromString(query->value(13).toString(),
                                                   DATE_FORMAT_STR);
        contract.endDate = QDate::fromString(query->value(14).toString(),
                                           DATE_FORMAT_STR);
        contract.creatDate = QDate::fromString(query->value(15).toString(),
                                           DATE_FORMAT_STR);

        contract.isIncludeTax = query->value(16).toDouble();

        contracts.push_back(contract);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getContractInClientNumber(QString clientNumber,
                                    QList<Contract> &contracts)
{
    Contract contract;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM contract WHERE clientNumber=?");
    query->addBindValue(clientNumber);
    if (!query->exec()) {
        ALOGE("SELECT * FROM contract!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        contract.number = query->value(0).toString();
        contract.clientName = query->value(1).toString();
        contract.clientNumber = query->value(2).toString();
        contract.projectName = query->value(3).toString();
        contract.projectAddress = query->value(4).toString();
        contract.requirement = query->value(5).toString();
        contract.supplement = query->value(6).toString();
        contract.remarks = query->value(7).toString();
        contract.creator = query->value(8).toString();
        contract.deliverySizes = query->value(9).toDouble();
        contract.structureLevel = query->value(10).toDouble();
        contract.taxRate = query->value(11).toInt();

        contract.signedDate = QDate::fromString(query->value(12).toString(),
                                               DATE_FORMAT_STR);
        contract.beginDate = QDate::fromString(query->value(13).toString(),
                                                   DATE_FORMAT_STR);
        contract.endDate = QDate::fromString(query->value(14).toString(),
                                           DATE_FORMAT_STR);
        contract.creatDate = QDate::fromString(query->value(15).toString(),
                                           DATE_FORMAT_STR);

        contract.isIncludeTax = query->value(16).toDouble();

        contracts.push_back(contract);                              //插入list
    }

    return SUCCESS;
}

bool
DataBase::isClientHasContract(QString clientNumber)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM contract WHERE clientNumber=?");
    query->addBindValue(clientNumber);
    if (!query->exec()) {
        ALOGE("exec [SELECT * FROM contract WHERE clientNumber=%s] failed!",
              clientNumber.toStdString().data());
        errorno = SELECT_DATABASE_FAIL;
        return false;
    }

    if (!query->next())
        return false;

    return true;
}

int
DataBase::getContractInNumber(QString number, Contract &contract)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM contract WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGE("[%s]: select contract table failed!");
        errorno = SELECT_DATABASE_FAIL;
        return SELECT_DATABASE_FAIL;
    }

    if (query->next()) {
        contract.number = query->value(0).toString();
        contract.clientName = query->value(1).toString();
        contract.clientNumber = query->value(2).toString();
        contract.projectName = query->value(3).toString();
        contract.projectAddress = query->value(4).toString();
        contract.requirement = query->value(5).toString();
        contract.supplement = query->value(6).toString();
        contract.remarks = query->value(7).toString();
        contract.creator = query->value(8).toString();
        contract.deliverySizes = query->value(9).toDouble();
        contract.structureLevel = query->value(10).toDouble();
        contract.taxRate = query->value(11).toInt();

        contract.signedDate = QDate::fromString(query->value(12).toString(),
                                               DATE_FORMAT_STR);
        contract.beginDate = QDate::fromString(query->value(13).toString(),
                                                   DATE_FORMAT_STR);
        contract.endDate = QDate::fromString(query->value(14).toString(),
                                           DATE_FORMAT_STR);
        contract.creatDate = QDate::fromString(query->value(15).toString(),
                                           DATE_FORMAT_STR);

        contract.isIncludeTax = query->value(16).toDouble();

        return SUCCESS;
    }

    return DATABASE_ITEM_NOT_EXIST;
}

int
DataBase::clearContractTable()
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    //删除操作
    query->finish();
    query->prepare("DELETE FROM contract;");
    if (!query->exec()) {
        ALOGE("%s fail!", __FUNCTION__);
        errorno = DELETE_TABLE_FAIL;
        return DELETE_TABLE_FAIL;
    }

    ALOGV("%s success!", __FUNCTION__);
    return SUCCESS;
}

int
DataBase::deleteContractDataInNumber(QString number)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM contract WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM contract failed!", __FUNCTION__);
        return DELETE_CONTRACT_ITEM_FAIL;
    }

    return SUCCESS;
}

bool
DataBase::isContractExist(const Contract &contract)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM contract WHERE number=?");
    query->addBindValue(contract.number);
    if (!query->exec()) {
        ALOGE("exec [SELECT * FROM contract WHERE number=%s] failed!",
              contract.number.toStdString().data());
        errorno = SELECT_DATABASE_FAIL;
        return false;
    }

    if (!query->next())
        return false;

    return true;
}

int
DataBase::getAllContractPriceData(const QString contractNumber,
                                  QList<CONTRACT_PRICE> &prices)
{
    CONTRACT_PRICE price;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM contract_price WHERE contractNumber=?");
    query->addBindValue(contractNumber);
    if (!query->exec()) {
        ALOGE("SELECT * FROM contract_price!");
        return SELECT_DATABASE_FAIL;
    }

    if (!prices.isEmpty())
        prices.clear();

    while (query->next()) {
        price.number = query->value(0).toString();
        price.contractNumber = query->value(1).toString();
        price.remarks = query->value(2).toString();
        price.pumpType = Car::getPumpType(query->value(3).toInt());
        price.squarePrice = query->value(4).toDouble();
        price.standardPrice = query->value(5).toDouble();

        prices.push_back(price);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::insertContractPriceTable(const CONTRACT_PRICE &price)
{
    ALOGDTRACE();
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    ALOGD("price.number = %s, price.contractNumber = %s, price.remarks = %s"
          "price.pumpType = %s, price.squarePrice = %f, price.standardPricee = %f",
          price.number.toStdString().data(),
          price.contractNumber.toStdString().data(),
          price.remarks.toStdString().data(),
          Car::getPumpTypeStr(price.pumpType).toStdString().data(),
          price.squarePrice, price.standardPrice);

    query->finish();
    query->prepare("INSERT INTO contract_price "
                   "VALUES(:number, :contractNumber, :remarks, "
                   ":pumpType, :squarePrice, :standardPrice)");
    query->bindValue(":number", price.number);
    query->bindValue(":contractNumber", price.contractNumber);
    query->bindValue(":remarks", price.remarks);
    query->bindValue(":pumpType", price.pumpType);
    query->bindValue(":squarePrice", price.squarePrice);
    query->bindValue(":standardPrice", price.standardPrice);

    if (!query->exec()) {
        ALOGE("%s failed!", __FUNCTION__);
        errorno = INSERT_PRICE_ITEM_FAIL;
        return INSERT_PRICE_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::delContractPriceInNumber(const QString number)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM contract_price WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM contract_price failed!", __FUNCTION__);
        return DELETE_PRICE_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::delContractPriceInContractNumber(const QString contractNumber)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM contract_price WHERE contractNumber=?");
    query->addBindValue(contractNumber);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM contract_price failed!", __FUNCTION__);
        return DELETE_PRICE_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::insertRentalDocumentTable(const RentalDocument &doc)
{

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("INSERT INTO rentaldocument "
                   "VALUES(:number, :clientName, :clientNumber, "
                   ":contractNumber, :carNumber, :carPlateNumber, "
                   ":contructPlace, :concreteLable, :principal, "
                   ":principalTel, :driver1, :driver2, "
                   ":driver3, :projectName, :projectAddress, "
                   ":remarks, :beginFuel, :endFuel, "
                   ":projectAmount, :receivedAccounts, :pumpSquare, :squareUnitPrice, "
                   ":pumpTimes, :pumpTimeUnitPrice, :workingHours, "
                   ":date, :arrivalDateTime, :leaveDateTime, "
                   ":rentalDocState, :pumpType)");
    query->bindValue(":number", doc.number);
    query->bindValue(":clientName", doc.clientName);
    query->bindValue(":clientNumber", doc.clientNumber);
    query->bindValue(":contractNumber", doc.contractNumber);
    query->bindValue(":carNumber", doc.carNumber);
    query->bindValue(":carPlateNumber", doc.carPlateNumber);
    query->bindValue(":contructPlace", doc.constructPlace);
    query->bindValue(":concreteLable", doc.concreteLable);
    query->bindValue(":principal", doc.principal);
    query->bindValue(":principalTel", doc.principalTel);
    query->bindValue(":driver1", doc.driver1);
    query->bindValue(":driver2", doc.driver2);
    query->bindValue(":driver3", doc.driver3);
    query->bindValue(":projectName", doc.projectName);
    query->bindValue(":projectAddress", doc.projectAddress);
    query->bindValue(":remarks", doc.remarks);
    query->bindValue(":beginFuel", doc.beginFuel);
    query->bindValue(":endFuel", doc.endFuel);
    query->bindValue(":projectAmount", doc.projectAmount);
    query->bindValue(":receivedAccounts", doc.receivedAccounts);
    query->bindValue(":pumpSquare", doc.pumpSquare);
    query->bindValue(":squareUnitPrice", doc.squareUnitPrice);
    query->bindValue(":pumpTimes", doc.pumpTimes);
    query->bindValue(":pumpTimeUnitPrice", doc.pumpTimeUnitPrice);
    query->bindValue(":workingHours", doc.workingHours);

    query->bindValue(":date",
                     doc.date.toString(DATE_FORMAT_STR));
    query->bindValue(":arrivalDateTime",
                     doc.arrivalDateTime.toString(DATETIME_FORMAT_STR));
    query->bindValue(":leaveDateTime",
                     doc.leaveDateTime.toString(DATETIME_FORMAT_STR));
    query->bindValue(":rentalDocState",
                     (int)doc.state);
    query->bindValue(":pumpType", (int)doc.pumpType);

    if (!query->exec()) {
        ALOGE("%s failed!", __FUNCTION__);
        errorno = INSERT_DOC_ITEM_FAIL;
        return INSERT_DOC_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::updateRentalDocumentData(const RentalDocument &doc)
{
    ALOGDTRACE();
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("UPDATE rentaldocument "
                   "SET clientName=?, "
                   "clientNumber=?, "
                   "contractNumber=?, "
                   "carNumber=?, "
                   "carPlateNumber=?, "
                   "contructPlace=?, "
                   "concreteLable=?, "
                   "principal=?, "
                   "principalTel=?, "
                   "driver1=?, "
                   "driver2=?, "
                   "driver3=?, "
                   "projectName=?, "
                   "projectAddress=?, "
                   "remarks=?, "
                   "beginFuel=?, "
                   "endFuel=?, "
                   "projectAmount=?, "
                   "receivedAccounts=?, "
                   "pumpSquare=?, "
                   "squareUnitPrice=?, "
                   "pumpTimes=?, "
                   "pumpTimeUnitPrice=?, "
                   "workingHours=?, "
                   "date=?, "
                   "arrivalDateTime=?, "
                   "leaveDateTime=?, "
                   "rentalDocState=?, "
                   "pumpType=? "
                   "WHERE number=?;");
    query->addBindValue(doc.clientName);
    query->addBindValue(doc.clientNumber);
    query->addBindValue(doc.contractNumber);
    query->addBindValue(doc.carNumber);
    query->addBindValue(doc.carPlateNumber);
    query->addBindValue(doc.constructPlace);
    query->addBindValue(doc.concreteLable);
    query->addBindValue(doc.principal);
    query->addBindValue(doc.principalTel);
    query->addBindValue(doc.driver1);
    query->addBindValue(doc.driver2);
    query->addBindValue(doc.driver3);
    query->addBindValue(doc.projectName);
    query->addBindValue(doc.projectAddress);
    query->addBindValue(doc.remarks);
    query->addBindValue(doc.beginFuel);
    query->addBindValue(doc.endFuel);
    query->addBindValue(doc.projectAmount);
    query->addBindValue(doc.receivedAccounts);
    query->addBindValue(doc.pumpSquare);
    query->addBindValue(doc.squareUnitPrice);
    query->addBindValue(doc.pumpTimes);
    query->addBindValue(doc.pumpTimeUnitPrice);
    query->addBindValue(doc.workingHours);
    query->addBindValue(doc.date.toString(DATE_FORMAT_STR));
    query->addBindValue(doc.arrivalDateTime.toString(DATETIME_FORMAT_STR));
    query->addBindValue(doc.leaveDateTime.toString(DATETIME_FORMAT_STR));
    query->addBindValue(doc.state);
    query->addBindValue(doc.pumpType);
    query->addBindValue(doc.number);
    if (!query->exec()) {
        ALOGE("%s fail", __FUNCTION__);
        errorno = UPDATE_DOC_ITEM_FAIL;
        return UPDATE_DOC_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getAllRentalDocumentData(QList<RentalDocument> &docs)
{
    RentalDocument doc;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM rentaldocument");
    if (!query->exec()) {
        ALOGE("SELECT * FROM rentaldocument!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        doc.number = query->value(0).toString();
        doc.clientName = query->value(1).toString();
        doc.clientNumber = query->value(2).toString();
        doc.contractNumber = query->value(3).toString();
        doc.carNumber = query->value(4).toString();
        doc.carPlateNumber = query->value(5).toString();
        doc.constructPlace = query->value(6).toString();
        doc.concreteLable = query->value(7).toString();
        doc.principal = query->value(8).toString();
        doc.principalTel = query->value(9).toString();
        doc.driver1 = query->value(10).toString();
        doc.driver2 = query->value(11).toString();
        doc.driver3 = query->value(12).toString();
        doc.projectName = query->value(13).toString();
        doc.projectAddress = query->value(14).toString();
        doc.remarks = query->value(15).toString();

        doc.beginFuel = query->value(16).toDouble();
        doc.endFuel = query->value(17).toDouble();
        doc.projectAmount = query->value(18).toDouble();
        doc.receivedAccounts = query->value(19).toDouble();
        doc.pumpSquare = query->value(20).toDouble();
        doc.squareUnitPrice = query->value(21).toDouble();
        doc.pumpTimes = query->value(22).toDouble();
        doc.pumpTimeUnitPrice = query->value(23).toDouble();
        doc.workingHours = query->value(24).toDouble();

        doc.date = QDate::fromString(query->value(25).toString(),
                                               DATE_FORMAT_STR);
        doc.arrivalDateTime = QDateTime::fromString(query->value(26).toString(),
                                                    DATETIME_FORMAT_STR);
        doc.leaveDateTime = QDateTime::fromString(query->value(27).toString(),
                                                  DATETIME_FORMAT_STR);
        doc.state = (RentalDocState)query->value(28).toInt();
        doc.pumpType = (PumpType)query->value(29).toInt();

        docs.push_back(doc);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getRentalDocInClientNumber(const QString clientNumber,
                                     QList<RentalDocument> &docs)
{
    RentalDocument doc;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM rentaldocument WHERE clientNumber=?");
    query->addBindValue(clientNumber);
    if (!query->exec()) {
        ALOGE("SELECT * FROM rentaldocument!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        doc.number = query->value(0).toString();
        doc.clientName = query->value(1).toString();
        doc.clientNumber = query->value(2).toString();
        doc.contractNumber = query->value(3).toString();
        doc.carNumber = query->value(4).toString();
        doc.carPlateNumber = query->value(5).toString();
        doc.constructPlace = query->value(6).toString();
        doc.concreteLable = query->value(7).toString();
        doc.principal = query->value(8).toString();
        doc.principalTel = query->value(9).toString();
        doc.driver1 = query->value(10).toString();
        doc.driver2 = query->value(11).toString();
        doc.driver3 = query->value(12).toString();
        doc.projectName = query->value(13).toString();
        doc.projectAddress = query->value(14).toString();
        doc.remarks = query->value(15).toString();

        doc.beginFuel = query->value(16).toDouble();
        doc.endFuel = query->value(17).toDouble();
        doc.projectAmount = query->value(18).toDouble();
        doc.receivedAccounts = query->value(19).toDouble();
        doc.pumpSquare = query->value(20).toDouble();
        doc.squareUnitPrice = query->value(21).toDouble();
        doc.pumpTimes = query->value(22).toDouble();
        doc.pumpTimeUnitPrice = query->value(23).toDouble();
        doc.workingHours = query->value(24).toDouble();

        doc.date = QDate::fromString(query->value(25).toString(),
                                               DATE_FORMAT_STR);
        doc.arrivalDateTime = QDateTime::fromString(query->value(26).toString(),
                                                   "yyyy-MM-dd hh:mm:ss");
        doc.leaveDateTime = QDateTime::fromString(query->value(27).toString(),
                                           "yyyy-MM-dd hh:mm:ss");
        doc.state = (RentalDocState)query->value(28).toInt();
        doc.pumpType = (PumpType)query->value(29).toInt();

        docs.push_back(doc);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::clearRentalDocumentTable()
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query) {
        exit (GET_DATABASE_FAIL);
    }

    //删除操作
    query->finish();
    query->prepare("DELETE FROM rentaldocument;");
    if (!query->exec()) {
        ALOGE("%s fail!", __FUNCTION__);
        errorno = DELETE_TABLE_FAIL;
        return DELETE_TABLE_FAIL;
    }

    ALOGV("%s success!", __FUNCTION__);
    return SUCCESS;
}

int
DataBase::deleteRentalDocumentInNumber(const QString number)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("DELETE FROM rentaldocument WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGD("%s, DELETE FROM rentaldocument failed!", __FUNCTION__);
        return DELETE_DOC_ITEM_FAIL;
    }

    return SUCCESS;
}

int
DataBase::getRentalDocumentDataInNumber(QString number, RentalDocument &doc)
{
    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM rentaldocument WHERE number=?");
    query->addBindValue(number);
    if (!query->exec()) {
        ALOGE("SELECT * FROM rentaldocument fail!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        doc.number = query->value(0).toString();
        doc.clientName = query->value(1).toString();
        doc.clientNumber = query->value(2).toString();
        doc.contractNumber = query->value(3).toString();
        doc.carNumber = query->value(4).toString();
        doc.carPlateNumber = query->value(5).toString();
        doc.constructPlace = query->value(6).toString();
        doc.concreteLable = query->value(7).toString();
        doc.principal = query->value(8).toString();
        doc.principalTel = query->value(9).toString();
        doc.driver1 = query->value(10).toString();
        doc.driver2 = query->value(11).toString();
        doc.driver3 = query->value(12).toString();
        doc.projectName = query->value(13).toString();
        doc.projectAddress = query->value(14).toString();
        doc.remarks = query->value(15).toString();

        doc.beginFuel = query->value(16).toDouble();
        doc.endFuel = query->value(17).toDouble();
        doc.projectAmount = query->value(18).toDouble();
        doc.receivedAccounts = query->value(19).toDouble();
        doc.pumpSquare = query->value(20).toDouble();
        doc.squareUnitPrice = query->value(21).toDouble();
        doc.pumpTimes = query->value(22).toDouble();
        doc.pumpTimeUnitPrice = query->value(23).toDouble();
        doc.workingHours = query->value(24).toDouble();

        doc.date = QDate::fromString(query->value(25).toString(),
                                               DATE_FORMAT_STR);
        doc.arrivalDateTime = QDateTime::fromString(query->value(26).toString(),
                                                   DATETIME_FORMAT_STR);
        doc.leaveDateTime = QDateTime::fromString(query->value(27).toString(),
                                           DATETIME_FORMAT_STR);
        doc.state = (RentalDocState)query->value(28).toInt();
        doc.pumpType = (PumpType)query->value(29).toInt();
    }

    return SUCCESS;
}

int
DataBase::getRentalDocInStateAndClientNum(const QString clientNumber,
                                          const RentalDocState state,
                                          QList<RentalDocument> &docs)
{
    RentalDocument doc;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM rentaldocument WHERE clientNumber=? AND rentalDocState=?");
    query->addBindValue(clientNumber);
    query->addBindValue(state);
    if (!query->exec()) {
        ALOGE("SELECT * FROM rentaldocument!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        doc.number = query->value(0).toString();
        doc.clientName = query->value(1).toString();
        doc.clientNumber = query->value(2).toString();
        doc.contractNumber = query->value(3).toString();
        doc.carNumber = query->value(4).toString();
        doc.carPlateNumber = query->value(5).toString();
        doc.constructPlace = query->value(6).toString();
        doc.concreteLable = query->value(7).toString();
        doc.principal = query->value(8).toString();
        doc.principalTel = query->value(9).toString();
        doc.driver1 = query->value(10).toString();
        doc.driver2 = query->value(11).toString();
        doc.driver3 = query->value(12).toString();
        doc.projectName = query->value(13).toString();
        doc.projectAddress = query->value(14).toString();
        doc.remarks = query->value(15).toString();

        doc.beginFuel = query->value(16).toDouble();
        doc.endFuel = query->value(17).toDouble();
        doc.projectAmount = query->value(18).toDouble();
        doc.receivedAccounts = query->value(19).toDouble();
        doc.pumpSquare = query->value(20).toDouble();
        doc.squareUnitPrice = query->value(21).toDouble();
        doc.pumpTimes = query->value(22).toDouble();
        doc.pumpTimeUnitPrice = query->value(23).toDouble();
        doc.workingHours = query->value(24).toDouble();

        doc.date = QDate::fromString(query->value(25).toString(),
                                               DATE_FORMAT_STR);
        doc.arrivalDateTime = QDateTime::fromString(query->value(26).toString(),
                                                   "yyyy-MM-dd hh:mm:ss");
        doc.leaveDateTime = QDateTime::fromString(query->value(27).toString(),
                                           "yyyy-MM-dd hh:mm:ss");
        doc.state = (RentalDocState)query->value(28).toInt();
        doc.pumpType = (PumpType)query->value(29).toInt();

        docs.push_back(doc);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getAllRenDocInState(const RentalDocState state, QList<RentalDocument> &docs)
{
    RentalDocument doc;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM rentaldocument WHERE rentalDocState=?");
    query->addBindValue(state);
    if (!query->exec()) {
        ALOGE("SELECT * FROM rentaldocument WHERE rentalDocState=%d error",
              (int)state);
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        doc.number = query->value(0).toString();
        doc.clientName = query->value(1).toString();
        doc.clientNumber = query->value(2).toString();
        doc.contractNumber = query->value(3).toString();
        doc.carNumber = query->value(4).toString();
        doc.carPlateNumber = query->value(5).toString();
        doc.constructPlace = query->value(6).toString();
        doc.concreteLable = query->value(7).toString();
        doc.principal = query->value(8).toString();
        doc.principalTel = query->value(9).toString();
        doc.driver1 = query->value(10).toString();
        doc.driver2 = query->value(11).toString();
        doc.driver3 = query->value(12).toString();
        doc.projectName = query->value(13).toString();
        doc.projectAddress = query->value(14).toString();
        doc.remarks = query->value(15).toString();

        doc.beginFuel = query->value(16).toDouble();
        doc.endFuel = query->value(17).toDouble();
        doc.projectAmount = query->value(18).toDouble();
        doc.receivedAccounts = query->value(19).toDouble();
        doc.pumpSquare = query->value(20).toDouble();
        doc.squareUnitPrice = query->value(21).toDouble();
        doc.pumpTimes = query->value(22).toDouble();
        doc.pumpTimeUnitPrice = query->value(23).toDouble();
        doc.workingHours = query->value(24).toDouble();

        doc.date = QDate::fromString(query->value(25).toString(),
                                               DATE_FORMAT_STR);
        doc.arrivalDateTime = QDateTime::fromString(query->value(26).toString(),
                                                   "yyyy-MM-dd hh:mm:ss");
        doc.leaveDateTime = QDateTime::fromString(query->value(27).toString(),
                                           "yyyy-MM-dd hh:mm:ss");
        doc.state = (RentalDocState)query->value(28).toInt();
        doc.pumpType = (PumpType)query->value(29).toInt();

        docs.push_back(doc);                              //插入list
    }

    return SUCCESS;
}

int
DataBase::getRentalDocumentDataInCarNumber(const QString carNumber, QList<RentalDocument> &docs)
{
    RentalDocument doc;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM rentaldocument WHERE carNumber=?");
    query->addBindValue(carNumber);
    if (!query->exec()) {
        ALOGE("SELECT * FROM rentaldocument!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        doc.number = query->value(0).toString();
        doc.clientName = query->value(1).toString();
        doc.clientNumber = query->value(2).toString();
        doc.contractNumber = query->value(3).toString();
        doc.carNumber = query->value(4).toString();
        doc.carPlateNumber = query->value(5).toString();
        doc.constructPlace = query->value(6).toString();
        doc.concreteLable = query->value(7).toString();
        doc.principal = query->value(8).toString();
        doc.principalTel = query->value(9).toString();
        doc.driver1 = query->value(10).toString();
        doc.driver2 = query->value(11).toString();
        doc.driver3 = query->value(12).toString();
        doc.projectName = query->value(13).toString();
        doc.projectAddress = query->value(14).toString();
        doc.remarks = query->value(15).toString();

        doc.beginFuel = query->value(16).toDouble();
        doc.endFuel = query->value(17).toDouble();
        doc.projectAmount = query->value(18).toDouble();
        doc.receivedAccounts = query->value(19).toDouble();
        doc.pumpSquare = query->value(20).toDouble();
        doc.squareUnitPrice = query->value(21).toDouble();
        doc.pumpTimes = query->value(22).toDouble();
        doc.pumpTimeUnitPrice = query->value(23).toDouble();
        doc.workingHours = query->value(24).toDouble();

        doc.date = QDate::fromString(query->value(25).toString(),
                                               DATE_FORMAT_STR);
        doc.arrivalDateTime = QDateTime::fromString(query->value(26).toString(),
                                                   "yyyy-MM-dd hh:mm:ss");
        doc.leaveDateTime = QDateTime::fromString(query->value(27).toString(),
                                           "yyyy-MM-dd hh:mm:ss");
        doc.state = (RentalDocState)query->value(28).toInt();
        doc.pumpType = (PumpType)query->value(29).toInt();

        docs.push_back(doc);                              //插入list
    }

    return SUCCESS;
}

QString
DataBase::rentalDocFilterToSqlStr(RECEIPT_FILTER filter)
{
    QString docNumStr, carStr, clientStr, contractNumStr;
    QString dateStr, pumpTypeStr, isAccountPositiveStr;
    QString sqlStr = "SELECT * FROM rentaldocument WHERE ";
    QString fromDateStr = filter.fromDate.toString(DATE_FORMAT_STR);
    QString toDateStr = filter.toDate.toString(DATE_FORMAT_STR);
    bool isPreNull;

    if (fromDateStr != NULL && toDateStr != NULL) {
        dateStr = " date BETWEEN '" + fromDateStr + "' AND '" + toDateStr + "'";
    } else if (fromDateStr != NULL && toDateStr == NULL)
        dateStr = " date >='" + fromDateStr + "'";
    else if (fromDateStr == NULL && toDateStr != NULL)
        dateStr = " date <='" + toDateStr + "'";
    if (dateStr != NULL) {
        sqlStr += dateStr;
        isPreNull = false;
    } else
        isPreNull = true;

    if (filter.pumpType != PumpType::UNKNOWN_PUMPTYPE)
        pumpTypeStr = " pumpType='" + QString::number((int)filter.pumpType) + "'";
    if (pumpTypeStr != NULL) {
        if (!isPreNull)
            sqlStr += " AND " + pumpTypeStr;
        else
            sqlStr += pumpTypeStr;
        isPreNull = false;
    }

    if (filter.isAccountPositive)
        isAccountPositiveStr = " projectAmount-receivedAccounts > 0";
    if (isAccountPositiveStr != NULL) {
        if (!isPreNull)
            sqlStr += " AND " + isAccountPositiveStr;
        else
            sqlStr += isAccountPositiveStr;
        isPreNull = false;
    }

    if (filter.carNumber != NULL)
        carStr = " carNumber='" + filter.carNumber + "'";
    if (carStr != NULL) {
        if (!isPreNull)
            sqlStr += " AND " + carStr;
        else
            sqlStr += carStr;
        isPreNull = false;
    }

    if (filter.clientName != NULL)
        clientStr = " clientName='" + filter.clientName + "'";
    if (clientStr != NULL) {
        if (!isPreNull)
            sqlStr += " AND " + clientStr;
        else
            sqlStr += clientStr;
        isPreNull = false;
    }

    if (filter.contractNumber != NULL)
        contractNumStr = " contractNumber='" + filter.contractNumber + "'";
    if (contractNumStr != NULL) {
        if (!isPreNull)
            sqlStr += " AND " + contractNumStr;
        else
            sqlStr += contractNumStr;
        isPreNull = false;
    }

    if (filter.rentalDocNumber != NULL)
        docNumStr = " number='" + filter.rentalDocNumber + "'";
    if (docNumStr != NULL) {
        if (!isPreNull)
            sqlStr += " AND " + docNumStr;
        else
            sqlStr += docNumStr;
        isPreNull = false;
    }

    ALOGD("%s fromStr = %s, toStr = %s", __FUNCTION__,
          fromDateStr.toStdString().data(), toDateStr.toStdString().data());
    ALOGD("%s sqlStr = %s", __FUNCTION__, sqlStr.toStdString().data());
    return sqlStr;
}

int
DataBase::getRentalDocInFilter(RECEIPT_FILTER filter, QList<RentalDocument> &docs)
{
    RentalDocument doc;

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare(rentalDocFilterToSqlStr(filter));
    if (!query->exec()) {
        ALOGE("SELECT * FROM rentaldocument in filter failed!");
        return SELECT_DATABASE_FAIL;
    }

    while (query->next()) {
        doc.number = query->value(0).toString();
        doc.clientName = query->value(1).toString();
        doc.clientNumber = query->value(2).toString();
        doc.contractNumber = query->value(3).toString();
        doc.carNumber = query->value(4).toString();
        doc.carPlateNumber = query->value(5).toString();
        doc.constructPlace = query->value(6).toString();
        doc.concreteLable = query->value(7).toString();
        doc.principal = query->value(8).toString();
        doc.principalTel = query->value(9).toString();
        doc.driver1 = query->value(10).toString();
        doc.driver2 = query->value(11).toString();
        doc.driver3 = query->value(12).toString();
        doc.projectName = query->value(13).toString();
        doc.projectAddress = query->value(14).toString();
        doc.remarks = query->value(15).toString();

        doc.beginFuel = query->value(16).toDouble();
        doc.endFuel = query->value(17).toDouble();
        doc.projectAmount = query->value(18).toDouble();
        doc.receivedAccounts = query->value(19).toDouble();
        doc.pumpSquare = query->value(20).toDouble();
        doc.squareUnitPrice = query->value(21).toDouble();
        doc.pumpTimes = query->value(22).toDouble();
        doc.pumpTimeUnitPrice = query->value(23).toDouble();
        doc.workingHours = query->value(24).toDouble();

        doc.date = QDate::fromString(query->value(25).toString(),
                                               DATE_FORMAT_STR);
        doc.arrivalDateTime = QDateTime::fromString(query->value(26).toString(),
                                                   "yyyy-MM-dd hh:mm:ss");
        doc.leaveDateTime = QDateTime::fromString(query->value(27).toString(),
                                           "yyyy-MM-dd hh:mm:ss");
        doc.state = (RentalDocState)query->value(28).toInt();
        doc.pumpType = (PumpType)query->value(29).toInt();

        docs.push_back(doc);                              //插入list
    }

    return SUCCESS;
}

bool
DataBase::isRentalDocumentExist(RentalDocument &doc)
{

    QMutexLocker locker(pmMutex);

    QSqlQuery *query = getDataBaseQuery();
    if (!query)
        exit GET_DATABASE_FAIL;

    query->finish();
    query->prepare("SELECT * FROM rentaldocument WHERE number=?");
    query->addBindValue(doc.number);
    if (!query->exec()) {
        ALOGE("exec [SELECT * FROM contract WHERE number=%s] failed!",
              doc.number.toStdString().data());
        errorno = SELECT_DATABASE_FAIL;
        return false;
    }

    if (!query->next())
        return false;

    return true;
}
