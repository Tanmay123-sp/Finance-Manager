#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>


class database : public QObject
{
    Q_OBJECT
public:
    // explicit database(QObject *parent = nullptr);
    static database* getInstance();
    static database* instance;
    ~database();

    bool createTable();
    bool setUserDetails(QVector<QString>v);
    QVector<QString> getUserAndPass(QString);

private:
    database();
    database(const database&)=delete;
    database operator=(const database&)=delete;

    QSqlDatabase db;

signals:

};

#endif // DATABASE_H
