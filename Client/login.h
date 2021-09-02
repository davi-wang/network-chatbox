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
#include "pcr.h"


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
    void DistributeHistory(const QJsonObject &);  //历史分发
    void DistributeMsg(const QJsonObject &);//信息分发
    void on_joininPCR_clicked();//加入聊天室按钮

private:
    Ui::login *ui;
    QVector<bool> IsShow;

    ClientServer *Repeater;
    QVector<QToolButton*>   ShowFriends;
    QMap<int,ChatWindow*>  ChatWindows;
    int total;

    AddFrd *  bu=nullptr;
    PCR * mo = nullptr;
};

#endif // LOGIN_H


