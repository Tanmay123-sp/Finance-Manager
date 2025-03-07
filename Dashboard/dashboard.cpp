#include "dashboard.h"
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QFormLayout>

using namespace std;

dashboard::dashboard(QString currentUser, QWidget *parent)
    : currentUser{currentUser},
    QWidget{parent},
    dbObj(database::getInstance())
{
    this->setStyleSheet("background-color:white");
    this->resize(800,600);
    this->setWindowTitle("DashBoard");
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

    connect(transactionButton, &QPushButton::clicked, this, &dashboard::getTransactionDetailsSlot);
    connect(transactionButton, &QPushButton::clicked, this, &dashboard::switchToTransaction);

    connect(homeButton, &QPushButton::clicked, this, &dashboard::switchToHome);
    connect(sendMoneyButton, &QPushButton::clicked, this, &dashboard::switchToSendMoney);
    connect(this, &dashboard::emitTransactionDetails, dbObj, &database::updateTransactionDetails);
    connect(logoutButton, &QPushButton::clicked, this, &dashboard::logoutUserSlot);
}

QWidget *dashboard::navigationBar()
{
    navWidget = new QWidget(this);
    navWidget->setStyleSheet("background-color:#2c3e50; color: white;");

    QVBoxLayout* navLayout = new QVBoxLayout(navWidget);

    QWidget *headerWidget = new QWidget(navWidget);
    QHBoxLayout* navHeaderLayout = new QHBoxLayout(headerWidget);
    headerWidget->setStyleSheet("background-color: #1abc9c;");

    QPixmap pixmap("C:/Users/HPGGS03/Documents/PersonalFianance/userIcon.png");
    QLabel *imageLabel = new QLabel();
    imageLabel->setPixmap(pixmap.scaled(24, 24, Qt::KeepAspectRatio));

    QLabel *userNameLabel = new QLabel(currentUser);
    userNameLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");

    navHeaderLayout->addWidget(imageLabel);
    navHeaderLayout->addWidget(userNameLabel);
    headerWidget->setLayout(navHeaderLayout);

    homeButton = new QPushButton("Home", this);
    homeButton->setStyleSheet("font-size: 18px; font-weight: bold;");

    transactionButton = new QPushButton("Transactions", this);
    transactionButton->setStyleSheet("font-size: 18px; font-weight: bold;");

    sendMoneyButton = new QPushButton("Send Money", this);
    sendMoneyButton->setStyleSheet("font-size: 18px; font-weight: bold;");

    logoutButton = new QPushButton("Logout",this);
    logoutButton->setStyleSheet("font-size: 18px; font-weight: bold");
    navLayout->addWidget(headerWidget);
    navLayout->addWidget(homeButton);
    navLayout->addWidget(transactionButton);
    navLayout->addWidget(sendMoneyButton);
    navLayout->addSpacing(380);
    navLayout->addWidget(logoutButton);
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

QWidget* dashboard::createTransactionsPage() {
    QWidget* transactionPage1 = new QWidget(this);
    transactionPage1->setObjectName("transactionPage1");

    // Create main layout for the page
    QVBoxLayout* mainLayout = new QVBoxLayout(transactionPage1);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Title label
    QLabel* pageTitle = new QLabel("Transactions", transactionPage1);
    pageTitle->setAlignment(Qt::AlignCenter);
    pageTitle->setStyleSheet("font-size: 24px; "
                             "font-weight: bold; "
                             "color: #2c3e50;"
                             "margin-bottom: 30px;"
                             "margin-top: 20px");

    mainLayout->addWidget(pageTitle, 0, Qt::AlignCenter);

    transactionTable = new QTableWidget(0, 4, transactionPage1);
    transactionTable->setObjectName("transactionTable");
    transactionTable->setFixedSize(500, 300);

    QStringList headers;
    headers << "Sender" << "Receiver" << "Amount" << "Date";
    transactionTable->setHorizontalHeaderLabels(headers);

    transactionTable->horizontalHeader()->setVisible(true);
    transactionTable->horizontalHeader()->setStretchLastSection(true);
    transactionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    transactionTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    transactionTable->verticalHeader()->setVisible(false);
    transactionTable->setShowGrid(true);
    transactionTable->setAlternatingRowColors(true);
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->setSelectionMode(QAbstractItemView::SingleSelection);

    transactionTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: blue;"
        "   border: 1px solid #e0e0e0;"
        "   color: black;"
        "   border-radius: 5px;"
        "}"
        "QHeaderView::section {"
        "   background-color: #cccccc;"
        "   color: green;"
        "   padding: 8px;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "   border: 1px solid #a0a0a0;"
        "}"
        );

    mainLayout->addWidget(transactionTable, 0, Qt::AlignCenter);

    mainLayout->addStretch(1);

    return transactionPage1;
}
void dashboard::updateTransactionPage()
{
    if (!transactionTable) return;

    transactionTable->setRowCount(0);

    transactionTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   color: black;"
        "}"
        "QHeaderView::section {"
        "   background-color: #e0e0e0;"
        "   color: black;"
        "   font-weight: bold;"
        "   padding: 8px;"
        "   border: none;"
        "   border-bottom: 2px solid #a0a0a0;"
        "   font-size: 14px;"
        "}"
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
            idItem->setBackground(QColor(255, 200, 200));
            descItem->setBackground(QColor(255, 200, 200));
            amountItem->setBackground(QColor(255, 200, 200));
            dateItem->setBackground(QColor(255, 200, 200));
        }
        else if (description.split(" ").contains(currentUser))
        {
            idItem->setBackground(QColor(200, 255, 200));
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
    transactionTable->repaint();
}

