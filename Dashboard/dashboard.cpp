#include "dashboard.h"
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>

dashboard::dashboard(QWidget *parent)
    : QWidget{parent}
{
    this->setStyleSheet("background-color:white");
    this->resize(800,600);

    layout = new QVBoxLayout(this);

    navWidget = navigationBar();
    stackWidget = new QStackedWidget(this);
    HomePage = createHomePage();
    TransactionPage = createTransactionsPage();
    SendMoneyPage = createSendMoneyPage();

    stackWidget->addWidget(HomePage);
    stackWidget->addWidget(TransactionPage);
    stackWidget->addWidget(SendMoneyPage);

    stackWidget->setCurrentIndex(0);

    mainLayout->addWidget(navWidget,20);
    mainLayout->addWidget(stackWidget,80);

    // connect(homeButton, &QPushButton::clicked, this, &dashboard::createHomePage);
    // connect(transactionButton, &QPushButton::clicked, this, &dashboard::createTransactionsPage);
    connect(transactionButton, &QPushButton::clicked, this, &dashboard::switchToTransaction);
    connect(homeButton, &QPushButton::clicked, this, &dashboard::switchToHome);
    connect(sendMoneyButton, &QPushButton::clicked, this, &dashboard::switchToSendMoney);
}

QWidget *dashboard::navigationBar()
{
    navWidget = new QWidget(this);
    navWidget->setStyleSheet("background-color:#2c3e50; color: white;");
    QVBoxLayout* navLayout = new QVBoxLayout(navWidget);
    QLabel *labelSidebar = new QLabel("Sidebar", navWidget);
    labelSidebar->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");


    homeButton = new QPushButton(this);
    homeButton->setText("Home");
    homeButton->setStyleSheet("font-size: 18px; font-weight: bold;");

    transactionButton = new QPushButton(this);
    transactionButton->setText("Transactions");
    transactionButton->setStyleSheet("font-size: 18px; font-weight: bold;");

    sendMoneyButton = new QPushButton(this);
    sendMoneyButton->setText("Send Money");
    sendMoneyButton->setStyleSheet("font-size: 18px; font-weight: bold;");

    navLayout->addWidget(labelSidebar);
    navLayout->addWidget(homeButton);
    navLayout->addWidget(transactionButton);
    navLayout->addWidget(sendMoneyButton);
    navLayout->addStretch();

    return navWidget;
}

QWidget *dashboard::createHomePage()
{
    QWidget* HomePage1 = new QWidget(this);
    QWidget* rightWidget = new QWidget(HomePage1);
    rightWidget->setStyleSheet("background-color: #ecf0f1;");
    QGridLayout* rightLayout = new QGridLayout(rightWidget);

    QLabel *labelUsername = new QLabel("home page", rightWidget);
    labelUsername->setStyleSheet("color: #2c3e50; font-size: 16px;");
    rightLayout->addWidget(labelUsername, 0, 0);

    QLabel *labelPassword = new QLabel("dsadmsl", rightWidget);
    labelPassword->setStyleSheet("color: #2c3e50; font-size: 16px;");
    rightLayout->addWidget(labelPassword, 1, 0);
    rightLayout->setColumnStretch(1, 1);
    return HomePage1;
}

QWidget *dashboard::createTransactionsPage()
{
    QWidget* transactionPage1 = new QWidget(this);
    QWidget* rightWidget = new QWidget(transactionPage1);
    rightWidget->setStyleSheet("background-color: #ecf0f1;");
    QGridLayout* rightLayout = new QGridLayout(rightWidget);

    QLabel *labelUsername = new QLabel("transaction Page", rightWidget);
    labelUsername->setStyleSheet("color: #2c3e50; font-size: 16px;");
    rightLayout->addWidget(labelUsername, 0, 0);

    QLabel *labelPassword = new QLabel("Create password:", rightWidget);
    labelPassword->setStyleSheet("color: #2c3e50; font-size: 16px;");
    rightLayout->addWidget(labelPassword, 1, 0);
    rightLayout->setColumnStretch(1, 1);
    return transactionPage1;
}

QWidget *dashboard::createSendMoneyPage()
{
    QWidget* sendMoneyPage1 = new QWidget(this);
    QWidget* rightWidget = new QWidget(sendMoneyPage1);
    rightWidget->setStyleSheet("background-color: #ecf0f1;");
    QGridLayout* rightLayout = new QGridLayout(rightWidget);

    QLabel *labelUsername = new QLabel("Send money Page", rightWidget);
    labelUsername->setStyleSheet("color: #2c3e50; font-size: 16px;");
    rightLayout->addWidget(labelUsername, 0, 0);

    QLabel *labelPassword = new QLabel("-----", rightWidget);
    labelPassword->setStyleSheet("color: #2c3e50; font-size: 16px;");
    rightLayout->addWidget(labelPassword, 1, 0);
    rightLayout->setColumnStretch(1, 1);
    return sendMoneyPage1;
}

void dashboard::switchToTransaction()
{
    stackWidget->setCurrentIndex(1);
}

void dashboard::switchToHome()
{
    stackWidget->setCurrentIndex(0);
}

void dashboard::switchToSendMoney()
{
    stackWidget->setCurrentIndex(2);
}
