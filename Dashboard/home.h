#ifndef HOME_H
#define HOME_H

#include <QObject>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

class home : public QWidget
{
    Q_OBJECT
public:
    explicit home(QWidget *parent = nullptr);

    QWidget* HomePage1 = new QWidget();
    QWidget* rightWidget = new QWidget(HomePage1);

signals:
};

#endif // HOME_H
