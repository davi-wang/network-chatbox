#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent, QString friend_name, int friend_uid, int my_uid) :
    QWidget(parent), friend_nickname(friend_name), to_uid(friend_uid), from_uid(my_uid),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    client = ClientServer::GetInstance();
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
    data.insert("datetime", QDateTime::currentDateTime().toString());
    data.insert("message", QJsonValue(ui->msgtxtEdit->toPlainText()));
    client->SendMsg(Connection::C3_request_message, data);
}

void ChatWindow::on_sendBtn_clicked()
{
    ClientServer* client = ClientServer::GetInstance();
    sendMessage();
    displayMessage(client->UsrName, QDateTime::currentDateTime().toString(), ui->msgtxtEdit->toPlainText());
    ui->msgtxtEdit->clear();
    ui->msgtxtEdit->setFocus();
}
