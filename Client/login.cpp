#include "login.h"
#include "ui_login.h"
#include <QIcon>
#include <QToolButton>
#include "widget.h"
#include "connection.h"
#include <QMessageBox>

//void login::paintEvent(QPaintEvent *event)
//{
//    Q_UNUSED(event);


//    QPainter painter;
//    painter.begin(this);
//    painter.setRenderHints(QPainter::Antialiasing,true);
//    QPixmap pixmap(":/img/1.jpg");
//    QPainterPath path;
//    path.addEllipse(56,20,120,120);//加入一个圆形   绘图是从左上角的（56，20）坐标开始绘制的  ，120，120是绘图的宽高
//    painter.setClipPath(path);

//    painter.drawPixmap(QRect(56,20,120,120),pixmap);

//    painter.end();
//}

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);


    //框
    this->setWindowIcon(QIcon(":/image/1.jpg"));//设置框的图标 （路径是 “ 冒号+前缀+路径 ” ）
    this->setWindowTitle("chat");//设置名称

    //群成员部分
    QList<QString> nameList;//昵称列表
    nameList<<"111"<<"222"<<"333"<<"444"<<"555"<<"111"<<"222"<<"333"<<"444"<<"555";//这里写的是列表预置的用户昵称
    QStringList iconNameList;//图标资源列表（头像）
    iconNameList<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"5"<<"6"<<"7";;//这里面写的是图片的名字

    QVector<QToolButton*>vector;
    for(int i=0;i<9;i++)
    {
        QToolButton *btn=new QToolButton(this);

        //聊天窗口部分
        btn->setIcon(QPixmap(QString(":/image/%1.png").arg(iconNameList[i])));//成员头像
        btn->setIconSize(QPixmap(QString(":/image/%1.png").arg(iconNameList[i])).size());//设置图片的大小

        //群成员部分
        btn->setText(QString("%1").arg(nameList[i]));//设置网名
        btn->setIconSize(QSize(70,70)); //设置头像大小
        btn->setAutoRaise(true);//设置透明
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置显示格式（头像在单人框里的位置）


        ui->vlayout->addWidget(btn);//把刚刚那一坨放到vlayout布局框里（变得有格式）
        vector.push_back(btn);
        IsShow.push_back(false);//初始化为false->该联系人的聊天窗口未打开
    }

    for(int i=0;i<9;i++)
    {
        connect(vector[i],&QToolButton::clicked,[=]()
        {
            if(IsShow[i])//IsShow为true ->当窗口已经打开
            {
                QMessageBox::warning(this,"警告","该聊天窗口已被打开！");
                return;
            }
            IsShow[i]=true;//窗口未打开,则打开窗口，并设置函数为“已打开状态”
            Widget *widget=new Widget(nullptr,vector[i]->text());
            widget->setWindowIcon(vector[i]->icon());
            widget->setWindowTitle(vector[i]->text());
            widget->show();         

            connect(widget,&Widget::closeWidget,this,[=]()//关闭时重置isshow函数
            {
                IsShow[i]=false;
            });
        });
    }

}

login::~login()
{
    delete ui;
}
