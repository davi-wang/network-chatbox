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
        // 客户端下线处理
        if (onlines.contains(connection->peer_uid)) {
            onlines.remove(connection->peer_uid);
        }
        connection->deleteLater();
        QString add = connection->peerAddress().toString();
        emit displayText("[INFO] Client " + add + " disconnected.");
    }
}

void Server::processMessage(Connection::DataType header, const QJsonObject &data)
{
    Connection* client_connection = qobject_cast<Connection*>(sender());
    qDebug() << "Server received <" << header <<
                "> from client:" << client_connection->peerAddress().toString();
    if (header == Connection::C3_request_message)
    {
        QJsonValue send_to_uid = data.value("to_uid");
        QJsonValue from_uid = data.value("from_uid");
        QString message = data.value("message").toString();
        QString datetime = data.value("datetime").toString();
        int to = send_to_uid.toInt();
        if (onlines.contains(to)) {
            Connection *send_to = onlines.value(to);
            send_to->sendMessage(Connection::C4_send_message, data);
        }
        // 存到历史记录数据库
        MySql *database = MySql::gethand();
        database->insertSinglehistory(from_uid.toInt(), send_to_uid.toInt(), message, datetime);
    }
    else if (header == Connection::R1_request_email)
    {
        QByteArray user_email = data.value("user_email").toString().toUtf8();
        client_connection->sendMessage(Connection::R2_verification_sending, QJsonObject());
        this->email_verify.requestVerify(user_email);
        client_connection->sendMessage(Connection::R3_verification_sent, QJsonObject());
    }
    else if (header == Connection::R4_request_register)
    {
        QByteArray user_email = data.value("user_email").toString().toUtf8();
        QString username = data.value("username").toString();
        QByteArray password = data.value("password").toString().toUtf8();
        QByteArray verification_code = data.value("verification_code").toString().toUtf8();
        EmailVerify::VerificationError error;
        error = email_verify.verify(user_email, verification_code);
        if (error == EmailVerify::NoError) {
            MySql* database = MySql::gethand();
            int new_uid = database->registerUser(user_email, username, password);
            client_connection->sendMessage(Connection::R6_success, QJsonObject());
        }
        else {
            QJsonObject reply;
            reply.insert("error", QJsonValue(error));
            client_connection->sendMessage(Connection::R5_fail, reply);
        }
    }
    else if (header == Connection::L1_request_login)
    {
        QByteArray user_email = data.value("user_email").toString().toUtf8();
        QByteArray password = data.value("password").toString().toUtf8();
        client_connection->sendMessage(Connection::L2_logging, QJsonObject());
        MySql* database = MySql::gethand();
        int your_uid = -1;
        if (database->login(user_email, password, your_uid)) {
            QJsonObject reply;
            reply.insert("your_uid", QJsonValue(your_uid));
            reply.insert("server_uid", QJsonValue(int(0)));
            client_connection->local_uid = 0;
            client_connection->peer_uid = your_uid;
            client_connection->sendMessage(Connection::L4_success, reply);
            onlines[your_uid] = client_connection;
            emit synchro_friend_list_for(client_connection);
        }
        else {
            client_connection->sendMessage(Connection::L3_fail, QJsonObject());
        }
    }
    else if (header == Connection::C1_request_chat)
    {
        // 查询历史消息
        // 发送同步数据包C2
    }
    else {
        qDebug() << "Undefined message received:" << header;
    }
}

void Server::synchro_friend_list_for(Connection* client_connection)
{
    // 检查对方是否在线
    if (!onlines.contains(client_connection->peer_uid))
        return;
    // 发送同步数据包L5
    MySql *database = MySql::gethand();
    QJsonObject friend_list_json = database->queryFriendlist(client_connection->peer_uid);
    client_connection->sendMessage(Connection::L5_synchro_data, friend_list_json);
    client_connection->sendMessage(Connection::L6_synchronization_complete, QJsonObject());
}

// temp
void parseFriendList(QJsonObject data)
{
    QJsonArray list = data.value("friend_list").toArray();
    for (int i=0; i < list.size(); ++i) {
        QJsonObject friend_info = list.at(i).toObject();
        // 下面三行是一个好友
        int friend_uid = friend_info.value("uid").toInt();
        QString friend_email = friend_info.value("email").toString();
        QString friend_name = friend_info.value("nickname").toString();
        // 上面三个变量就可以存了
    }
}
void parseHistoryList(QJsonObject data)
{
    QJsonArray list = data.value("history_list").toArray();
    for (int i=0; i < list.size(); ++i) {
        QJsonObject friend_info = list.at(i).toObject();
        // 下面三行是一条聊天消息
        int sender_uid = friend_info.value("sender_uid").toInt();
        int receiver_uid = friend_info.value("receiver_uid").toInt();
        QString datetime = friend_info.value("datetime").toString();
        QString message = friend_info.value("message").toString();
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
