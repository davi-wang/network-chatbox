#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QLayout>
#include <connection.h>
#include "Repeater.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent ,QString name);
   ~Widget();
    // enum Msgtype{Msg,UserEnter,UserLeft};//枚举： 人发的消息内容，用户上线信息提示，用户下线信息提示
    // void closeEvent(QCloseEvent *);  //重载关闭事件
    // void userEnter(QString username);//处理用户进入
    // void userLeft(QString username,QString time);//处理用户离开

signals:
     void closeWidget();//关闭聊天框时，所有相关参数重新归0

private slots:
    void syn_history();
    void on_sendBtn_clicked();
    void receive_msg();

private:
    Ui::Widget *ui;
    ClientServer *Repeater;
};
#endif // WIDGET_H
