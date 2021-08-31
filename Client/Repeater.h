#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QObject>
#include"connection.h"
#include<QJsonObject>
#include<QJsonValue>
#include<QFont>
#include<DataTypes.h>
#include<QMetaEnum>
#include<QQueue>
#include<QMap>
#include<QVector>


class ClientServer:public QObject
{
    Q_OBJECT
public:
    static ClientServer* CreateInstance(QString IP,QString Port) {
            if(instance == nullptr) {
                instance = new ClientServer( IP, Port);
            }
            return instance;
        }

    static ClientServer* GetInstance() {
            if(instance == nullptr) {
                instance = new ClientServer();
            }
            return instance;
        }
    void SendMsg(Connection::DataType header,QJsonObject &data)
    {
        connector->sendMessage(header,data);
    }


    QString UsrName;
    QJsonObject Register;


    QMap<int,UsrInfo> FriendList;//好友列表
    QVector<int>   Friend_u_IDs;//好友U_ID


    QQueue<ChatRecord> HistoryInfo;//历史记录
    ReceieveText GetTextMsg;//收到发来的消息
    VerificationError ErrorReason;//错误信息


    QMap<int,UsrInfo> NewFriends;//好友列表
    QVector<int>   NewFriend_u_ID;//好友U_ID
    int * NewFriend_ID;  //选中好友的U_ID


signals:
    void verification_sending();
    void verification_sent();
    void Reg_fail();
    void Reg_Success();

    void Logging();
    void SignInSuccess();
    void SignInFail();
    void synchro_data();
    void synchronization_complete();

    void sychro_history();
    void send_message();

    void return_users();
    void new_friend();
    void request_user_info();


private:
    static ClientServer* instance;
    ClientServer();
    ClientServer(QString IP,QString Port)
    {

        connect_status=false;
        connector=new Connection();
        connector->connectToHost(IP,Port.toUShort());
        connect(connector,SIGNAL(connected()),this,SLOT(Status_detect()));

    }
    ~ClientServer()
    {
        delete connector;

    }

    Connection* connector;
    bool connect_status=false;


    QJsonObject Cache;


    void processRecievedTextMsg(QJsonObject Data);  //加载收到的信息
    void parseHistoryList(QJsonObject data);  //加载历史记录
    void parseFriendList(QJsonObject data);   //加载还有列表

private slots:
    void ProcessMsg(Connection::DataType,const QJsonObject &);
    void Status_detect();
};

#endif // CLIENTSERVER_H
