#include "simpleclientwindow.h"
#include "ui_simpleclientwindow.h"

SimpleClientWindow::SimpleClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SimpleClientWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);


    connection = new Connection();
    connect(connection, SIGNAL(connected()), this, SLOT(connectedToServer()));
    connect(connection, SIGNAL(receiveMessage(Connection::DataType, const QJsonObject &)),
            this, SLOT(process(Connection::DataType, const QJsonObject &)));
}

SimpleClientWindow::~SimpleClientWindow()
{
    delete ui;
}

void SimpleClientWindow::connectedToServer()
{
    ui->textEdit->append("Connected!");
}

void SimpleClientWindow::process(Connection::DataType header, const QJsonObject &data)
{
    QJsonDocument doc(data);
    QByteArray json_array = doc.toJson();
    ui->textEdit->append("received: " + QString::number(header) + json_array);
}

void SimpleClientWindow::on_pushButton_1_clicked()
{
    connection->connectToHost("10.179.166.249", 14400);
}

void SimpleClientWindow::on_pushButton_2_clicked()
{
    QJsonObject json;
    json.insert("user_email", QJsonValue("ffflfffl@126.com"));
    connection->sendMessage(Connection::R1_request_email, json);
}

void SimpleClientWindow::on_pushButton_3_clicked()
{
    QJsonObject json;
    json.insert("user_email", QJsonValue("ffflfffl@126.com")); // 邮箱字符串，保证纯ASCII
    json.insert("username", QJsonValue("debugger"));  // 用户昵称
    json.insert("password", QJsonValue("password"));  // 密码，
    json.insert("verification_code", QJsonValue(ui->textEdit_code->toPlainText()));
    connection->sendMessage(Connection::R4_request_register, json);
}

void SimpleClientWindow::on_pushButton_4_clicked()
{
    QJsonObject json;
    json.insert("user_email", QJsonValue("ffflfffl@126.com"));
    json.insert("password", QJsonValue("password"));
    connection->sendMessage(Connection::L1_request_login, json);
}


void SimpleClientWindow::on_pushButton_5_clicked()
{
    QJsonObject json;
    QDateTime now;
    json.insert("from_uid", QJsonValue(0));
    json.insert("to_uid", QJsonValue(0)); // 填好友的uid
    json.insert("datetime", QJsonValue(now.currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    json.insert("message", QJsonValue("Hello World!"));  // 填发送的聊天消息
    connection->sendMessage(Connection::C3_request_message, json);
}

void SimpleClientWindow::on_pushButton_6_clicked()
{
    QJsonObject json;
    json.insert("request_uid", QJsonValue(int(0)));
    connection->sendMessage(Connection::C1_request_chat, json);
}

void SimpleClientWindow::on_pushButton_7_clicked()
{
    QJsonObject json;
    json.insert("uid", QJsonValue(16739));
    connection->sendMessage(Connection::F2_request_add_friend, json);
}
