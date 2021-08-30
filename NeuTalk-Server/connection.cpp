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
    connect(this, SIGNAL(disconnect()), &ping_timer, SLOT(stop()));
    connect(&ping_timer, SIGNAL(timeout()), this, SLOT(sendPing()));
    connect(this, SIGNAL(connected()), this, SLOT(connected()));

}

bool Connection::sendMessage(DataType header, const QJsonObject &data)
{
    QByteArray message;
    QJsonDocument json(data);
    QJsonDocument::JsonFormat format = QJsonDocument::Compact;
    message = encodeDataTypeToHeader(header) + json.toJson(format);
    return write(message) == message.size();
}

void Connection::processReadyRead()
{
    // 读取header，验证有效性
    if (current_data_type == Undefined) {
        if (!readHeader())
            return;
    }
    // 读取并解析Json
    QByteArray data = readAll();
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
        qDebug() << "Json parse error: " << json_error.error;
    }
    current_data_type = Undefined;
}

void Connection::connected()
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
