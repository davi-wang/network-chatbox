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
    // 根据指定的主机ip和端口号开启TcpServer
    bool startServer(QHostAddress hostadd, QString serverPort);
    void stopServer();
    void getHostInfo();

signals:
    // 传给ui窗口的信号
    void serverIsUp();
    void displayText(const QString &);
    //
    void synchro_friend_list(Connection*);

private slots:
    void lookedUp(QHostInfo host);
    void connectClient(Connection *connection);  // 被TcpServer的信号触发 处理新连接
    void closeClient();  // 被Socket的信号触发，关闭连接
    void processMessage(Connection::DataType, const QJsonObject &);  // 被Connection的信号触发，处理报文
    void synchro_friend_list_for(Connection*);

private:
    explicit Server(QObject *parent = nullptr);
    static Server* instance;

    QString default_port;
    TcpServer *tcp_server;
    QList<Connection*> connections;
    EmailVerify email_verify;
    QMap<int, Connection*> onlines;
};

#endif // SERVER_H
