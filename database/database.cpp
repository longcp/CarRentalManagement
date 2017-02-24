#include "database.h"
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>
#include <client.h>
#include <datatype.h>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>

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
    ALOGD("mDbPath = %s", mDbPath.toStdString().data());
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
                     .createDate.toString("yyyy-MM-dd"));
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
                    PayType::CASH : PayType::MONTHLY;
        client.clienttype = query->value(10).toInt() ?
                    ClientType::CONTRACT : ClientType::TEMPORARY;
        client.createDate = QDate::fromString(query->value(11)
                                              .toString(), "yyyy-MM-dd");
        client.monthly = query->value(12).toInt();
        client.amount = query->value(13).toFloat();
        client.paid = query->value(14).toFloat();

        return SUCCESS;
    }

    return DATABASE_ITEM_NOT_EXIST;
}

int
DataBase::getAllClientsNumber(QList<QString> &numbers)
{
    ALOGD("getAllClientsNumber enter");
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
                    PayType::CASH : PayType::MONTHLY;
        client.clienttype = query->value(10).toInt() ?
                    ClientType::CONTRACT : ClientType::TEMPORARY;
        client.createDate = QDate::fromString(query->value(11)
                                              .toString(), "yyyy-MM-dd");
        client.monthly = query->value(12).toInt();
        client.amount = query->value(13).toFloat();
        client.paid = query->value(14).toFloat();

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

    case SELECT_DATABASE_FAIL:
        return "SELECT_DATABASE_FAIL";

    case UPDATE_CLIENT_ITEM_FAIL:
        return "UPDATE_CLIENT_ITEM_FAIL";

    case OK:
    default:
        return "Ok";
        break;
    }

    return "Ok";
}
