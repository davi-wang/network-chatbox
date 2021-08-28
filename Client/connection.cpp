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

void Connection::sendMessage(DataType header, const QString &data)
{
    QDataStream out(this);
    out.setVersion(QDataStream::Qt_5_9);
    out << static_cast<qint32>(header);
    out << data;
}

void Connection::processReadyRead()
{
    if (current_data_type == Undefined) {
        if (!readHeader())
            return;
    }
    QString data;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_9);
    in >> data;

    switch(current_data_type) {
    case Ping:
        sendPong();
        break;
    case Pong:
        pong_time.restart();
        break;
    case Request_ChatMessage:
        emit receiveMessage(Request_ChatMessage, data);
    // ...
    default:
        break;
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
        QDataStream out(this);
        out.setVersion(QDataStream::Qt_5_9);
        out << static_cast<qint32>(Ping);
    }
}

void Connection::sendPong()
{
    QDataStream out(this);
    out.setVersion(QDataStream::Qt_5_9);
    out << static_cast<qint32>(Pong);
}

bool Connection::readHeader()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_9);
    qint32 temp = -1;
    in >> temp;
    // 有效性检查，暂时没有
    if (temp == -1) return false;
    current_data_type = static_cast<DataType>(temp);
    return true;
}

