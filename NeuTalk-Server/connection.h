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

    // 定义数据包的内容类型，作为报文头(header)
    enum DataType {
        Undefined,
        Ping,
        Pong,
        R1_request_email,
        R2_verification_sending,
        R3_verification_sent,
        R4_request_register,
        R5_fail,
        R6_success,
        L1_request_login,
        L2_logging,
        L3_fail,
        L4_success,
        L5_synchro_data,
        L6_synchronization_complete,
        C1_request_chat,
        C2_sychro_history,
        C3_request_message,
        C4_send_message,
        F1_search_user,
        F2_return_users,
        F3_request_add_friend,
        F4_new_friend,
        F5_request_user_info,
        B0_broadcast_msg,
        B1_join,
        B2_synchro_list,
        B3_leave
        // ...
    };

    // 双方的唯一标识符，由服务器一次性分配，未分配时为-1
    int local_uid;
    int peer_uid;

    // 发送报文的接口
    bool sendMessage(DataType header, const QJsonObject &data);

signals:
    // 接受报文的信号，连接并触发上层槽函数
    void receiveMessage(Connection::DataType, const QJsonObject &);

private slots:
    void connectionUp();
    void processReadyRead();
    void parseMessage();
    void sendPing();
    void sendPong();

private:
    QByteArray encodeDataTypeToHeader(DataType type);
    bool readHeader();

    QTimer ping_timer;
    QTime pong_time;
    DataType current_data_type;
};

#endif // CONNECTION_H
