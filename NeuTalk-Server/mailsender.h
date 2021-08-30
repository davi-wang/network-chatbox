#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <QTcpSocket>
#include <QString>

class MailSender : public QObject
{
    Q_OBJECT

public:
    MailSender(QObject *parent = nullptr);
    void sendEmail(const QByteArray &recvaddr, const QString &subject, const QString &content);
    ~MailSender();

private:
    QTcpSocket *clientsocket;
    QByteArray username;  //发送方邮箱
    QByteArray password;  //授权码
    QByteArray mailfrom = "mail from:<";
    QByteArray rcptto = "rcpt to:<";
    QByteArray prefrom = "from:";
    QByteArray preto = "to:";
    QByteArray presubject ="subject:";
    QByteArray recvaddr;  //接收方邮箱
    QByteArray recvdata;  //接收到的数据
};
#endif // MAINWINDOW_H


