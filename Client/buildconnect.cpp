#include "buildconnect.h"
#include "ui_buildconnect.h"
#include "signin.h"

BuildConnect::BuildConnect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuildConnect)
{
    ui->setupUi(this);
}

BuildConnect::~BuildConnect()
{
    delete ui;
}

void BuildConnect::on_Build_clicked()
{
    Ip =ui->IPEdit->text(); //获取ip
    Port = ui->PORTEdit->text(); //获取port

    //和Repeater建立连接
    Repeater->CreateInstance(Ip, Port);
    //告知已建立连接
    QMessageBox::information(this, "Information", "Contact has been established !");

    //废掉按钮
    ui->Build->setEnabled(false);

    SignIn * sign = new SignIn(nullptr); //新建登录页面
    this->close(); //关闭建联系页面
    sign->show(); //打开登陆页面
}
