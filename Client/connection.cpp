#include "connection.h"

//static const int TransferTimeout = 30 * 1000;
static const int PongTimeout = 60 * 1000;
static const int PingInterval = 5 * 1000;

Connection::Connection(QObject *parent)
    : QTcpSocket(parent)
{
    local_uid = -1;
    peer_uid = -1;
    ping_timer.setInterval(PingInterval);
    current_data_type = Undefined;
//    transfer_timer_id = 0;

    connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
    connect(this, SIGNAL(disconnected()), &ping_timer, SLOT(stop()));
    connect(&ping_timer, SIGNAL(timeout()), this, SLOT(sendPing()));
    connect(this, SIGNAL(connected()), this, SLOT(connectionUp()));

}

bool Connection::sendMessage(DataType header, const QJsonObject &data)
{
    QByteArray message;
    QJsonDocument json(data);
    QJsonDocument::JsonFormat format = QJsonDocument::Compact;
    message = json.toJson(format);
    // 获取JSON串的字节数，然后将该整型数转为固定8位长度的byte array
    int json_length = message.size();
    QByteArray json_size_before_padding = QString::number(json_length).toUtf8();
    QByteArray padding_digits;
    for (int t=0; t < 8 - (json_size_before_padding.size()); t++) {
        padding_digits.append('0');
    }
    QByteArray json_size = padding_digits + json_size_before_padding;
    qDebug() << "json size = " << json_size;
    // 发送定长9字节消息头，再发送JSON串
    QByteArray send = encodeDataTypeToHeader(header) + json_size + message;
    return write(send) == send.size();
}

void Connection::processReadyRead()
{
    // 读取header，验证有效性
    if (current_data_type == Undefined) {
        if (!readHeader())
            return;
    }
    // 读取json_size
    QByteArray json_size_array = read(8);
    if (json_size_array.size() != 8)
        return;
    int json_size = json_size_array.toInt();
    // 读取并解析Json
    QByteArray data = read(json_size);
    if (data.size() != json_size)
        return;
    QJsonParseError json_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(data, &json_error);
    if(json_error.error == QJsonParseError::NoError) {
        QJsonObject json = json_doc.object();
        switch(current_data_type) {
        case Undefined:
            break;
        case Ping:
            sendPong();
            break;
        case Pong:
            pong_time.restart();
            break;
        // -----以下部分可扩展报文类型 自定义Connection层的处理-----
		// 
        // -----------------------------------------------------
        default:
            emit receiveMessage(current_data_type, json);
            break;
        }
    }
    else {
        qDebug() << "Json parse error. error type " << json_error.error;
    }
    current_data_type = Undefined;
}

void Connection::connectionUp()
{
    current_data_type = Undefined;
    ping_timer.start();
    pong_time.start();
}

void Connection::sendPing()
{
    // 附加功能，ping超过某一个时长没收到响应pong，则立即终止连接
    if (pong_time.elapsed() > PongTimeout) {
        abort();
        qDebug() << "Connection is aborted for ping timeout.";
        return;
    }
    else {
        sendMessage(Ping, QJsonObject());
    }
}

void Connection::sendPong()
{
    sendMessage(Pong, QJsonObject());
}

QByteArray Connection::encodeDataTypeToHeader(DataType type)
{
    char byte = '!' + int(type);
    return QByteArray(&byte, 1);
}

bool Connection::readHeader()
{
    char c = read(1).at(0);
    if (c < '!') return false;  // 待增加上限检查
    current_data_type = DataType(int(c) - int('!'));
    return true;
}
