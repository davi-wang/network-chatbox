#include "mailsender.h"

MailSender::MailSender(QWidget *parent)
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
    clientsocket=new QTcpSocket();
    this->clientsocket->connectToHost("smtp.163.com",25,QTcpSocket::ReadWrite);
    this->clientsocket->waitForConnected(1000);
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
    this->clientsocket->write("HELO smtp.163.com\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
    this->clientsocket->write("AUTH LOGIN\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
    qDebug()<<"username:"<<username;
    this->clientsocket->write(username.toBase64().append("\r\n"));
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
    qDebug()<<"password:"<<password;
    this->clientsocket->write(password.toBase64().append("\r\n"));
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
    this->clientsocket->write(mailfrom.append(usernametmp.append(">\r\n")));
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
    //发送邮箱
    //qDebug()<<"mail from:"<<mailfrom.append(usernametmp.append(">\r\n"));
    this->clientsocket->write(rcptto.append(recvaddrtmp.append(">\r\n")));
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
    //接收邮箱
    //qDebug()<<"rcp to:"<<rcptto.append(recvaddrtmp.append(">\r\n"));
    //data表示开始传输数据
    this->clientsocket->write("data\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
    usernametmp = this->username;
    recvaddrtmp = this->recvaddr;
    this->clientsocket->write(prefrom.append(usernametmp.append("\r\n")));
    this->clientsocket->write(preto.append(recvaddrtmp.append("\r\n")));
    this->clientsocket->write(presubject.append(subject.toLocal8Bit().append("\r\n")));
    this->clientsocket->write("\r\n");
    this->clientsocket->write(content.toLocal8Bit().append("\r\n"));
    this->clientsocket->write(".\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
    this->clientsocket->write("quit\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;

    clientsocket->close();
    delete clientsocket;
}
