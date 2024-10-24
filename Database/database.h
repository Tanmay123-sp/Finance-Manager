#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
using namespace std;

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
    QVector<pair<QString, pair<QString, pair<QString, QString>>>> getTransactionDetails(QString);

private:
    database();
    database(const database&)=delete;
    database operator=(const database&)=delete;
    QString currentUser;
    QSqlDatabase db;

signals:

public slots:
    void updateTransactionDetails(const QString sender, const QString receiver, const QString amt);
};

#endif // DATABASE_H
