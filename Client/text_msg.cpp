#include "text_msg.h"

Text_Msg::Text_Msg(Connection *connector)
{
    this->connector=connector;


}

void Text_Msg::SendMes(QString U_localID, QString U_peerID, QJsonObject Data)
{
    connector->peer_uid=U_peerID;
    connector->local_uid=U_localID;

    QJsonObject sender;
    sender.insert("Time",time.currentTime().toString("hh:mm:ss"));
    sender.insert("Reciever",connector->peer_uid);
    sender.insert("Sender",connector->local_uid);
    sender.insert("DataType","Text");
    sender.insert("Data",Data);

    connector->sendMessage(Connection::C3_request_message,sender);
}

void Text_Msg::MesReciever(Connection::DataType header,QJsonObject Data)
{
    if(header==Connection::C4_send_message)
    {
        QString From=Data.value("Sender");
        emit RecieveChat(From,Data.value("Data"));
    }
}
