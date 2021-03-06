#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <datatype.h>

class User;

#define DEBUG_VERSION

//#ifdef DEBUG_VERSION
//#define
#define DB_PATH_SUFFIX  "./dataBase/CarRentalManagement.db"
#define DB_NAME         "carrentalmanagement"

class QMutex;
class Client;
class Car;
class Contract;
class RentalDocument;
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
    int                 getClientInFilter(QList<Client> &clients, QString filter);
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
    bool                isCarExist(Car &car);

    // insurance表
    int                 getAllInsuranceData(QList<INSURANCE_RECORD> &records);
    int                 getInsuranceDataInCarNumber(QString carNumber, QList<INSURANCE_RECORD> &records);
    int                 insertInsuranceTable(INSURANCE_RECORD &record);
    int                 delInsuranceDataInNumber(QString number);

    // business insurance表
    int                 getAllBusinessInsuranceData(QList<INSURANCE_RECORD> &records);
    int                 getBusinessInsuranceDataInCarNumber(QString carNumber, QList<INSURANCE_RECORD> &records);
    int                 insertBusinessInsuranceTable(INSURANCE_RECORD &record);
    int                 delBusinessInsuranceDataInNumber(QString number);

    // annual
    int                 getAllAnnualData(QList<ANNUALFEE_RECORD> &records);
    int                 getAnnualDataInCarNumber(QString carNumber, QList<ANNUALFEE_RECORD> &records);
    int                 insertAnnualTable(ANNUALFEE_RECORD &record);
    int                 delAnnualDataInNumber(QString number);

    // project表
    int                 getAllProjectData(QList<PROJECT_RECORD> &records);
    int                 getProjectDataInCarNumber(QString carNumber, QList<PROJECT_RECORD> &records);
    int                 insertProjectTable(PROJECT_RECORD &record);
    int                 delProjectDataInNumber(QString number);

    // contract表
    int                 insertContractTable(Contract &contract);
    int                 updateContractTableData(Contract &contract);
    int                 getAllContractData(QList<Contract> &contracts);
    int                 getContractInClientNumber(QString clientNumber,
                                                  QList<Contract> &contracts);
    bool                isClientHasContract(QString clientNumber);
    bool                isClientHasRentalDoc(QString clientNumber);
    int                 getContractInNumber(QString number,
                                            Contract &contract);
    int                 clearContractTable();
    int                 deleteContractDataInNumber(QString number);
    bool                isContractExist(const Contract &contract);

    // contract_price表
    int                 getAllContractPriceData(const QString contractNumber,
                                                QList<CONTRACT_PRICE> &prices);
    int                 getPriceInNumber(const QString number, CONTRACT_PRICE &price);
    int                 insertContractPriceTable(const CONTRACT_PRICE &price);
    int                 delContractPriceInNumber(const QString number);
    int                 delContractPriceInContractNumber(const QString contractNumber);

    // rentaldocument表
    int                 insertRentalDocumentTable(const RentalDocument &doc);
    int                 updateRentalDocumentData(const RentalDocument &doc);
    int                 getAllRentalDocumentData(QList<RentalDocument> &docs);
    int                 getRentalDocInClientNumber(const QString clientNumber,
                                                   QList<RentalDocument> &docs);
    int                 getRentalDocumentDataInCarNumber(const QString carNumber,
                                                         QList<RentalDocument> &docs);
    int                 getRentalDocInStateAndClientNum(const QString clientNumber,
                                                        const RentalDocState state,
                                                        QList<RentalDocument> &docs);
    int                 getAllRenDocInState(const RentalDocState state, QList<RentalDocument> &docs);
    int                 clearRentalDocumentTable();
    int                 deleteRentalDocumentInNumber(const QString number);
    int                 getRentalDocumentDataInNumber(const QString number, RentalDocument &doc);
    int                 getRentalDocInFilter(RECEIPT_FILTER filter, QList<RentalDocument> &docs);
    bool                isRentalDocumentExist(RentalDocument &doc);
    QString             rentalDocFilterToSqlStr(RECEIPT_FILTER filter);

    QString             getMaxNumber(QString table, QString field);
    QString             getTableName(DataBaseTable table);

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
