#include "addfrd.h"
#include "ui_addfrd.h"
#include "login.h"

#include<QDebug>

AddFrd::AddFrd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFrd)
{
    ui->setupUi(this);

    //连接信号槽函数
    connect(Repeater, SIGNAL(return_users()), this, SLOT(getF())); //服务器返回查到的匹配用户列表
}

AddFrd::~AddFrd()
{
    delete ui;
}

void AddFrd::on_Find_clicked() //查找按钮
{
    QString info = ui->info->text(); //获取查找信息
    QJsonObject json; //打包json
    json.insert("search", QJsonValue(info)); //打包
    Repeater->SendMsg(Connection::F1_search_user, json); //用户按email/username查询信息

    //废掉按钮
    ui->Find->setEnabled(false);
}

void AddFrd::on_Add_clicked() //添加
{
    //按钮列表
    qDebug()<<"Sending Requirement";
    //调
    QJsonObject json; //json
    json.insert("uid", *Repeater->NewFriend_ID); //打包
     Repeater->SendMsg(Connection::F3_request_add_friend, json); //申请加好友
    //废掉按钮



    ui->Add->setEnabled(false);
     this->close();




 //关闭页面

}

void AddFrd::getF(){ //服务器返回查到的匹配用户列表
    //重新拉取好友信息
    //群成员部分
    QList<QString> nameList;//昵称列表



    for(int i=0;i<(Repeater->NewFriends.size());i++)
    {
        vector.push_back(nullptr);
        vector[i]=new QToolButton(this);

        //群成员部分
        QString arg=Repeater->NewFriends[Repeater->NewFriend_u_ID[i]].NickName;
        vector[i]->setText(arg);//设置网名
        vector[i]->setIconSize(QSize(70,70)); //设置头像大小
        vector[i]->setAutoRaise(true);//设置透明
        vector[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置显示格式（头像在单人框里的位置）

        qDebug()<<"Addition1 Succees";
        ui->List->addWidget(vector[i]);//把刚刚那一坨放到vlayout布局框里（变得有格式）
        vector.push_back(vector[i]);
        IsShow.push_back(false);//初始化为false->该联系人的聊天窗口未打开
        qDebug()<<"Addition2 Succees";

}

        for(int i = 0; i < Repeater->NewFriend_u_ID.size(); i++){ //列表

            connect(vector[i],&QToolButton::clicked,[=]() //click
            {

                int U =Repeater-> NewFriend_u_ID[i]; //获取当前点击的按钮的uid
                Repeater->NewFriend_ID=new int(U);
                //connect


            });
        }

}
