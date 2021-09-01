#include "server.h"

Server* Server::instance = nullptr;

Server::Server(QObject *parent) : QObject(parent)
{
    tcp_server = nullptr;
    default_port = "14400";
    display_socket_debug = false;
    display_json_sent_debug = true;
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
        startServer();
    }
}

void Server::connectClient(Connection *connection)
{
    connect(connection, SIGNAL(disconnected()), this, SLOT(closeClient()));
    connect(connection, SIGNAL(receiveMessage(Connection::DataType, const QJsonObject &)),
            this, SLOT(processMessage(Connection::DataType, const QJsonObject &)));
    emit displayText("[INFO] Client " + connection->peerAddress().toString() + " connected.");
}

void Server::closeClient()
{
    if (Connection *connection = qobject_cast<Connection *>(sender())) {
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
    MySql *database = MySql::gethand();
    Connection* client_connection = qobject_cast<Connection*>(sender());
    if (display_socket_debug) {
        emit displayText("[SOCKET] Server received <" + QString::number(header) +
                         "> from client:" + client_connection->peerAddress().toString());
        qDebug() << "Server received <" << header <<
                    "> from client:" << client_connection->peerAddress().toString();
    }
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
        database->insertSinglehistory(from_uid.toInt(), send_to_uid.toInt(), message, datetime);
    }
    else if (header == Connection::R1_request_email)
    {
        QByteArray user_email = data.value("user_email").toString().toUtf8();
        client_connection->sendMessage(Connection::R2_verification_sending, QJsonObject());
        emit displayText("[INFO] Sending verification email to " + QString(user_email));
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
        qDebug() << error;
        if (error == EmailVerify::NoError) {
            if (!database->queryUserInfo(user_email).empty()) {
                emit displayText("[INFO] user: \"" + QString(user_email) + "\" has successfully registered an account.");
                int new_uid = database->registerUser(user_email, username, password);
                client_connection->sendMessage(Connection::R6_success, QJsonObject());
                emit displayText("[INFO] Allocated uid" + QString::number(new_uid) + " to " + user_email);
            }
            else {
                emit displayText("[INFO] user:" + QString(user_email) + " registration failed. -1");
                QJsonObject reply;
                reply.insert("error", QJsonValue("-1"));
                client_connection->sendMessage(Connection::R5_fail, reply);
            }
        }
        else {
            emit displayText("[INFO] user:" + QString(user_email) + " registration failed. " + int(error));
            QJsonObject reply;
            reply.insert("error", QJsonValue(error));
            client_connection->sendMessage(Connection::R5_fail, reply);
        }
    }
    else if (header == Connection::L1_request_login)
    {
        emit displayText("[INFO] received login request!");
        QByteArray user_email = data.value("user_email").toString().toUtf8();
        QByteArray password = data.value("password").toString().toUtf8();
//        client_connection->sendMessage(Connection::L2_logging, QJsonObject());
        int your_uid = -1;
        if (database->login(user_email, password, your_uid)) {
            QJsonObject reply;
            QString username = database->queryUser(your_uid).value("nickname").toString();
            reply.insert("your_uid", QJsonValue(your_uid));
            reply.insert("server_uid", QJsonValue(int(0)));
            reply.insert("username", QJsonValue(username));
            client_connection->local_uid = 0;
            client_connection->peer_uid = your_uid;
            client_connection->sendMessage(Connection::L4_success, reply);
            onlines[your_uid] = client_connection;

            emit displayText("[INFO] user: \"" + user_email + "\" successfully logged in!");
            emit synchro_friend_list_for(client_connection);
        }
        else {
            client_connection->sendMessage(Connection::L3_fail, QJsonObject());
        }
    }
    else if (header == Connection::C1_request_chat)
    {
        int from_uid = client_connection->peer_uid;
        int to_uid = data.value("request_uid").toInt();
        // 同步数据C2
        emit displayText("[INFO] query chat history between " +
                         QString::number(from_uid) + " - " + QString::number(to_uid));
        QJsonObject sync_data = database->queryHistorylist(from_uid, to_uid);
        client_connection->sendMessage(Connection::C2_sychro_history, sync_data);
    }
    else if (header == Connection::F1_search_user)
    {
        QJsonObject matched_users = database->queryUserInfo(data.value("search").toString());
        client_connection->sendMessage(Connection::F2_return_users, matched_users);
        QJsonDocument doc(matched_users);
        displayText("[SEND JSON] " + doc.toJson());
    }
    else if (header == Connection::F3_request_add_friend)
    {
        int friend_uid = data.value("uid").toInt();
        if (database->addFriends(client_connection->peer_uid, friend_uid)) {
            QDateTime now = QDateTime::currentDateTime();
            database->insertSinglehistory(client_connection->peer_uid, friend_uid,
                                          "Greetings! We are friends now.",
                                          now.toString("yyyy-MM-dd hh:mm:ss"));
            QJsonObject reply, tell;
            reply.insert("new_friend_uid", QJsonValue(int(friend_uid)));
            tell.insert("new_friend_uid", QJsonValue(int(client_connection->peer_uid)));
            client_connection->sendMessage(Connection::F4_new_friend, reply);
            if (onlines.contains(friend_uid)) {
                onlines[friend_uid]->sendMessage(Connection::F4_new_friend, tell);
            }
        }
    }
    else if (header == Connection::F5_request_user_info)
    {
        int query_user = data.value("uid").toInt();
        QJsonObject result = database->queryUser(query_user);
        client_connection->sendMessage(Connection::F5_request_user_info, result);
    }
    else {
        emit displayText("[ERROR] Undefined message received:" + QString::number(header));
    }
}

void Server::synchro_friend_list_for(Connection* client_connection)
{
    emit displayText("[INFO] enter slot");
    if (!onlines.contains(client_connection->peer_uid))
        return;
    // 发送同步数据包L5
    MySql *database = MySql::gethand();
    QJsonObject friend_list_json = database->queryFriendlist(client_connection->peer_uid);
    emit displayText("[INFO] before");
    client_connection->sendMessage(Connection::L5_synchro_data, friend_list_json);
    client_connection->sendMessage(Connection::L6_synchronization_complete, QJsonObject());
    emit displayText("[INFO] after");
}

bool Server::startServer()
{
    if (tcp_server != nullptr) {
        delete tcp_server;
    }
    tcp_server = new TcpServer();
    if (tcp_server->listen(hostadd_list.at(2), default_port.toUInt())) {
        connect(tcp_server, SIGNAL(newConnection(Connection *)),
                this, SLOT(connectClient(Connection *)));
        emit displayText("[INFO] Server is up. Listening for new connections.");
        emit displayText("[INFO] Server host ip: " + hostadd_list.at(2).toString());
        emit displayText("[INFO] Server port: " + default_port);
        return true;
    }
    else {
        emit displayText("[ERROR] Listen for connections failed. Server is down. ");
        return false;
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
    tcp_server = nullptr;
    onlines.clear();
    emit displayText("[INFO] Server cloesed. ");
}
