#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent, QString friend_name, int friend_uid, int my_uid) :
    QWidget(parent), friend_nickname(friend_name), to_uid(friend_uid), from_uid(my_uid),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    client = ClientServer::GetInstance();
    connect(client,SIGNAL(sychro_history(QJsonObject)),this,SLOT(LoadHistory(QJsonObject)));
    setWindowTitle(friend_nickname);



    connect(client,SIGNAL(send_message()),this,SLOT(newMessage()));


}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::displayMessage(QString username, const QString &datetime, const QString &text)
{
    ui->msgBrowser->setCurrentFont(QFont("Times New Romance"));
    ui->msgBrowser->append("["+datetime+"]  "+username);
    ui->msgBrowser->setCurrentFont(QFont("Consolas"));
    ui->msgBrowser->append(text);
}

void ChatWindow::newMessage()
{


    displayMessage(friend_nickname, client->GetTextMsg.datetime_str, client->GetTextMsg.new_message);
}

void ChatWindow::sendMessage()
{

    QJsonObject data;
    data.insert("from_uid", QJsonValue(client->local_uid));
    data.insert("to_uid", QJsonValue(to_uid));
    data.insert("datetime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    data.insert("message", QJsonValue(ui->msgtxtEdit->toPlainText()));
    client->SendMsg(Connection::C3_request_message, data);
}

void ChatWindow::on_sendBtn_clicked()
{
    ClientServer* client = ClientServer::GetInstance();
    sendMessage();
    displayMessage(client->UsrName, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), ui->msgtxtEdit->toPlainText());
    ui->msgtxtEdit->clear();
    ui->msgtxtEdit->setFocus();
}


void ChatWindow::LoadHistory(QJsonObject history)
{
    if(his.empty())
    {
     qDebug()<<"Enter LoadHistory";

     QJsonArray list = history.value("history_list").toArray();
    for (int i=0; i < list.size(); ++i) {
            QJsonObject friend_info = list.at(i).toObject();
            // 下面三行是一条聊天消息
            his.push_back(friend_info);
        }
        int cnt=his.count();
        for(int i=0;i<cnt;i++)
         {

            QJsonObject friend_info=his[i];
            ChatRecord Row;
            Row.sender_uid = friend_info.value("sender_uid").toString().toInt();
            Row.receiver_uid = friend_info.value("receiver_uid").toString().toInt();
            Row.datetime = friend_info.value("datetime").toString();
            Row.message = friend_info.value("message").toString();

            if(Row.sender_uid==client->local_uid)
            {

                displayMessage(client->UsrName,Row.datetime,Row.message);

            }
            if(Row.sender_uid==to_uid)
            {
                 displayMessage(client->FriendList[to_uid].NickName,Row.datetime,Row.message);
            }
        }
    }
}


void ChatWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "window closed";
}







