#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "QColorDialog"

ChatWindow::ChatWindow(QWidget *parent, QString friend_name, int friend_uid, int my_uid) :
    QWidget(parent), friend_nickname(friend_name), to_uid(friend_uid), from_uid(my_uid),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    client = ClientServer::GetInstance();

    setWindowTitle("MyChat"); //设置标题
    setWindowIcon(QPixmap(":/image/TuBiao.png")); //设置图标
    //文本编辑框中选中字体的加粗、倾斜、下划线和清除功能
    connect(ui->fontCbx,&QFontComboBox::currentFontChanged,[=](const QFont &font)//字体
        {
            ui->msgtxtEdit->setFontFamily(font.toString());
            ui->msgtxtEdit->setFocus();
        });

        void (QComboBox:: *sizebtn)(const QString &text)=&QComboBox::currentTextChanged;//字体大小
        connect(ui->sizeCbx,sizebtn,[=](const QString &text){
            ui->msgtxtEdit->setFontPointSize(text.toDouble());
            ui->msgtxtEdit->setFocus();
        });

        connect(ui->boldTBtn,&QToolButton::clicked,this,[=](bool checked)//加粗
        {
            if(checked)
            {
                ui->msgtxtEdit->setFontWeight(QFont::Bold);
            }
            else
            {
                 ui->msgtxtEdit->setFontWeight(QFont::Normal);
            }
        });

        connect(ui->italicTbtn,&QToolButton::clicked,this,[=](bool checked)//倾斜
        {
            ui->msgtxtEdit->setFontItalic(checked);
            ui->msgtxtEdit->setFocus();
        });

        connect(ui->underlineTBtn,&QToolButton::clicked,this,[=](bool checked)//下划线
        {
            ui->msgtxtEdit->setFontUnderline(checked);
            ui->msgtxtEdit->setFocus();
        });

        connect(ui->colorTBtn,&QToolButton::clicked,this,[=](bool checked)//更改字的颜色
        {
            QColor color=QColorDialog::getColor(color,this);

            ui->msgtxtEdit->setTextColor(color);
        });

        connect(ui->clearTBtn,&QToolButton::clicked,[=]()//清空聊天记录
        {
            ui->msgtxtEdit->clear();
        });

    ui->label_2->setText(friend_nickname);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::displayMessage(QString username, const QString &datetime, const QString &text,const QString &size,const int &IfBold,const int IfItalic,const int IfUnderline)
{
    ui->msgBrowser->setCurrentFont(QFont("Times New Romance", 9));
    ui->msgBrowser->append("["+datetime+"]  "+username);
    ui->msgBrowser->setCurrentFont(QFont("Consolas", 13));
    ui->msgBrowser->append(text);
    ui->msgBrowser->setFontPointSize(size.toDouble()); //改变字体大小
    if(IfBold)//加粗
    {
        ui->msgBrowser->setFontWeight(QFont::Bold);
    }
    else
    {
        ui->msgBrowser->setFontWeight(QFont::Normal);
    }

    if(IfItalic)//倾斜
    {
        ui->msgBrowser->setFontItalic(1);
        ui->msgBrowser->setFocus();
    }

    if(IfUnderline)//下划线
    {
        ui->msgBrowser->setFontUnderline(1);
        ui->msgBrowser->setFocus();
    }
}

void ChatWindow::newMessage(const QJsonObject &data)
{
    displayMessage(friend_nickname, data.value("datetime").toString(), data.value("message").toString(),data.value("size").toString(),data.value("IfBold").toInt(),data.value("IfItalic").toInt(),data.value("IfUnderline").toInt());
}

void ChatWindow::sendMessage()
{
    QJsonObject data;
    data.insert("from_uid", QJsonValue(client->local_uid));
    data.insert("to_uid", QJsonValue(to_uid));
    data.insert("datetime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    data.insert("message", QJsonValue(ui->msgtxtEdit->toPlainText()));
    data.insert("size",QJsonValue(ui->sizeCbx->currentText()));
    data.insert("IfBold",QJsonValue(ui->boldTBtn->isChecked()));//加粗
    data.insert("IfItalic",QJsonValue(ui->italicTbtn->isChecked()));//倾斜
    data.insert("IfUnderline",QJsonValue(ui->underlineTBtn->isChecked()));//下划线
    client->SendMsg(Connection::C3_request_message, data);

}

void ChatWindow::on_sendBtn_clicked()
{
    ClientServer* client = ClientServer::GetInstance();
    sendMessage();
    displayMessage(client->UsrName, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), ui->msgtxtEdit->toPlainText(),ui->sizeCbx->currentText(),ui->boldTBtn->isChecked(),ui->italicTbtn->isChecked(),ui->underlineTBtn->isChecked());

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
                           chat_message.value("message").toString(),
                           chat_message.value("size").toString(),
                           chat_message.value("IfBold").toBool(),
                           chat_message.value("IfItalic").toBool(),
                           chat_message.value("IfUnderline").toBool());
        }
        else {
            displayMessage(client->UsrName,
                           chat_message.value("datetime").toString(),
                           chat_message.value("message").toString(),
                           chat_message.value("size").toString(),
                           chat_message.value("IfBold").toBool(),
                           chat_message.value("IfItalic").toBool(),
                           chat_message.value("IfUnderline").toBool());
        }
    }
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "window closed";
}







