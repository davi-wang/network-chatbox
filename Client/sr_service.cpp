#include "sr_service.h"

SR_Service::SR_Service()
{

    status=false;
    m_skt= new QTcpSocket(this);


}
SR_Service::~SR_Service()
{
    delete m_skt;

}

bool SR_Service::Start_Server(QString IP, QString Port)
{
    m_skt->connectToHost(QHostAddress(IP),Port.toUShort());
    connect(m_skt, SIGNAL(connected), this, SLOT(connect_status(bool)));
    if(status)
    {
        CurrentServer=new Server;
        CurrentServer->ServerIP=IP;
        CurrentServer->ServerPort=Port;
        connect_fun=new Connection(m_skt);

        connect(connect_fun,SIGNAL(receiveMessage),this,SLOT(RecieveMsg(DataType header, const QJsonObject &data)));
    }
    return status;
}

bool SR_Service::Halt_Server(){
    m_skt->disconnect();
    connect(m_skt,SIGNAL(disconnected),this,SLOT(disconnect_status(bool)));
    if(!status)
       {
        delete CurrentServer;
        delete connect_fun;
    }
    return !status;
}


void SR_Service::connect_status(){
    status=true;

}
void SR_Service::disconnect_status(){
    status=false;

}


void SR_Service::SendMes(DataType RequirementType,QJsonObject Data)
{
    QJsonObject Mes2send;
    QTime *time = new QTime();


    Mes2send.insert("Type",RequirementType);
    Mes2send.insert("Sender",connect_fun->local_uid);
    Mes2send.insert("Reciever",connect_fun->peer_uid);
    Mes2send.insert("Time",time->currentTime().toString("hh:mm:ss"));
    Mes2send.insert("Data",Data);
    connect_fun->sendMessage(RequirementType,Mes2send);

}

Response SR_Service::SignUP(QString U_name, QString password, QString Email)
{
    QJsonObject SignUP;
    SignUP.insert("U_name",U_name);
    SignUP.insert("Password",password);
    SignUP.insert("U_ID",Email);
}




void SR_Service::RecieveMsg(DataType header, const QJsonObject data)
{
    if(header==Respond_Register||header==Respond_Login)
        flag=true;

}
