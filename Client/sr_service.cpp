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

        connect(connect_fun,SIGNAL(receiveMessage(DataType,QJsonObject)),this,SLOT(RecieveMsg(DataType header, const QJsonObject &data)));
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
    m_skt->close();
    status=false;

}


void SR_Service::SendMes(Connection::DataType RequirementType,QJsonObject Data)
{
    QJsonObject Mes2send;
    QTime *time = new QTime();

    Mes2send.insert("Sender",connect_fun->local_uid);
    Mes2send.insert("Reciever",connect_fun->peer_uid);
    Mes2send.insert("Time",time->currentTime().toString("hh:mm:ss"));
    Mes2send.insert("Data",Data);
    connect_fun->sendMessage(RequirementType,Mes2send);

}

void SR_Service::Sign()
{
  SignAccount=new SignUp(connect_fun);
  connect(SignAccount,SIGNAL(Success),this,SLOT(Signresponce));
  connect(SignAccount,SIGNAL(Fail),this,SLOT(Signresponce));
}

void SR_Service::SignEND()
{
    delete SignAccount;
}


