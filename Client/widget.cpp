#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QUdpSocket>
#include <QLayout>
#include <QJsonObject>
#include <QJsonValue>
#include <connection.h>
#include <Repeater.h>

Widget::Widget(QWidget *parent, QString name)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString IP, Port;
    Repeater = ClientServer::CreateInstance(IP, Port);
    connect(Repeater, SIGNAL(sychro_history()), this, SLOT(syn_history())); //发送消息
    connect(Repeater, SIGNAL(send_message()), this, SLOT(receive_msg()));   //收到消息

    connect(ui->exitBtn, &QPushButton::clicked, [=] //退出按钮
            { this->close(); });

    connect(ui->clearTBtn, &QToolButton::clicked, [=]() //清空聊天记录
            { ui->msgtxtEdit->clear(); });

    connect(ui->saveTBtn, &QToolButton::clicked, [=]() //保存聊天记录
            {
                if (ui->msgBrowser->document()->isEmpty())
                {
                    QMessageBox::warning(this, "警告", "保存内容不能为空！");
                }
                else
                {
                    QString filename = QFileDialog::getSaveFileName(this, "保存聊天记录", "聊天记录", "*.txt");
                    if (!filename.isEmpty()) //保存文件名不为空才进行保存
                    {
                        QFile file(filename);
                        file.open(QIODevice::WriteOnly | QFile::Text);
                        QTextStream stream(&file);
                        stream << ui->msgBrowser->toPlainText();
                        file.close();
                    }
                }
            });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_sendBtn_clicked() //发送消息
{
    //判空，规定不能发送空消息
    if (this->ui->msgtxtEdit->toPlainText() == "")
    {
        QMessageBox::warning(this, "警告", "发送内容不能为空");
        return;
    }

    //->判定可以发送消息，然后开始打包并且 呈现在自己的聊天窗里

    //把发送的内容 呈现在自己的聊天框里
    connect(ui->fontCbx, &QFontComboBox::currentFontChanged, [=](const QFont &font) //字体
            {
                ui->msgtxtEdit->setFontFamily(font.toString());
                ui->msgtxtEdit->setFocus();
            });

    void (QComboBox::*sizebtn)(const QString &text) = &QComboBox::currentTextChanged; //字体大小
    connect(ui->sizeCbx, sizebtn, [=](const QString &text)
            {
                ui->msgtxtEdit->setFontPointSize(text.toDouble());
                ui->msgtxtEdit->setFocus();
            });

    connect(ui->boldTBtn, &QToolButton::clicked, this, [=](bool checked) //加粗
            {
                if (checked)
                {
                    ui->msgtxtEdit->setFontWeight(QFont::Bold);
                }
                else
                {
                    ui->msgtxtEdit->setFontWeight(QFont::Normal);
                }
            });

    connect(ui->italicTbtn, &QToolButton::clicked, this, [=](bool checked) //倾斜
            {
                ui->msgtxtEdit->setFontItalic(checked);
                ui->msgtxtEdit->setFocus();
            });

    connect(ui->underlineTBtn, &QToolButton::clicked, this, [=](bool checked) //下划线
            {
                ui->msgtxtEdit->setFontUnderline(checked);
                ui->msgtxtEdit->setFocus();
            });

    connect(ui->colorTBtn, &QToolButton::clicked, this, [=](bool checked) //更改字的颜色
            {
                QColor color = QColorDialog::getColor(color, this);

                ui->msgtxtEdit->setTextColor(color);
            });

    //获取所有需要的信息，准备打包发送
    bool IfBold = ui->boldTBtn->isChecked();                     //加粗
    bool IfUnderline = ui->italicTbtn->isChecked();              //倾斜
    bool IfItalic = ui->underlineTBtn->isChecked();              //下划线
    QFont style = ui->fontCbx->currentFont();                    //字体
    QString size = ui->sizeCbx->currentText();                   //字号
    QColor color = QColorDialog::getColor(color, ui->colorTBtn); //颜色
    QString msg = ui->msgtxtEdit->toPlainText().toUtf8();        //消息内容
    QDateTime now;                                               //时间

    //开始打包
    QJsonObject json;
    json.insert("IfBold", QJsonValue(IfBold));     //加粗
    json.insert("IfItalic", QJsonValue(IfItalic)); //倾斜on.insert("to_uid", QJsonValue(friend_uid)); // 填好友的uid
    json.insert("datetime", QJsonValue(now.currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    json.insert("message", QJsonValue(msg));             // 填发送的聊天消息
    json.insert("IfUnderline", QJsonValue(IfUnderline)); //下划线
                                                         //    json.insert("color",QJsonValue(color));

    Repeater->SendMsg(Connection::C4_send_message, json); //发出去
}

void Widget::receive_msg()
{
    //      Repeater *data =Repeater::get();

    //      int friend_uid = data->friend_uid; // 是谁发给我的消息
    //      QString new_message =data->msg; // 聊天内容
    //      QString datetime_str = data->datatime; // 什么时候发的，字符串格式
    //      QDateTime datetime = QDateTime::fromString(datetime_str, "yyyy-MM-dd hh:mm:ss");  // 什么时候发的格式处理
    //      bool IfBold=data->IfBold;//加粗
    //      bool IfUnderline =data.IfUnderline;//下划线
    //      bool IfItalic =data->IfItalic;//倾斜
    //      QFont style= data->style;//字体
    //      QString size=data->size;//字号
    // //      QColor color

    //      //根据消息开始处理
    //      ui->msgBrowser->setTextColor(Qt::black);
    //      ui->msgBrowser->setCurrentFont(QFont("Times New Roman",11));
    //      ui->msgBrowser->append("["+data->name+"]"+datetime);//已知ID 从服务器搞昵称（todo
    //      ui->msgBrowser->append(new_message);

    //      if(IfBold)//加粗
    //      {
    //          ui->msgBrowser->setFontWeight(QFont::Bold);
    //      }
    //      else
    //      {
    //           ui->msgBrowser->setFontWeight(QFont::Normal);
    //      }

    //      if(IfItalic)//倾斜
    //      {
    //          ui->msgBrowser->setFontItalic(1);
    //          ui->msgBrowser->setFocus();
    //      }

    //      if(IfUnderline)//下划线
    //      {
    //          ui->msgBrowser->setFontUnderline(1);
    //          ui->msgBrowser->setFocus();
    //      }

    //      ui->msgBrowser->setFontPointSize(size.toDouble()); //改变字体大小
    //      ui->msgBrowser->setFontFamily(style.toString());//改变字体样式
    ////      ui->msgBrowser->setTextColor(color);//改变字的颜色
}

void Widget::syn_history() //同步历史消息
{
}
