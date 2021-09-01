#include "login.h"






login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);


    //框
    this->setWindowIcon(QIcon(":/image/1.jpg"));//设置框的图标 （路径是 “ 冒号+前缀+路径 ” ）
    this->setWindowTitle("Mychat");//设置名称
    Repeater = ClientServer::GetInstance();//定义一个repeater指针
    //群成员部分
    int cnt=Repeater->FriendList.size();

    for(int i=0;i<cnt;i++)
    {

        //对 button进行重写
        ShowFriends.push_back(nullptr);
        ShowFriends[i]=new QToolButton(this);

        //群成员部分
        QString arg=Repeater->FriendList[Repeater->Friend_u_IDs[i]].NickName;
        ShowFriends[i]->setText(arg);//设置网名

        ShowFriends[i]->setAutoRaise(true);//设置透明
        ShowFriends[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置显示格式（头像在单人框里的位置）


        ui->vlayout->addWidget(ShowFriends[i]);//把刚刚那一坨放到vlayout布局框里（变得有格式）
        IsShow.push_back(false);//初始化为false->该联系人的聊天窗口未打开
    }
    connect(Repeater,SIGNAL(request_user_info()),this,SLOT(AddFriendFinish()));

    total=cnt;
    for(int i=0;i<cnt;i++)
    {
        qDebug()<<"OpenWindows";
        connect(ShowFriends[i],&QToolButton::clicked,[=]()
        {
           int id=Repeater->Friend_u_IDs[i];
           qDebug()<<"OpenWindows";
           QString Name=Repeater->FriendList[Repeater->Friend_u_IDs[i]].NickName;
           ChatWindows[id]=new ChatWindow(nullptr,Name,id,Repeater->local_uid);
           ChatWindows[id]->show();

        });

    }


}



login::~login()
{
    delete ui;
    int cnt=ShowFriends.size();
    for(int i=0;i<cnt;i++)
    {
        delete ShowFriends[i];
    }
}

void login::on_pushButton_clicked() //加好友
{
      bu = new AddFrd(nullptr); //打开加好友界面
    connect(bu,SIGNAL(request_user_info()),this,SLOT(AddFriendFinish()));

    bu->show();
}



void login::AddFriendFinish()
{
    int NewSize=Repeater->Friend_u_IDs.size();
    for(int i=total;i<NewSize;i++)
    {
        ShowFriends.push_back(nullptr);
    }
    for(int i=total;i<NewSize;i++)
    {

        ShowFriends[i]=new QToolButton(this);

        //群成员部分
        QString arg=Repeater->FriendList[Repeater->Friend_u_IDs[i]].NickName;
        ShowFriends[i]->setText(arg);//设置网名

        ShowFriends[i]->setAutoRaise(true);//设置透明
        ShowFriends[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置显示格式（头像在单人框里的位置）

        qDebug()<<Repeater->Friend_u_IDs[i];
        ui->vlayout->addWidget(ShowFriends[i]);//把刚刚那一坨放到vlayout布局框里（变得有格式）
        IsShow.push_back(false);//初始化为false->该联系人的聊天窗口未打开
        connect(ShowFriends[i],&QToolButton::clicked,[=]() //click
        {

            int id=Repeater->Friend_u_IDs[i];
            QString Name=Repeater->FriendList[Repeater->Friend_u_IDs[i]].NickName;
            ChatWindows[id]=new ChatWindow(this,Name,id,Repeater->local_uid);
            ChatWindows[id]->show();


        });


    }

    total=NewSize;

    if(bu!=nullptr)
    {
        delete bu;
    }

}


