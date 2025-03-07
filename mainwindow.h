#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
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
#include <QSettings>
#include <QByteArray>

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
    QDateTime currentDateTime;
    QString formattedDateAndTime;
    QString encryptedPass;
    QString decryptPass;
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    void saveSession(const QString &username);
    QString loadSession();
    void clearSession();
    QString encryptDecrypt(const QString& pass,char key);
    QString decrypt(const QString& encryptedPass,char key);

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
