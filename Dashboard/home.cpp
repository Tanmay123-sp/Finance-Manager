#include "home.h"


home::home(QWidget *parent)
    : QWidget{parent}
{

rightWidget->setStyleSheet("background-color: #ecf0f1;");
QGridLayout* rightLayout = new QGridLayout(rightWidget);

QLabel *labelUsername = new QLabel("New home page", rightWidget);
labelUsername->setStyleSheet("color: #2c3e50; font-size: 16px;");
rightLayout->addWidget(labelUsername, 0, 0);

QLabel *labelPassword = new QLabel("dsadmsl", rightWidget);
labelPassword->setStyleSheet("color: #2c3e50; font-size: 16px;");
rightLayout->addWidget(labelPassword, 1, 0);
rightLayout->setColumnStretch(1, 1);
}
