#include "connection.h"

//static const int TransferTimeout = 30 * 1000;
static const int PongTimeout = 60 * 1000;
static const int PingInterval = 5 * 1000;

Connection::Connection(QObject *parent)
    : QTcpSocket(parent)
{
    local_uid = "";
    peer_uid = "";
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
    if (current_data_type == Undefined) {
        if (!readHeader())
            return;
    }
    QByteArray data = readAll();
    QJsonParseError json_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(data, &json_error);
    if(json_error.error == QJsonParseError::NoError) {
        QJsonObject json = json_doc.object();
        switch(current_data_type) {
        case Ping:
            sendPong();
            break;
        case Pong:
            pong_time.restart();
            break;
        case Request_ChatMessage:
            emit receiveMessage(Request_ChatMessage, json);
        // ...
        default:
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
    if (pong_time.elapsed() > PongTimeout) {
        abort();
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
    current_data_type = DataType(int(c) - int('!'));
    return true;
}
