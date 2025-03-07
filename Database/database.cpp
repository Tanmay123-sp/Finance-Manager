#include "database.h"
using namespace std ;
database* database::instance = nullptr;

database::database()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("finance.db");
    if(db.open()) qDebug()<<"Connection Successfull";
    else qDebug()<<"Connection Failed";
    createTable();
}


database * database::getInstance(){
    if(instance == nullptr) instance = new database();
    return instance;
}

database::~database()
{
    delete instance;
    if(db.isOpen()) db.close();
}

bool database::createTable()
{
    bool success = true;
    QSqlQuery query;

    // Create userInfo table
    query.prepare("CREATE TABLE IF NOT EXISTS userInfo ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "user TEXT UNIQUE NOT NULL,"
                  "pass TEXT NOT NULL"
                  ");");
    if (!query.exec()) {
        success = false;
        qDebug() << "userInfo table creation failed: " << query.lastError().text();
    }

    query.prepare("CREATE TABLE IF NOT EXISTS transactionTable ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "sender TEXT NOT NULL,"
                  "receiver TEXT NOT NULL,"
                  "amount INTEGER NOT NULL,"
                  "timestamp TEXT DEFAULT (strftime('%Y-%m-%d %H:%M:%S', 'now', 'localtime'))"
                  ");");
    if (!query.exec()) {
        success = false;
        qDebug() << "transactionTable creation failed: " << query.lastError().text();
    }

    return success;
}
bool database::setUserDetails(QVector<QString>v)
{

    qDebug()<<v[0]<<" === "<<v[1];
    QSqlQuery query;
    query.prepare("INSERT INTO userInfo(user,pass) VALUES (:user,:pass) ;");
    query.bindValue(":user",v[0]);
    query.bindValue(":pass",v[1]);
    bool success = query.exec();
    if(!success) {
        qDebug()<<"user and pass values doesn,t get inserted into userInfo table ";
    }
    qDebug()<<"user and pass values are set to DB";
    return success;
}

void database::updateTransactionDetails(const QString sender, const QString receiver, const QString amt)
{
    QSqlQuery query;
    query.prepare("INSERT INTO transactionTable (sender, receiver, amount) "
                  "VALUES (:sender, :receiver, :amount);");
    query.bindValue(":sender", sender);
    query.bindValue(":receiver", receiver);
    query.bindValue(":amount", amt);

    if (!query.exec()) {
        qDebug() << "Failed to insert transaction details: " << query.lastError().text();
    }
    qDebug() << "Transaction inserted successfully.";
}

QVector<QString> database::getUserAndPass(QString userName)
{
    QVector<QString>userDetails;
    QSqlQuery query;
    qDebug()<<"getuser"<<userName;
    query.prepare("SELECT user,pass FROM userInfo WHERE user = :userName");
    query.bindValue(":userName", userName);
    qDebug()<<"getuser1";
    if(query.exec() && query.next()){
        userDetails.append(query.value(0).toString());
        userDetails.append(query.value(1).toString());

        qDebug()<<"dbuser"<<userDetails[0];
    }
    return userDetails;
}

QVector<pair<QString, pair<QString, pair<QString, QString>>>> database::getTransactionDetails(QString userName)
{
    QVector<pair<QString, pair<QString, pair<QString, QString>>>> transactionDetails;
    QSqlQuery query;
    query.prepare("SELECT sender, receiver, amount, timestamp FROM transactionTable WHERE sender = :userName OR receiver = :userName");
    query.bindValue(":userName", userName);

    if(query.exec()) {
        while(query.next()) {
            QString sender = query.value(0).toString();
            QString receiver = query.value(1).toString();
            QString amount = query.value(2).toString();
            QString timestamp = query.value(3).toString();

            transactionDetails.push_back({sender, {receiver, {amount, timestamp}}});
            qDebug() << "Sender: " << sender
                     << ", Receiver: " << receiver
                     << ", Amount: " << amount
                     << ", Timestamp: " << timestamp;
        }
    } else {
        qDebug() << "Query execution failed.";
    }
    return transactionDetails;
}
