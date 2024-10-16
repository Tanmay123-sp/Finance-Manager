#include "database.h"

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
    query.prepare("CREATE TABLE IF NOT EXISTS userInfo("
                    "user TEXT PRIMARY KEY,"
                    "pass TEXT"
                    ");");
    if(!query.exec()) {
        success=false;
        qDebug()<<"userInfo table doesn't get created";
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
        qDebug()<<"values doesn,t get inserted into userInfo table ";
    }
    return success;
}

QVector<QString> database::getUserAndPass(QString userName)
{
    QVector<QString>userDetails;
    QSqlQuery query;
    qDebug()<<"getuser";
    query.prepare("SELECT user,pass FROM userInfo WHERE user = :userName");
    query.bindValue(":userName", userName);
    if(query.exec() && query.next()){
        // query.bindValue("amar",userName);
        userDetails.append(query.value(0).toString());
        userDetails.append(query.value(1).toString());

        qDebug()<<"dbuser"<<userDetails[0];
    }
    return userDetails;
}

