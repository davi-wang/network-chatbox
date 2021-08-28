#include "sr_service.h"

SR_Service::SR_Service()
{
    connect_fun=new Connection();
    status=false;
    m_skt= new QTcpSocket(this);


}
SR_Service::~SR_Service()
{
    delete m_skt;
    delete connect_fun;
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
    }
    return status;
}

bool SR_Service::Halt_Server(){
    m_skt->disconnect();
    connect(m_skt,SIGNAL(disconnected),this,SLOT(disconnect_status(bool)));
    if(!status)
        delete CurrentServer;
    return !status;
}


void SR_Service::connect_status(){
    status=true;

}
void SR_Service::disconnect_status(){
    status=false;

}


void SR_Service::SendMes(DataType req,QVector<QString> Data)
{
    QJsonObject Mes2send;
    Mes2send["DataType"]=req;
    for(int index=0;index<Data.count();index++)
    {
        Mes2send[index]=Data[i];
    }

    QString mes;
    mes=req;
    m_skt
}