void dashboard::getTransactionDetailsSlot()
{
    transactionHistory = dbObj->getTransactionDetails(currentUser);
    updateTransactionPage();
    qDebug() << "Updated transaction table with" << transactionHistory.size() << "rows";
}

void dashboard::logoutUserSlot()
{
    // QSettings settings("PersonalFinanceApp","userSession");
    // settings.remove("session/username");
    this->close();
}

QWidget* dashboard::createSendMoneyPage()
{
    QWidget* sendMoneyPage1 = new QWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(sendMoneyPage1);

    QWidget* formContainer = new QWidget(sendMoneyPage1);
    formContainer->setFixedWidth(400);
    formContainer->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border-radius: 10px;"
        "   padding: 20px;"
        "}"
        "QLineEdit {"
        "   padding: 8px;"
        "   border: 2px solid #e0e0e0;"
        "   border-radius: 5px;"
        "   font-size: 14px;"
        // "   margin-bottom: 10px;"
        "   color: black;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #3498db;"
        "}"
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   border-radius: 5px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QLabel {"
        "   color: #2c3e50;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
        );

    QFormLayout* formLayout = new QFormLayout(formContainer);
    formLayout->setSpacing(15);
    formLayout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("Send Money", formContainer);
    titleLabel->setStyleSheet(
        "font-size: 24px;"
        "color: #2c3e50;"
        "font-weight: bold;"
        "margin-bottom: 20px;"
        );
    titleLabel->setAlignment(Qt::AlignCenter);
    formLayout->addRow(titleLabel);

    QLabel* labelUsername = new QLabel("Receiver Username", formContainer);
    receiverName = new QLineEdit(formContainer);
    receiverName->setPlaceholderText("Enter username");
    formLayout->addRow(labelUsername);
    formLayout->addRow(receiverName);

    QLabel* amtLabel = new QLabel("Amount", formContainer);
    inputAmt = new QLineEdit(formContainer);
    inputAmt->setPlaceholderText("Enter amount");
    formLayout->addRow(amtLabel);
    formLayout->addRow(inputAmt);

    send = new QPushButton("Send Money", formContainer);
    formLayout->addRow(send);

    mainLayout->addWidget(formContainer, 0, Qt::AlignCenter);

    mainLayout->addStretch(1);
    mainLayout->insertStretch(0, 2);

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
