#include "server.h"

Server* Server::instance = nullptr;

Server::Server(QObject *parent) : QObject(parent)
{
    tcp_server = nullptr;
    default_port = "14400";
}

void Server::getHostInfo()
{
    hostadd_list.clear();
    QHostInfo::lookupHost(QHostInfo::localHostName(), this, SLOT(lookedUp(QHostInfo)));
}

void Server::lookedUp(QHostInfo host)
{
    // 将lookupHost返回的QHostInfo中的IP地址填入hostadd_list
    if (host.error() != QHostInfo::NoError) {
        emit displayText("[ERROR] Lookup failed:" + host.errorString());
        return;
    }
    const auto addresses = host.addresses();
    for (const QHostAddress &address : addresses) {
        hostadd_list.append(address);
        emit displayText("[INFO] Found host ip address " + address.toString());
    }
    // 自动开启服务器
    if (!hostadd_list.empty()) {
        startServer(hostadd_list.at(0), default_port);
    }
}

void Server::connectClient(Connection *connection)
{
    connections.append(connection);

    connect(connection, SIGNAL(disconnect()), this, SLOT(closeClient()));
    connect(connection, SIGNAL(receiveMessage(DataType, const QJsonObject &)),
            this, SLOT(processMessage(DataType, const QJsonObject &)));

    QString add = connection->peerAddress().toString();
    emit displayText("[INFO] Client " + add + " connected.");
}

void Server::closeClient()
{
    if (Connection *connection = qobject_cast<Connection *>(sender())) {
        // 需要在列表里删除它的指针！这个列表暂时没有用处
        connection->deleteLater();
        QString add = connection->peerAddress().toString();
        emit displayText("[INFO] Client " + add + " disconnected.");
    }
}

void Server::processMessage(Connection::DataType header, const QJsonObject &data)
{
    if (header == Connection::Request_ChatMessage) {
        QJsonValue send_to_uid = data.value("send_to_uid");
        QString to = send_to_uid.toString();
        // 检查是否在线
        if (true) {
            // 查一个uid到Connection的映射
            Connection *target=nullptr;
            target->sendMessage(Connection::Request_ChatMessage, data);
        }
        // 存到历史记录数据库
    }
    else if (true) {
        // 其他消息处理
    }
}

bool Server::startServer(QHostAddress hostAdd, QString serverPort)
{
    if (tcp_server != nullptr) {
        delete tcp_server;
    }
    tcp_server = new TcpServer();
    if (tcp_server->listen(hostAdd, serverPort.toUInt())) {
        connect(tcp_server, SIGNAL(newConnection(Connection *)),
                this, SLOT(connectClient(Connection *)));
        emit serverIsUp();
        emit displayText("[INFO] Server is up. Listening for new connections.");
        emit displayText("[INFO] Server host ip: " + hostAdd.toString());
        emit displayText("[INFO] Server port: " + serverPort);
        return true;
    }
    else {
        emit displayText("[ERROR] Listen for connections failed. Server is down. ");
        return false;
    }
}

void Server::stopServer()
{
    tcp_server->close();
    delete tcp_server;
    connections.clear();
    emit displayText("[INFO] Server cloesed. ");
}
