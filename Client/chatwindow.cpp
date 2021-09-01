#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent, QString friend_name, int friend_uid, int my_uid) :
    QWidget(parent), friend_nickname(friend_name), to_uid(friend_uid), from_uid(my_uid),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    client = ClientServer::GetInstance();

    setWindowTitle(friend_nickname);

}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::displayMessage(QString username, const QString &datetime, const QString &text)
{
    ui->msgBrowser->setCurrentFont(QFont("Times New Romance", 9));
    ui->msgBrowser->append("["+datetime+"]  "+username);
    ui->msgBrowser->setCurrentFont(QFont("Consolas", 13));
    ui->msgBrowser->append(text);
}

void ChatWindow::newMessage(const QJsonObject &data)
{
    displayMessage(friend_nickname, data.value("datetime").toString(), data.value("message").toString());
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

void ChatWindow::LoadHistory(const QJsonObject &history)
{
    ui->msgBrowser->clear();
    QJsonArray list = history.value("history_list").toArray();
    for (int i=0; i < list.size(); ++i) {
        QJsonObject chat_message = list.at(i).toObject();
        // 如果消息是对方发的
        if (chat_message.value("sender_uid").toString().toInt() == to_uid) {
            displayMessage(friend_nickname,
                           chat_message.value("datetime").toString(),
                           chat_message.value("message").toString());
        }
        else {
            displayMessage(client->UsrName,
                           chat_message.value("datetime").toString(),
                           chat_message.value("message").toString());
        }
    }
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "window closed";
}







