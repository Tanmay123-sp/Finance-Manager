#include "mainwindow.h"
#include <QUuid>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),dbObj(database::getInstance())
{
    this->setStyleSheet("QMainWindow { background-color: white; }");
    this->resize(800, 600);
    this->setWindowTitle("Personal finance manager");
    QLabel *titleLabel = new QLabel("Personal Finance", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color:green");
    titleLabel->setAlignment(Qt::AlignCenter);

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

    connect(inputPassword, &QLineEdit::textChanged, this, &MainWindow::checkPasswords);
    connect(inputPassword2, &QLineEdit::textChanged, this, &MainWindow::checkPasswords);

    connect(submitButton, &QPushButton::clicked, this, &MainWindow::onSubmitButtonClicked);
    connect(this, &MainWindow::emitSig, this, &MainWindow::setUserDetailsToDB);
    connect(dashObj, &QWidget::destroyed, this, &MainWindow::close);
    decryptPass = decrypt(encryptedPass,'a');
    QString sessionUser = loadSession();
    if(!sessionUser.isEmpty()){
        currentUser = sessionUser;
        dashObj = new dashboard(currentUser);
        dashObj->show();
        qDebug()<<"dashboard opened";
    }
    // if(sessionUser.isEmpty()){
    //     connect(loginButton, &QPushButton::clicked, this, &MainWindow::openDashboardAfterLogin);
    // }
}

MainWindow::~MainWindow()
{
    qDebug()<<"destruct";

    delete loginPage;
    delete signUpPage;
    delete dbObj;
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

void MainWindow::saveSession(const QString &username)
{
    QSettings settings("FinanceApp","userSession");
    QString sessionId = QUuid::createUuid().toString();
    currentDateTime = QDateTime::currentDateTime();
    formattedDateAndTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    encryptedPass = encryptDecrypt(inputPassword->text(),'a');
    qDebug()<<encryptedPass;
    qDebug()<<decrypt(encryptedPass,'a');
    settings.setValue("session/sessionId",sessionId);
    settings.setValue("session/username.pass.time",username+"."+encryptedPass+"."+formattedDateAndTime);
    qDebug() << "Session saved for user:" << username<<" and session id:"<<sessionId<<"currentDateTime:"<<formattedDateAndTime;
}

QString MainWindow::loadSession()
{
    QSettings settings("FinanceApp","userSession");
    QString storedUser = settings.value("session/username","").toString();
    QString storedSessionId = settings.value("session/sessionId","").toString();
    if(!storedSessionId.isEmpty()){
        qDebug()<<"sessionId found, user :"<<storedUser;
    }
    else qDebug()<<"No active session";
    return storedUser;
}

void MainWindow::clearSession()
{
    QSettings settings("FinanceApp","userSession");
    settings.remove("session/userrname");
}

QString MainWindow::encryptDecrypt(const QString &pass, char key)
{
    QByteArray data = pass.toUtf8();
    for(int i =0; i<data.size();i++){
        data[i]=data[i]^key;
    }
    return QString::fromUtf8(data.toBase64());
}

QString MainWindow::decrypt(const QString &encryptedPass, char key)
{
    QByteArray data = QByteArray::fromBase64(encryptedPass.toUtf8());
    for (int i = 0; i < data.size(); ++i) {
        data[i] = data[i]^key;
    }
    return QString::fromUtf8(data);
}

void MainWindow::checkUserFromDb(QString a, QString b)
{
    qDebug()<<a<<" = "<<b;
    QVector<QString> userDetail = dbObj->getUserAndPass(a);
    qDebug()<<"111"<<userDetail[0]<<" "<<userDetail[1];
    if(a == userDetail[0]){
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
    saveSession(currentUser);
    dashObj = new dashboard(currentUser);
    dashObj->show();
    qDebug()<<"dashboard opened";
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
    qDebug()<<"onLoginButtonClicked() user = "<<user<<" = "<<pass;
    emit emitUserAndPass(user,pass);
}
