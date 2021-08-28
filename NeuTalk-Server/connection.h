#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QTime>
#include <QTimerEvent>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>


class Connection : public QTcpSocket
{
    Q_OBJECT
public:
    Connection(QObject *parent = nullptr);

    // 定义数据包的内容类型，作为报文头
    enum DataType {
        Undefined,
        Ping,
        Pong,
        Request_Register, Respond_Register,
        Request_Login, Respond_Login,
        Request_ChatMessage, Respond_ChatMessage,
        // ...
    };

    // 双方的唯一标识符，由服务器一次性分配，初始化为空（代表未登录）
    QString local_uid;
    QString peer_uid;

    // 发送报文的接口
    bool sendMessage(DataType header, const QJsonObject &data);

signals:
    // 接受报文的信号，连接并触发上层槽函数
    void receiveMessage(DataType header, const QJsonObject &data);

private slots:
    void connected();
    void processReadyRead();
    void sendPing();
    void sendPong();

private:
    QByteArray encodeDataTypeToHeader(DataType type);
    bool readHeader();

    QTimer ping_timer;
    QTime pong_time;
    DataType current_data_type;
//    int transfer_timer_id;
};

#endif // CONNECTION_H
