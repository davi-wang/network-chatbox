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
        qDebug()<<"Send Msg";
    }

    int local_uid;
    QString UsrName;


    VerificationError ErrorReason;




    QMap<int,UsrInfo> FriendList;//好友列表
    QVector<int>   Friend_u_IDs;//好友U_ID


    QMap<int,UsrInfo> NewFriends;//新好友列表
    QVector<int>   NewFriend_u_ID;//新好友U_ID
    int * NewFriend_ID;  //选中好友的U_ID

    QJsonObject Cache;  //额外缓存
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

      void return_users();  //返回新好友列表
      void new_friend();    //新好友uID
      void request_user_info(); //好友列表中添加新好友信息

      void DistributeHistory(const QJsonObject &);//分发历史信息
      void DistributeMsg(const QJsonObject &);//分发信息

      void NewMsgInPCR(const QJsonObject &);//聊天室收到新消息
      void SynList(const QJsonObject &);//同步在线群成员




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
    Connection* connector;  //连接器
    bool connect_status=false;


    void parseFriendList(QJsonObject data);   //加载好友列表

private slots:
    void ProcessMsg(Connection::DataType,const QJsonObject &);
    void Status_detect();
};

#endif // CLIENTSERVER_H
