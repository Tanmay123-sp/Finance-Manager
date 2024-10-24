#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),dbObj(database::getInstance())
{
    this->setStyleSheet("QMainWindow { background-color: white; }");
    this->resize(800, 600);

    QLabel *titleLabel = new QLabel(QCoreApplication::applicationName(), this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color:green"); // Style the label
    // titleLabel->setGeometry(100,100,0,120);
    titleLabel->setAlignment(Qt::AlignCenter); //

    stackwidget = new QStackedWidget(this);
    loginPage = createLoginPage();
    signUpPage = createSignUpPage();

    stackwidget->addWidget(loginPage);
    stackwidget->addWidget(signUpPage);
    stackwidget->setCurrentIndex(0);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(stackwidget);
    setCentralWidget(centralWidget);

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
    connect(this, &MainWindow::emitUserAndPass, this, &MainWindow::checkUserFromDb);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::openDashboardAfterLogin);

    connect(inputPassword, &QLineEdit::textChanged, this, &MainWindow::checkPasswords);
    connect(inputPassword2, &QLineEdit::textChanged, this, &MainWindow::checkPasswords);

    connect(submitButton, &QPushButton::clicked, this, &MainWindow::onSubmitButtonClicked);
    connect(this, &MainWindow::emitSig, this, &MainWindow::setUserDetailsToDB);
    connect(dashObj, &QWidget::destroyed, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    qDebug()<<"destruct";

    // dashObj->close();
    // delete dashObj;
    qDebug()<<"dashObj";
    delete loginPage;
    qDebug()<<"loginPage";
    delete signUpPage;
    qDebug()<<"signUpPage";
    delete dbObj;
    qDebug()<<"dbObj";
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "MainWindow is closing";
    delete dashObj;
    dashObj=nullptr;
    event->accept();
}


QWidget* MainWindow::createLoginPage()
{
    loginPage = new QWidget(this);
    QVBoxLayout *loginLayout = new QVBoxLayout(loginPage);
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    QGridLayout *gridLayout = new QGridLayout();

    QLabel *labelUsername = new QLabel("Enter username:", this);
    labelUsername->setStyleSheet("QLabel { color: blue; }");
    gridLayout->addWidget(labelUsername, 0, 0);

    inputUsername1 = new QLineEdit(this);
    gridLayout->addWidget(inputUsername1, 0, 1);

    QLabel *labelPassword = new QLabel("Enter password:", this);
    labelPassword->setStyleSheet("QLabel { color: blue; }");
    gridLayout->addWidget(labelPassword, 1, 0);

    inputPassword = new QLineEdit(this);
    inputPassword->setEchoMode(QLineEdit::Password);
    gridLayout->addWidget(inputPassword, 1, 1);

    loginButton = new QPushButton("Login", this);
    gridLayout->addWidget(loginButton, 2, 0, 1, 1);

    signupButton = new QPushButton("Sign Up", this);
    gridLayout->addWidget(signupButton, 2, 1, 1, 1);

    gridLayout->setColumnStretch(0, 0);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setSpacing(10);

    horizontalLayout->addStretch();
    horizontalLayout->addLayout(gridLayout);
    horizontalLayout->addStretch();

    loginLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    loginLayout->addLayout(horizontalLayout);
    loginLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(signupButton, &QPushButton::clicked, this, &MainWindow::switchToSignUp);

    return loginPage;
}

QWidget* MainWindow::createSignUpPage()
{
    signUpPage = new QWidget(this);
    QVBoxLayout *signUpLayout = new QVBoxLayout(signUpPage);
    QHBoxLayout *signUpHorizontalLayout = new QHBoxLayout();
    QGridLayout *SignUpGridLayout = new QGridLayout();

    QLabel *labelUsername1 = new QLabel("Enter username:", this);
    labelUsername1->setStyleSheet("QLabel { color: blue; }");
    SignUpGridLayout->addWidget(labelUsername1, 0, 0);

    inputUsername2 = new QLineEdit(this);
    SignUpGridLayout->addWidget(inputUsername2, 0, 1);

    QLabel *labelPassword = new QLabel("Create password:", this);
    labelPassword->setStyleSheet("QLabel { color: blue; }");
    SignUpGridLayout->addWidget(labelPassword, 1, 0);

    inputPassword1 = new QLineEdit(this);
    inputPassword1->setEchoMode(QLineEdit::Password);
    SignUpGridLayout->addWidget(inputPassword1, 1, 1);

    QLabel *labelPassword2 = new QLabel("Confirm password:", this);
    labelPassword2->setStyleSheet("QLabel { color: blue; }");
    SignUpGridLayout->addWidget(labelPassword2, 2, 0);

    inputPassword2 = new QLineEdit(this);
    inputPassword2->setEchoMode(QLineEdit::Password);
    SignUpGridLayout->addWidget(inputPassword2, 2, 1);

    backToLoginButton = new QPushButton("Back to Login", this);
    SignUpGridLayout->addWidget(backToLoginButton, 3,0);

    submitButton = new QPushButton("submit", this);
    SignUpGridLayout->addWidget(submitButton, 3,1);

    labelPasswordError = new QLabel(this);
    labelPasswordError->setStyleSheet("QLabel { color: red; }");
    labelPasswordError->setVisible(false);
    SignUpGridLayout->addWidget(labelPasswordError, 4, 0);

    SignUpGridLayout->setColumnStretch(0, 0);
    SignUpGridLayout->setColumnStretch(1, 1);
    SignUpGridLayout->setSpacing(10);

    signUpHorizontalLayout->addStretch();
    signUpHorizontalLayout->addLayout(SignUpGridLayout);
    signUpHorizontalLayout->addStretch();

    signUpLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    signUpLayout->addLayout(signUpHorizontalLayout);
    signUpLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(backToLoginButton, &QPushButton::clicked, this, &MainWindow::switchToLogin);
    return signUpPage;
}

void MainWindow::checkUserFromDb(QString a, QString b)
{
    qDebug()<<a<<" = "<<b;
    QVector<QString> userDetail = dbObj->getUserAndPass("tanmay");
    qDebug()<<"111"<<userDetail[0]<<" "<<userDetail[1];
    if("tanmay" == userDetail[0]){
        if(b == userDetail[1]){
            qDebug()<<"Login successfull";
        }
        else qDebug()<<"password is incorrect";
    }
    else qDebug()<<"user not found";
}

void MainWindow::openDashboardAfterLogin()
{
    currentUser = inputUsername1->text();
    qDebug()<<currentUser<<" main currentUser";
    dashObj = new dashboard(currentUser);
    dashObj->show();

    qDebug()<<"dash win";
}

void MainWindow::switchToSignUp()
{
    stackwidget->setCurrentIndex(1);
}

void MainWindow::switchToLogin()
{
    stackwidget->setCurrentIndex(0);
}

void MainWindow::onSubmitButtonClicked()
{
    QString user11 = inputUsername2->text();
    QString pass11= inputPassword1->text();
    qDebug()<<"user"<<user11;
    qDebug()<<"pass"<<pass11;
    emit emitSig(user11,pass11);
}

void MainWindow::setUserDetailsToDB(QString user,QString pass)
{
    qDebug()<<user<<" "<<pass;
    QVector<QString>userInputs;
    userInputs.push_back(user);
    userInputs.push_back(pass);
    dbObj->setUserDetails(userInputs);
}

void MainWindow::checkPasswords()
{
    QString password1 = inputPassword1->text();
    QString password2 = inputPassword2->text();

    if (password1.isEmpty() || password2.isEmpty()) {
        labelPasswordError->setVisible(false);
    } else if (password1 != password2) {
        labelPasswordError->setText("Passwords do not match");
        labelPasswordError->setVisible(true);
    } else {
        labelPasswordError->setVisible(false);
    }
}

void MainWindow::onLoginButtonClicked()
{
    QString user = inputUsername1->text();
    QString pass = inputPassword->text();
    qDebug()<<"user = "<<user<<" = "<<pass;
    emit emitUserAndPass(user,pass);
}
