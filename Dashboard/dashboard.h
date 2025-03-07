#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QObject>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTableView>
#include <QTableWidget>
#include <QHeaderView>
#include "home.h"
#include "../Database/database.h"
#include <QSettings>

class dashboard : public QWidget
{
    Q_OBJECT
public:
    explicit dashboard(QString currentUser, QWidget *parent = nullptr);
    QLabel *labelUsername1;
    ~dashboard(){}

    database *dbObj;
    QWidget* HomePage;
    QWidget* TransactionPage;
    QWidget* SendMoneyPage;
    QWidget* navWidget;
    home* homeObj;
    QTableWidget* transactionTable;
    QVector<pair<QString, pair<QString, pair<QString, QString>>>>transactionHistory;
    void updateTransactionPage();

private:

    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* layout;
    QStackedWidget* stackWidget;
    QWidget* navigationBar();
    QWidget* createHomePage();
    QWidget* createTransactionsPage();
    QWidget* createSendMoneyPage();
    QPushButton* homeButton;
    QPushButton* transactionButton;
    QPushButton* sendMoneyButton;
    QPushButton* send;
    QPushButton* logoutButton;
    QLineEdit *receiverName;
    QLineEdit *inputAmt;
    QString currentUser;

signals:
    void emitTransactionDetails(QString sender,QString receiver,QString amt);

public slots:
    void switchToTransaction();
    void switchToHome();
    void switchToSendMoney();
    void switchToHomeWidget();
    void setTransactionDetails();
    void getTransactionDetailsSlot();
    void logoutUserSlot();
};

#endif // DASHBOARD_H
