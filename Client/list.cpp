#include "list.h"
#include "ui_list.h"
#include "QToolButton"
#include "QString"

List::List(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::List)
{
    ui->setupUi(this);

    setWindowTitle("MyChat"); //设置标题
    setWindowIcon(QPixmap(":/image/TuBiao.png")); //设置图标

    //准备图标
    QStringList NameList; //昵称
    NameList<<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G";

    QStringList PicNameList; //头像
    PicNameList <<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7";

    QVector<QToolButton*> MyToolButton; //vector

    for(int i = 0; i < 7; i++ ){
        QToolButton * head_prt = new QToolButton;
        head_prt->setText(NameList.at(i)); //设置文字
        QString str = QString(":/image/%1.png").arg(PicNameList.at(i));
        head_prt->setIcon(QPixmap(str)); //设置头像
        head_prt->setIconSize(QSize(75,75)); //设置头像大小
        head_prt->setAutoRaise(true); //设置按钮风格
        head_prt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); //设置文字和图片一起显示

        ui->vLatout->addWidget(head_prt); //加到垂直布局里
        MyToolButton.push_back(head_prt); //vector保存按钮
    }
}

List::~List()
{
    delete ui;
}
