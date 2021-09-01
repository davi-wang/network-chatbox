#ifndef LOGON_H
#define LOGON_H

#include <QWidget>
#include "Repeater.h"
#include "ui_signin.h"
#include "QMessageBox"
#include "QSettings"
#include "qdebug.h"
#include "QJsonObject"
#include "QJsonValue"
#include "QCryptographicHash"


namespace Ui { class SignIn; }

class SignIn : public QWidget
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent = nullptr);
    ~SignIn();

private slots:
    void on_SigninBt_clicked(); //登录
    void on_SignonBt_clicked(); //注册

    void on_userEdit_currentTextChanged(QString arg1); //通过userEdit索引自动填充密码



    //与Repeater响应
    //自己的槽函数 /接收信号
    void ShowLoggingProcess(); //服务器已收到登录请求
    void SignInSuccess(); //登录成功，同步数据
    void SignInFail(); //登录失败
    void synchro_data(); //同步数据（用户个人profile+好友列表+群组列表）
    void synchronization_complete(); //同步数据完成


private:
    Ui::SignIn *ui;
    ClientServer * Repeater = ClientServer::GetInstance();
    QString usersname; //用户名
    QString password; //密码
    QString Ip; //ip
    QString Port; //port
};

#endif // LOGON_H
