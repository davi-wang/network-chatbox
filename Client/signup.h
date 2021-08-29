#ifndef SIGNUP_H
#define SIGNUP_H

#include <QObject>
#include"connection.h"
#include<QCoreApplication>
#include<QJsonObject>
#include<QTime>




class SignUp:public QObject
{
    Q_OBJECT
public:
    SignUp(Connection* Connector);
    void SignUp_GetEmail(QString VerifyCode);
    void SignUp_GetUser(QString UsrName,QString password,QString Email);

    void SignIn(QString Email,QString password);
private:

    Connection* Connector;
    bool flag;
    QTime time;
    QJsonObject Usr;



private slots:
    void RecieveMsg_Reg(Connection::DataType header, const QJsonObject data);
    void RecieveMsg_log(Connection::DataType header, const QJsonObject data);

signals:
    void EmailSENGING();
    void EmailVERIFY();
    void Success(QJsonObject Info);
    void Fail(QJsonObject reason);

    void logging();
    void Fail_log(QJsonObject Data);
    void SuccessLog();
    void GetSynchro_Data(QJsonObject Data);
    void Synchro_DataComplete();


};

#endif // SIGNUP_H
