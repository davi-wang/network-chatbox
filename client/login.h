#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
//#include<QPainter>


namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private:
    Ui::login *ui;
    QVector<bool> IsShow;

};

#endif // LOGIN_H


