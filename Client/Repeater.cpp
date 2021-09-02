#include "Repeater.h"
#include<QDebug>
ClientServer* ClientServer:: instance=nullptr;


ClientServer::ClientServer()
{}

//连接状态检查
void ClientServer::Status_detect()
{

    connect_status=true;
    qDebug()<<"Connected!";
    connect(connector,SIGNAL(receiveMessage(Connection::DataType , const QJsonObject &)),this,SLOT(ProcessMsg(Connection::DataType ,const QJsonObject &)));
}


//信息分类
void ClientServer::ProcessMsg(Connection::DataType header,const QJsonObject &Data)
{
    switch(header)
    {

    case Connection::R2_verification_sending:
        emit verification_sending();break;
    case Connection::R3_verification_sent: emit verification_sent();break;
    case Connection::R5_fail:
    {
        int Error = Data.value("error").toInt();
         ErrorReason      = VerificationError(Error);
         qDebug()<<ErrorReason;
        emit Reg_fail(); break;
    }
    case Connection::R6_success:
        emit Reg_Success();break;

    case Connection::L2_logging:emit Logging();break;
    case Connection::L3_fail:
    {

        int Error = Data.value("error").toInt();
         ErrorReason      = VerificationError(Error);
        qDebug()<<ErrorReason;
        SignInFail( );
        break;
    }
    case Connection::L4_success:
    {
        connector->local_uid=Data.value("your_uid").toInt();
        local_uid=connector->local_uid;
        connector->peer_uid=Data.value("server_uid").toInt();
        UsrName=Data.value("username").toString();
        emit SignInSuccess();break;
    }
    case Connection::L5_synchro_data:
    {
        parseFriendList(Data);
       emit synchro_data();break;
    }
    case Connection::L6_synchronization_complete:
        emit synchronization_complete();break;


    case Connection::C2_sychro_history:
    {

       emit DistributeHistory(Data);
        break;
    }
    case Connection::C4_send_message:
    {
      emit DistributeMsg( Data);
        break;
    }

    case Connection::F2_return_users:
    {
        QJsonArray list = Data.value("users_list").toArray();

            for (int i=0; i < list.size(); ++i) {

                QJsonObject friend_info = list.at(i).toObject();
                UsrInfo Info;

                Info.u_id = friend_info.value("uid").toInt();
                Info.Email = friend_info.value("email").toString();
                Info.NickName = friend_info.value("nickname").toString();
                NewFriends[Info.u_id]=Info;
                NewFriend_u_ID.push_back(Info.u_id);
            }
            QList<int> onlines_uid = NewFriends.keys();
            for(int i=0;i<onlines_uid.size();i++)
            {
                qDebug()<<i;
            }
            emit return_users();
            break;
    }
    case Connection::F4_new_friend:
    {
        NewFriend_ID=new int;
        *NewFriend_ID=Data.value("new_friend_uid").toInt();
        QJsonObject json;
        json.insert("uid",*NewFriend_ID);
        connector->sendMessage(Connection::F5_request_user_info,json);
        qDebug()<<"New Friend ID"<<*NewFriend_ID;
        break;
    }
    case Connection::F5_request_user_info:
    {

        UsrInfo friends;
        friends.u_id= Data.value("uid").toInt();
        friends.Email = Data.value("email").toString();
        friends.NickName = Data.value("nickname").toString();
        FriendList[friends.u_id]=friends;
        Friend_u_IDs.push_back(friends.u_id);

        qDebug()<<"Success"<<*NewFriend_ID;
        emit request_user_info();
        break;
    }
    case Connection::B0_broadcast_msg:
    {
        emit NewMsgInPCR(Data);
        break;
    }
    case Connection::B2_synchro_list:
    {
        emit SynList(Data);
        break;
    }
    default: qDebug()<<"Oops, invalid header!";

    }
}



void ClientServer::parseFriendList(const QJsonObject data)
{
    QJsonArray list = data.value("friend_list").toArray();

    for (int i=0; i < list.size(); ++i) {
        QJsonObject friend_info = list.at(i).toObject();
        // 下面三行是一个好友
        UsrInfo friends;

        friends.u_id= friend_info.value("uid").toInt();
        friends.Email = friend_info.value("email").toString();
        friends.NickName = friend_info.value("nickname").toString();
        Friend_u_IDs.push_back(friends.u_id);
        FriendList.insert(friends.u_id,friends);
        // 上面三个变量就可以存了
    }

}



