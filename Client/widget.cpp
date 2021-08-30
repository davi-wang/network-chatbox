#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QDataStream>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QUdpSocket>
#include <QLayout>
#include <QJsonObject>
#include <QJsonValue>
#include <connection.h>

Widget::Widget(QWidget *parent ,QString name)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    myname=name;
    Connection *server;
    connect(server, SIGNAL(receiveMessage(DataType, const QJsonObject &)),
                this, SLOT(processMessage(DataType, const QJsonObject &)));



}

//void Widget::closeEvent(QCloseEvent *)//处理用户离开
//{
//    emit this->closeWidget();//当点击"关闭窗口"，就会发送信号，调用closeWidget()函数

//    sndMsg(UserLeft);//发送用户下线消息

//    udpSocket->close();
//    udpSocket->destroyed();
//}


//QString Widget::getName()//获取名字
//{
//    return this->myname;
//}


//QString Widget::getMsg()//获取聊天信息，重消息发送框
//{
//    QString msg= ui->msgtxtEdit->toHtml();
//    ui->msgtxtEdit->clear();//获取完消息之后清屏
//    ui->msgtxtEdit->setFocus();//光标复位
//    return msg;
//}


//void Widget::userEnter(QString username)//处理用户进入
//{
//     bool isEmpty=ui->tableWidget->findItems(username,Qt::MatchExactly).isEmpty();
//     if(isEmpty)
//     {
//         QTableWidgetItem *table=new QTableWidgetItem(username);
//         ui->tableWidget->insertRow(0);
//         ui->tableWidget->setItem(0,0,table);
//         ui->tableWidget->verticalHeader()->setVisible(false);//垂直表头不显示
//         ui->msgBrowser->setTextColor(Qt::gray);
//         ui->msgBrowser->append(username+"用户已上线");
//         ui->userNumLbl->setText(QString("在线人数：%1人").arg(ui->tableWidget->rowCount()));

//         sndMsg(UserEnter);
//     }
//}


//void Widget::userLeft(QString username,QString time)//处理用户离开
//{
//    bool isEmpty=ui->tableWidget->findItems(username,Qt::MatchExactly).isEmpty();
//    if(!isEmpty)
//    {
//        int row=ui->tableWidget->findItems(username,Qt::MatchExactly).first()->row();

//        ui->tableWidget->removeRow(row);

//        ui->msgBrowser->setTextColor(Qt::gray);
//        ui->msgBrowser->append(username+"用户于"+time+"下线");
//        ui->userNumLbl->setText(QString("在线人数：%1人").arg(ui->tableWidget->rowCount()));
//    }
//}


void Widget::ReceiveMessagee(Connection::DataType ttype, const QJsonObject &json)//接收消息
{

    //解析消息
    int friend_uid = json.value("from_uid").toInt();  // 是谁发给我的消息
    QString new_message = json.value("message").toString();  // 聊天内容
    QString datetime_str = json.value("datetime").toString();  // 什么时候发的，字符串格式    
        QDateTime datetime = QDateTime::fromString(datetime_str, "yyyy-MM-dd hh:mm:ss");  // 什么时候发的格式处理
    bool IfBold=json.value("IfBold").toBool();//加粗
    bool IfUnderline =json.value("IfUnderline").toBool();//下划线
    bool IfItalic =json.value("IfItalic").toBool();//倾斜
    QFont style=json.value("style").toString();//字体
    QString size=json.value("size").toString();//字号
    QColor color
    
    
   

    //根据消息开始处理
    ui->msgBrowser->setTextColor(Qt::black);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",11));
    ui->msgBrowser->append("["+name+"]"+datetime);//已知ID 从服务器搞昵称（todo
    ui->msgBrowser->append(new_message);

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

    ui->msgBrowser->setFontPointSize(size.toDouble()); //改变字体大小
    ui->msgBrowser->setFontFamily(style.toString());//改变字体样式
    ui->msgBrowser->setTextColor(color);//改变字的颜色

}


void Widget::on_sendBtn_clicked()//发送消息
{
    if(this->ui->msgtxtEdit->toPlainText()=="")//判空，规定不能发送空消息
    {
        QMessageBox::warning(this,"警告","发送内容不能为空");
        return;
    }
    //判定可以发送消息，然后开始呈现在自己的代码里，然后打包

    
    
    //呈现在自己的代码里
    connect(ui->sendBtn,&QPushButton::clicked,[=](){
        sndMsg(Msg);
    });

    sndMsg(UserEnter);//发送用户上线消息

    connect(ui->exitBtn,&QPushButton::clicked,[=]{
        this->close();
    });

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

    connect(ui->saveTBtn,&QToolButton::clicked,[=]()//保存聊天记录
    {
        if(ui->msgBrowser->document()->isEmpty())
        {
            QMessageBox::warning(this,"警告","保存内容不能为空！");
        }
        else{
            QString filename=QFileDialog::getSaveFileName(this,"保存聊天记录","聊天记录","*.txt");
            if(!filename.isEmpty())//保存文件名不为空才进行保存
            {
                QFile file(filename);
                file.open(QIODevice::WriteOnly|QFile::Text);
                QTextStream stream(&file);
                stream<<ui->msgBrowser->toPlainText();
                file.close();
            }

        }
    });

 

    //获取所有需要的信息
    bool IfBold = ui->boldTBtn->isChecked();//加粗
    bool IfUnderline = ui->italicTbtn->isChecked();//倾斜
    bool IfItalic = ui->underlineTBtn->isChecked();//下划线
    QFont style=ui->fontCbx->currentFont();//字体
    QString size= ui->sizeCbx->currentText();//字号
    QColor color=QColorDialog::getColor(color,ui->colorTBtn);//颜色

    QString msg=ui->msgtxtEdit->toPlainText().toUtf8();//消息内容
    QDateTime now;//时间

    //开始打包
    QJsonObject json;

    json.insert("IfBold",QJsonValue(IfBold));//加粗
    json.insert("IfItalic",QJsonValue(IfItalic));//倾斜on.insert("to_uid", QJsonValue(friend_uid)); // 填好友的uid
    json.insert("datetime", QJsonValue(now.currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    json.insert("message", QJsonValue(msg));  // 填发送的聊天消息
    json.insert("IfUnderline",QJsonValue(IfUnderline));//下划线
    json.insert("color",QJsonValue(color));

    Connection *server;
    server->sendMessage(Connection::C4_send_message, json);
}


Widget::~Widget()
{
    delete ui;
}

