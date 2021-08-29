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

#include<QVector>
#include<QThread>
#include<windows.h>
#include<QCoreApplication>
#include"signup.h"
#include "text_msg.h"


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
    void Sign();
    Response Sendtxt(QString U_peerID,QJsonObject Msg);                  //send text Msg
    Response SendFile(QString U_peerID,QString FilePath);            //send files
    bool Start_Server(QString IP,QString Port);  //connect

    //recieving functions

//signals   send Message to this Object Owner
signals:
void TextMsg(QJsonObject text,QString Sender);             //recieve textMsg
void GetFile(QString FileName);             //recieve file, Write the file into hardDisk
void Userinfo(QJsonObject User);            //recieve info

//SignUP part signal
void EmailSending();




//Don't use these signals, they are limited in this Object.




public slots:
  //  void SaveFile(QString FileName,QString Dir);  //save files


private:
    Server* CurrentServer;
    Connection*connect_fun;
    QTcpSocket* m_skt;
    QJsonObject* Message;
    QJsonObject *TemStash;

    SignUp *SignAccount;
    Text_Msg *TextMsgServer;

    void SendMes(Connection::DataType req,QJsonObject Data);



    bool flag;



private slots:
    void connect_status();
    void disconnect_status();
    void SignEND();
};

#endif // SR_SERVICE_H
