#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QObject>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGridLayout>
#include <QPushButton>

class dashboard : public QWidget
{
    Q_OBJECT
public:
    explicit dashboard(QWidget *parent = nullptr);
    QLabel *labelUsername1;
    ~dashboard(){}

    QWidget* HomePage;
    QWidget* TransactionPage;
    QWidget* SendMoneyPage;
    QWidget* navWidget;

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

signals:

public slots:
    void switchToTransaction();
    void switchToHome();
    void switchToSendMoney();
};

#endif // DASHBOARD_H
