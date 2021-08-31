#include "Repeater.h"
#include<QDebug>
ClientServer* ClientServer:: instance=nullptr;


ClientServer::ClientServer()
{}

void ClientServer::Status_detect()
{
    connector=new Connection(tcp);
    connect_status=true;
    connect(connector,SIGNAL(receiveMessage(DataType header, const QJsonObject &data)),this,SLOT(ProcessMsg(Connection::DataType header, QJsonObject Data)));
}

void ClientServer::ProcessMsg(Connection::DataType header, QJsonObject Data)
{
    switch(header)
    {

    case Connection::R2_verification_sending: emit verification_sending();break;
    case Connection::R3_verification_sent: emit verification_sent();break;
    case Connection::R5_fail: emit Reg_fail(Data); break;
    case Connection::R6_success:
        FriendList = Data.value("friend_list").toArray();
        emit Reg_Success();break;

    case Connection::L2_logging:emit Logging();break;
    case Connection::L3_fail:emit SignInFail();break;
    case Connection::L4_success:
        connector->local_uid=Data.value("your_uid").toInt();
        connector->peer_uid=Data.value("server_uid").toInt();
        emit SignInSuccess();break;
    case Connection::L5_synchro_data:emit synchro_data(Data);break;
        UsrName =Data.value("").toInt();


    case Connection::L6_synchronization_complete:emit synchronization_complete();break;

    case Connection::C2_sychro_history:emit sychro_history(Data);break;
    case Connection::C4_send_message:emit send_message(Data);break;
    default: qDebug()<<"Oops, invalid header!";

    }
}
