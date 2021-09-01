#include "mainwindow.h"

#include <QApplication>
#include <login.h>
#include <signin.h>
#include <signon.h>

#include <buildconnect.h>
#include <QDebug>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w; //主页面
//    w.show();
//    login list; //好友列表页面
//    list.show();
//    SignIn signin; //登录页面
//    signin.show();
//    Signon signon; //注册页面
//    signon.show();
    BuildConnect BCt; //建立联系页面
    BCt.show();
    return a.exec();
}
