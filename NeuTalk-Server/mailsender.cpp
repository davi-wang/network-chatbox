#include "mailsender.h"

MailSender::MailSender(QObject *parent) : QObject(parent)
{
}

MailSender::~MailSender()
{

}

void MailSender::sendEmail(const QByteArray &recvaddr,
                           const QString &subject,
                           const QString &content)
{
    username = "wmy-zz@163.com";
    password= "NNPWUNMXQIDSSAPP";

    this->recvaddr = recvaddr;
    QByteArray usernametmp = username;
    QByteArray recvaddrtmp = recvaddr;
    clientsocket = new QTcpSocket();
    this->clientsocket->connectToHost("smtp.163.com",25,QTcpSocket::ReadWrite);
    this->clientsocket->waitForConnected(1000);
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] 1 " +recvdata;
    this->clientsocket->write("HELO smtp.163.com\r\n");
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] 2 " +recvdata;
    this->clientsocket->write("AUTH LOGIN\r\n");
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] 3 " +recvdata;
    this->clientsocket->write(username.toBase64().append("\r\n"));
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] 4 " +recvdata;
    this->clientsocket->write(password.toBase64().append("\r\n"));
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] 5 " +recvdata;
    this->clientsocket->write(mailfrom.append(usernametmp.append(">\r\n")));
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] 6 " +recvdata;
    this->clientsocket->write(rcptto.append(recvaddrtmp.append(">\r\n")));
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] 7 " +recvdata;
    this->clientsocket->write("data\r\n");
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] 8 " +recvdata;
    usernametmp = this->username;
    recvaddrtmp = this->recvaddr;
    this->clientsocket->write(prefrom.append(usernametmp.append("\r\n")));
    this->clientsocket->write(preto.append(recvaddrtmp.append("\r\n")));
    this->clientsocket->write(presubject.append(subject.toLocal8Bit().append("\r\n")));
    this->clientsocket->write("\r\n");
    this->clientsocket->write(content.toLocal8Bit().append("\r\n"));
    this->clientsocket->write(".\r\n");
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] 9 " +recvdata;
    this->clientsocket->write("quit\r\n");
    this->clientsocket->waitForReadyRead(1000);
//    recvdata = clientsocket->readAll();
//    qDebug() << "[Email] T " +recvdata;
    clientsocket->close();
    delete clientsocket;
}
