#include "dashboard.h"
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>

using namespace std;

dashboard::dashboard(QString currentUser, QWidget *parent)
    : currentUser{currentUser},
      QWidget{parent},
      dbObj(database::getInstance())
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
    connect(transactionButton, &QPushButton::clicked, this, &dashboard::getTransactionDetailsSlot);
    connect(transactionButton, &QPushButton::clicked, this, &dashboard::switchToTransaction);

    connect(homeButton, &QPushButton::clicked, this, &dashboard::switchToHome);
    connect(sendMoneyButton, &QPushButton::clicked, this, &dashboard::switchToSendMoney);
    // connect(sendMoneyButton, &QPushButton::clicked, this, &dashboard::switchToHomeWidget);
    connect(this, &dashboard::emitTransactionDetails, dbObj, &database::updateTransactionDetails);
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

QWidget* dashboard::createTransactionsPage()
{
    QWidget* transactionPage1 = new QWidget(this);
    transactionPage1->setObjectName("transactionPage1");

    QVBoxLayout* pageLayout = new QVBoxLayout(transactionPage1);
    pageLayout->setContentsMargins(10, 10, 10, 10);

    QLabel* pageTitle = new QLabel("Transactions", transactionPage1);
    pageTitle->setStyleSheet("font-size: 24px; font-weight: bold;");
    pageLayout->addWidget(pageTitle);

    transactionTable = new QTableWidget(0, 4, transactionPage1);
    transactionTable->setObjectName("transactionTable");
    QStringList headers;
    headers << "Sender" << "Receiver" << "Amount" << "Date";
    transactionTable->setHorizontalHeaderLabels(headers);
    transactionTable->setStyleSheet(
        "QTableWidget { background-color: white; }"
        "QHeaderView::section { background-color: #f0f0f0; }"
        );

    transactionTable->setMinimumSize(500, 300);
    pageLayout->addWidget(transactionTable);

    transactionPage1->setLayout(pageLayout);

    return transactionPage1;
}

void dashboard::updateTransactionPage()
{
    if (!transactionTable) return;

    transactionTable->setRowCount(0);  // Clear existing rows

    transactionTable->setStyleSheet(
        "QTableWidget { background-color: white; color: black; }"
        "QHeaderView::section { background-color: #f0f0f0; color: black; }"
        );

    for (int i = 0; i < transactionHistory.size(); ++i)
    {
        transactionTable->insertRow(i);

        QString transactionId = transactionHistory[i].first;
        QString description = transactionHistory[i].second.first;
        QString amount = transactionHistory[i].second.second.first;
        QString date = transactionHistory[i].second.second.second;

        QTableWidgetItem *idItem = new QTableWidgetItem(transactionId);
        QTableWidgetItem *descItem = new QTableWidgetItem(description);
        QTableWidgetItem *amountItem = new QTableWidgetItem(amount);
        QTableWidgetItem *dateItem = new QTableWidgetItem(date);

        // Set text color to black for all items
        idItem->setForeground(Qt::black);
        descItem->setForeground(Qt::black);
        amountItem->setForeground(Qt::black);
        dateItem->setForeground(Qt::black);

        if (transactionId == currentUser)
        {
            idItem->setBackground(QColor(255, 200, 200));  // Light green
            descItem->setBackground(QColor(255, 200, 200));
            amountItem->setBackground(QColor(255, 200, 200));
            dateItem->setBackground(QColor(255, 200, 200));
        }
        else if (description.split(" ").contains(currentUser))
        {
            idItem->setBackground(QColor(200, 255, 200));  // Light red
            descItem->setBackground(QColor(200, 255, 200));
            amountItem->setBackground(QColor(200, 255, 200));
            dateItem->setBackground(QColor(200, 255, 200));
        }

        transactionTable->setItem(i, 0, idItem);
        transactionTable->setItem(i, 1, descItem);
        transactionTable->setItem(i, 2, amountItem);
        transactionTable->setItem(i, 3, dateItem);
    }

    transactionTable->resizeColumnsToContents();
    transactionTable->update();
}

void dashboard::getTransactionDetailsSlot()
{
    transactionHistory = dbObj->getTransactionDetails(currentUser);
    updateTransactionPage();
    qDebug() << "Updated transaction table with" << transactionHistory.size() << "rows";
}

QWidget *dashboard::createSendMoneyPage()
{
    QWidget* sendMoneyPage1 = new QWidget(this);
    QWidget* rightWidget = new QWidget(sendMoneyPage1);
    rightWidget->setStyleSheet("background-color: blue;");
    QGridLayout* rightLayout = new QGridLayout(rightWidget);

    QLabel *labelUsername = new QLabel("Receiver username:", rightWidget);
    labelUsername->setStyleSheet("color: #2c3e50; font-size: 16px;");
    rightLayout->addWidget(labelUsername, 0, 0);

    receiverName = new QLineEdit(rightWidget);
    rightLayout->addWidget(receiverName, 0, 1);

    QLabel *amtLabel = new QLabel("Amount:", rightWidget);
    amtLabel->setStyleSheet("color: #2c3e50; font-size: 16px;");
    rightLayout->addWidget(amtLabel, 1, 0);

    inputAmt = new QLineEdit(rightWidget);
    rightLayout->addWidget(inputAmt, 1, 1);

    // Set parent of the button to rightWidget so it can be part of rightLayout
    send = new QPushButton("Send", rightWidget);
    send->setStyleSheet("color: #2c3e89; font-size: 18px; font-weight: bold;");
    rightLayout->addWidget(send, 2, 0, 1, 2);  // Make sure it spans 2 columns

    rightLayout->setColumnStretch(1, 2);

    connect(send, &QPushButton::clicked, this, &dashboard::setTransactionDetails);

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

void dashboard::switchToHomeWidget()
{
    homeObj = new home();
}

void dashboard::setTransactionDetails()
{
    QString rName = receiverName->text();
    QString amt = inputAmt->text();
    qDebug()<<currentUser<<"  currentUser"<<" rName"<<rName<<" amt "<<amt;

    emit emitTransactionDetails(currentUser,rName,amt);
}
