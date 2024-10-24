#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QCloseEvent>
#include "database.h"
#include "dashboard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget *loginPage = nullptr ;
    QWidget *signUpPage = nullptr ;
    database* dbObj = nullptr;
    dashboard* dashObj = nullptr;
    QString currentUser;

    void closeEvent(QCloseEvent *event);
private:
    QStackedWidget *stackwidget;

    QWidget* createLoginPage();
    QWidget* createSignUpPage();
    QLineEdit *inputUsername1;
    QLineEdit *inputUsername2;
    QLineEdit *inputPassword;
    QLineEdit *inputPassword1;
    QLineEdit *inputPassword2;
    QLabel *labelPasswordError;
    QPushButton *loginButton;
    QPushButton *signupButton;
    QPushButton *submitButton;
    QPushButton *backToLoginButton;
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);


signals:
    void emitSig(QString , QString);
    void emitUserAndPass(QString,QString);

private slots:
    void switchToSignUp();
    void switchToLogin();
    void onSubmitButtonClicked();
    void setUserDetailsToDB(QString,QString);
    void checkPasswords();
    void onLoginButtonClicked();
    void checkUserFromDb(QString,QString);
    void openDashboardAfterLogin();
};

#endif // MAINWINDOW_H
