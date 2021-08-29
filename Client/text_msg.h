#ifndef TEXT_MSG_H
#define TEXT_MSG_H

#include <QObject>
#include"connection.h"
#include<QTime>
class Text_Msg:public QObject
{
    Q_OBJECT
public:
    Text_Msg(Connection *connector);
    void SendMes(QString U_localID,QString U_peerID,QJsonObject Data);



private:
    Connection *connector;
    QTime time;

signals:
    void RecieveChat(QString From,QJsonObject Content);


private slots:
    void MesReciever(Connection::DataType,QJsonObject );
};






#endif // TEXT_MSG_H
