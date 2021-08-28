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

Widget::Widget(QWidget *parent ,QString name)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    myname=name;

    this->port=9999;//初始化端口
    this->udpSocket=new QUdpSocket(this);//new 套接字

    udpSocket->bind(port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);//绑定端口->实现群聊功能必备
    //大概是采用 shareaddress -> 允许其他的服务器连接到相同的地址和端口
    //         ReuseAddressHint -> 重连服务器（应用于多客户端监听同一个服务器端口超级有效）

  //  connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::ReceiveMessage);

    connect(ui->sendBtn,&QPushButton::clicked,[=](){
       // sndMsg(Msg);
    });

 //   sndMsg(UserEnter);//发送用户上线消息

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


}

void Widget::closeEvent(QCloseEvent *)//处理用户离开
{
    emit this->closeWidget();//当点击"关闭窗口"，就会发送信号，调用closeWidget()函数

    //sndMsg(UserLeft);//发送用户下线消息

    udpSocket->close();
    udpSocket->destroyed();
}


QString Widget::getName()//获取名字
{
    return this->myname;
}


QString Widget::getMsg()//获取聊天信息，重消息发送框
{
    QString msg= ui->msgtxtEdit->toHtml();
    ui->msgtxtEdit->clear();//获取完消息之后清屏
    ui->msgtxtEdit->setFocus();//光标复位
    return msg;
}

//This part should be rewriten
/*

void Widget::sndMsg(Widget::Msgtype type)//广播udp信息
{
    QByteArray array;//造一个数组

    //消息流分析&划分
    QDataStream stream(&array,QIODevice::WriteOnly);//创建流
    stream<<type<<this->getName();//流入类型和用户姓名
    switch(type)//聊天框内整体分三类操作
    {
        case Msg:
            if(this->ui->msgtxtEdit->toPlainText()=="")//判空，规定不能发送空消息
            {
                QMessageBox::warning(this,"警告","发送内容不能为空");
                return;
            }
            stream<<this->getMsg();//流入 用户发的消息
            break;

        case UserEnter://不是聊天内容 不需要流入消息
            break;
        case UserLeft://同上
            break;
    }

    //聊天框报文写入
    udpSocket->writeDatagram(array.data(),array.size(),QHostAddress::Broadcast,this->port);//消息内容||消息规模（长度）||ip地址（广播，发给所有人）||端口
}


void Widget::userEnter(QString username)//处理用户进入
{
     bool isEmpty=ui->tableWidget->findItems(username,Qt::MatchExactly).isEmpty();
     if(isEmpty)
     {
         QTableWidgetItem *table=new QTableWidgetItem(username);
         ui->tableWidget->insertRow(0);
         ui->tableWidget->setItem(0,0,table);
         ui->tableWidget->verticalHeader()->setVisible(false);//垂直表头不显示
         ui->msgBrowser->setTextColor(Qt::gray);
         ui->msgBrowser->append(username+"用户已上线");
         ui->userNumLbl->setText(QString("在线人数：%1人").arg(ui->tableWidget->rowCount()));

         sndMsg(UserEnter);
     }
}


void Widget::userLeft(QString username,QString time)//处理用户离开
{
    bool isEmpty=ui->tableWidget->findItems(username,Qt::MatchExactly).isEmpty();
    if(!isEmpty)
    {
        int row=ui->tableWidget->findItems(username,Qt::MatchExactly).first()->row();

        ui->tableWidget->removeRow(row);

        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->append(username+"用户于"+time+"下线");
        ui->userNumLbl->setText(QString("在线人数：%1人").arg(ui->tableWidget->rowCount()));
    }
}
*/
//This part should be rewriten!!!!!!!!!!
/*
void Widget::ReceiveMessage()//接收udp信息
{
    qint64 size=udpSocket->pendingDatagramSize();//返回数据大小的函数
    int mysize= static_cast<int>(size); //把qint64变成int类型
    QByteArray array=QByteArray(mysize,0);//int类型的size， 0的意思是没东西的时候全部初始化为0
    udpSocket->readDatagram(array.data(),size);//读取消息

    QDataStream stream(&array,QIODevice::ReadOnly);//创建一个流
    int msgtype;
    stream>>msgtype;//读取消息的类型
    QString name,msg;//然后搞用户名和聊天信息
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    switch(msgtype)
    {
        case Msg:
            stream>>name>>msg;

            ui->msgBrowser->setTextColor(Qt::blue);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman",11));
            ui->msgBrowser->append("["+name+"]"+time);
            ui->msgBrowser->append(msg);
            break;
        case UserEnter://不是聊天内容 不需要流入消息
            stream>>name;
            userEnter(name);
            break;
        case UserLeft://同上
            stream>>name;
            userLeft(name,time);
            break;
    }
    return;
}
*/

Widget::~Widget()
{
    delete ui;
}

