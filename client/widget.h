#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent ,QString name);

    void closeEvent(QCloseEvent *);  //重载关闭事件
    enum Msgtype{Msg,UserEnter,UserLeft};//枚举： 人发的消息内容，用户上线信息提示，用户下线信息提示
    void sndMsg(Msgtype type);//广播udp信息
    QString getName();//获取名字
    QString getMsg();//获取聊天信息
    void userEnter(QString username);//处理用户进入
    void userLeft(QString username,QString time);//处理用户离开
    void ReceiveMessage();//接收udp信息

    ~Widget();

signals:
    void closeWidget();//关闭聊天框时，所有相关参数重新归0

private:
    Ui::Widget *ui;
    QString myname;
    quint16 port;//quint16: 16位的无符号整形 定义端口
    QUdpSocket *udpSocket;//udp 套接字
};
#endif // WIDGET_H
