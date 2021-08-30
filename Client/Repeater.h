#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QObject>
#include"connection.h"
#include<QJsonObject>
#include<QJsonValue>

class ClientServer:public QObject
{
    Q_OBJECT
public:
    static ClientServer* getInstance(QString IP,QString Port) {
            if(instance == nullptr) {
                instance = new ClientServer( IP, Port);
            }
            return instance;
        }

    void SendMsg(Connection::DataType header,QJsonObject &data)
    {
        connector->sendMessage(header,data);
    }


    QJsonArray FriendList;
    QString UsrName;



signals:
    void verification_sending();
    void verification_sent();
    void Reg_fail(QJsonObject Error);
    void Reg_Success();

    void Logging();
    void SignInSuccess();
    void SignInFail();
    void synchro_data(QJsonObject);
    void synchronization_complete();

    void sychro_history(QJsonObject);
    void send_message(QJsonObject);



private:
    static ClientServer* instance;
    ClientServer();
    ClientServer(QString IP,QString Port)
    {
        tcp=new QTcpSocket();
        connect_status=false;
        tcp->connectToHost(IP,Port.toUShort());

        connect(tcp,SIGNAL(&QTcpSocket::connected),this,SLOT(Status_detect()));

    }
    ~ClientServer()
    {
        delete connector;
        delete tcp;
    }
    Connection* connector;
    bool connect_status=false;
    QTcpSocket *tcp;

private slots:
    void ProcessMsg(Connection::DataType header, QJsonObject Data);
    void Status_detect();
};

#endif // CLIENTSERVER_H
