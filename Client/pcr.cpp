#include "pcr.h"
#include "ui_pcr.h"

PCR::PCR(QWidget *parent, int my_uid) :
    QWidget(parent), my_uid(my_uid),
    ui(new Ui::PCR)
{
    ui->setupUi(this);
    //需要获取当前聊天室内成员
    ui->onlinemembers->setReadOnly(true);
    ui->onlinemembers->setCurrentFont(QFont("楷体", 15));
    ui->onlinemembers->append("公共聊天室");
    ClientServer* client = ClientServer::GetInstance();
    connect(client,SIGNAL(NewMsgInPCR(const QJsonObject &)),
            this,SLOT(newMessage(const QJsonObject &)));
    connect(client,SIGNAL(SynList(const QJsonObject &)),
            this,SLOT(updateList(const QJsonObject &)));
    setWindowTitle("Public Chat Room");
    //文本编辑框中选中字体的加粗、倾斜、下划线和清除功能
        connect(ui->fontCbx,&QFontComboBox::currentFontChanged,[=](const QFont &font){
            ui->msgtxtEdit->setFontFamily(font.toString());
            ui->msgtxtEdit->setFocus();
        });
        void (QComboBox:: *sizebtn)(const QString &text)=&QComboBox::currentTextChanged;
        connect(ui->sizeCbx,sizebtn,[=](const QString &text){
            ui->msgtxtEdit->setFontPointSize(text.toDouble());
            ui->msgtxtEdit->setFocus();
        });
        connect(ui->boldTBtn,&QToolButton::clicked,[=](bool checked){
            if(checked)
            {
                QFont cc;
                ui->msgtxtEdit->setFontWeight(cc.bold());
            }
            else{
                ui->msgtxtEdit->setFontWeight(QFont::Normal);
            }
        });
        connect(ui->italicTbtn,&QToolButton::clicked,[=](bool checked){
            ui->msgtxtEdit->setFontItalic(checked);
            ui->msgtxtEdit->setFocus();
        });
        connect(ui->underlineTBtn,&QToolButton::clicked,[=](bool checked){
            ui->msgtxtEdit->setFontUnderline(checked);
            ui->msgtxtEdit->setFocus();
        });
        connect(ui->clearTBtn,&QToolButton::clicked,[=](){
            ui->msgtxtEdit->clear();
        });
}

PCR::~PCR()
{
    delete ui;
}

void PCR::closeEvent(QCloseEvent *event)
{
    //发送退出聊天室的信号
    QJsonObject json;
    ClientServer* client = ClientServer::GetInstance();
    json.insert("uid", QJsonValue(client->local_uid));
    json.insert("username",client->UsrName);
    client->SendMsg(Connection::B3_leave, json);
    qDebug() << "you have exited the chat room !";
}

void PCR::on_sendBtn_clicked()
{
    ClientServer* client = ClientServer::GetInstance();
    sendMessage();
    ui->msgtxtEdit->clear();
    ui->msgtxtEdit->setFocus();
}

void PCR::newMessage(const QJsonObject & data)//接收其他用户的消息
{
    displayMessage(data.value("sender_name").toString(), data.value("datetime").toString(), data.value("message").toString());
}

void PCR::sendMessage(){
    ClientServer* client = ClientServer::GetInstance();
    QJsonObject data;
    data.insert("sender_uid", QJsonValue(my_uid));
    data.insert("sender_name",QJsonValue(client->UsrName));
    data.insert("datetime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    data.insert("message", QJsonValue(ui->msgtxtEdit->toPlainText()));
    client->SendMsg(Connection::B0_broadcast_msg, data);
}

void PCR::displayMessage(QString username, const QString &datetime, const QString &text)
{
    ui->msgBrowser->setCurrentFont(QFont("Times New Romance"));
    ui->msgBrowser->append("["+datetime+"]  "+username);
    ui->msgBrowser->setCurrentFont(QFont("Consolas"));
    ui->msgBrowser->append(text);
}

void PCR::updateList(const QJsonObject &data)
{
    ui->onlinemembers->clear();
    ui->onlinemembers->setCurrentFont(QFont("楷体", 15));
    ui->onlinemembers->append("公共聊天室");
    QJsonArray list = data.value("online_list").toArray();
    for (int i=0; i<list.size(); ++i) {
        qDebug()<<i;
        QString username = list.at(i).toString();
        ui->onlinemembers->append(username);
    }
}
