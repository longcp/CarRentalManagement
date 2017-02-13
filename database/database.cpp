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

#define LOG_TAG                         "DATABASE"
#include "utils/Log.h"

DataBase::DataBase() :
    pmMutex(new QMutex(QMutex::Recursive)),
    pmQuery(new QMap<int, QSqlQuery*>)
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
        return DELETE_TABLE_FAIL;
    }

    ALOGV("clearClientTable success!");
    return SUCCESS;
}
