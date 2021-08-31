#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "tcpserver.h"
#include "connection.h"
#include "emailverify.h"
#include "mysql.h"
#include "serverwindow.h"

class Server : public QObject
{
    Q_OBJECT
public:
    static Server* getInstance() {
        if (instance == nullptr)
            instance = new Server();
        return instance;
    }
    // 备选的主机ip列表
    QList<QHostAddress> hostadd_list;
    bool startServer();  // 使用默认主机ip和默认端口
    bool startServer(QHostAddress hostadd, QString serverPort);  // 根据指定的主机ip和端口号开启
    void stopServer();
    void getHostInfo();  // 获取主机网络信息，然后自动开启服务器
    friend class ServerWindow;

signals:
    void displayText(const QString &);
    void synchro_friend_list(Connection*);

private slots:
    void lookedUp(QHostInfo);
    void connectClient(Connection *);  // 被TcpServer的信号触发 处理新连接
    void closeClient();  // 被Socket的信号触发，关闭连接
    void processMessage(Connection::DataType, const QJsonObject &);  // 被Connection的信号触发，处理报文
    void synchro_friend_list_for(Connection*);

private:
    explicit Server(QObject *parent = nullptr);
    static Server* instance;

    bool display_socket_debug;
    bool display_json_sent_debug;
    QString default_port;
    TcpServer *tcp_server;
//    QList<Connection*> connections;
    EmailVerify email_verify;
    QMap<int, Connection*> onlines;
};

#endif // SERVER_H
