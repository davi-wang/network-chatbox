#include "tcpserver.h"

TcpServer::TcpServer()
{

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    Connection *connection = new Connection(this);
    connection->setSocketDescriptor(socketDescriptor);
    emit newConnection(connection);
}
