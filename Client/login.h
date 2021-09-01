#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "Repeater.h"
#include "DataTypes.h"
#include "ui_login.h"
#include <QIcon>
#include <QToolButton>
#include <QApplication>
#include <QMessageBox>
#include "Repeater.h"
#include "DataTypes.h"
#include "addfrd.h"
#include"chatwindow.h"
#include<QMap>


namespace Ui {
class login;
}





class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();


private slots:
    void on_pushButton_clicked();
    void AddFriendFinish();

private:
    Ui::login *ui;
    QVector<bool> IsShow;

    ClientServer *Repeater;
    QVector<QToolButton*>   ShowFriends;
    QMap<int,ChatWindow*>  ChatWindows;
    int total;

    AddFrd *  bu=nullptr;

};

#endif // LOGIN_H


