#ifndef SR_SERVICE_H
#define SR_SERVICE_H

#include <QObject>
#include"connection.h"
#include"QString"
#include<QHostAddress>
#include<QJsonObject>
#include<QJsonDocument>
#include<QBitArray>
#include<QTime>
#include"processingmsg.h"
#include<QVector>
#include<QThread>


struct Server{
    QString ServerIP;
    QString ServerPort;
};

enum Response
{
    OK,
    Warning,
    Error
};



class SR_Service:public QObject
{
    Q_OBJECT
public:

    bool status;//connection status

    SR_Service();
    bool Halt_Server();   // disconnect
    ~SR_Service();


    //sending functions
    Response Signin(QString U_name,QString password);                // sign in
    Response SignUP(QString U_name,QString password,QString Email);  //sign up
    Response Sendtxt(QString U_peerID,QJsonObject Msg);                  //send text Msg
    Response SendFile(QString U_peerID,QString FilePath);            //send files


    //recieving functions

//signals   send Message to this Object Owner
signals:
void TextMsg(QJsonObject text,QString Sender);             //recieve textMsg
void GetFile(QString FileName);             //recieve file, Write the file into hardDisk
void Userinfo(QJsonObject User);            //recieve info

//Don't use these signals, they are limited in this Object.
void LogInSuccess();
void SendSuccess();



public slots:
  //  void SaveFile(QString FileName,QString Dir);  //save files





private:
    Server* CurrentServer;
    Connection*connect_fun;
    QTcpSocket* m_skt;
    QJsonObject* Message;
    QJsonObject *TemStash;



    void SendMes(DataType req,QJsonObject Data);
    bool Start_Server(QString IP,QString Port);  //connect


    bool flag;



private slots:
    void connect_status();
    void disconnect_status();
    void RecieveMsg(DataType header, const QJsonObject data);

};

#endif // SR_SERVICE_H
