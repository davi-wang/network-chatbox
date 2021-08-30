#include "mainwindow.h"

#include <QApplication>
#include <login.h>
#include <signin.h>
#include <signon.h>
#include <widget.h>
#include <connection.h>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include"Repeater.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


//    server->connectToHost(IP,port);//--------------要写的地方-----------
    QString IP,Port;
    ClientServer*Repeater=ClientServer::getInstance(IP,Port);

    //创建数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("pswd.db");
    if(db.open()){
        qDebug()<<"Build pswd database success !";
    }else{
        qDebug()<<"Build pswd database fail !";
    }
    //创建数据库表(用户名，密码, uid)
    QString logcmd = "create table if not exists userinfo (usersname verchar(64) primary key, password verchar(64), uid verchar(64))";
    QSqlQuery query;
    if(query.exec(logcmd)){
        qDebug()<<"Build pswd's table success!";
    }else{
        qDebug()<<"Build pswd's table fail!";
    }


//    MainWindow w; //主页面
//    w.show();
//    login list; //好友列表页面
//    list.show();
    SignIn signin; //登录页面
    signin.show();
//    Signon signon; //注册页面
//    signon.show();
    return a.exec();
}
