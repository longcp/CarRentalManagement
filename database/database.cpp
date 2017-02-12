#include "database.h"
#include <QFile>
#include <QDir>

#define LOG_TAG                         "DATABASE"
#include "utils/Log.h"

DataBase::DataBase()
{
    mDbPath = QDir::currentPath().append("/").append(DB_PATH_SUFFIX);
    openDataBase();
}

DataBase::~DataBase(){}

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
        ALOGE("database[%s] is not existed!", mDbPath.toStdString());
        if (!QFile::copy(mDbPath, mDbPath)) {
            ALOGE("copy database[%s] failed.", mDbPath.toStdString());
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
        ALOGE("open database[%s] failed!", mDbPath.toStdString());
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
