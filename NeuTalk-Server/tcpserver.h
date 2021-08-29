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
    void newConnection(Connection *connection);
private slots:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // TCPSERVER_H
