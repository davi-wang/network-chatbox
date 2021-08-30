#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "connection.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer();
signals:
    // 重写QTcpServer的新连接信号，以传递Connection指针
    void newConnection(Connection *connection);
private slots:
    // 重写QTcpServer接收新连接的槽函数，以创建Connection实例
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // TCPSERVER_H
