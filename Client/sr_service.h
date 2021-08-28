#ifndef SR_SERVICE_H
#define SR_SERVICE_H

#include <QObject>
#include"connection.h"
#include"QString"
#include<QHostAddress>
#include<QJsonObject>
#include<QJsonDocument>
#include<QVector>
struct Server{
    QString ServerIP;
    QString ServerPort;
};

enum DataType {
   Regitser,
   Login,
   SendMes,

    // ...
};


class SR_Service: QObject
{
    Q_OBJECT
public:
    SR_Service();
    bool Start_Server(QString IP,QString Port);
    bool Halt_Server();
    ~SR_Service();

private:
    Server* CurrentServer;
    bool status;
    Connection*connect_fun;
    QTcpSocket* m_skt;
    QJsonObject* Message;
    void SendMes(DataType req,QVector<QString> Data);

private slots:
    void connect_status();
    void disconnect_status();

};

#endif // SR_SERVICE_H
