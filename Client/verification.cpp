#include "verification.h"
#include "ui_verification.h"
#include "signin.h"

Verification::Verification(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Verification)
{
    ui->setupUi(this);
}

Verification::~Verification()
{
    delete ui;
}

void Verification::on_OKBt_clicked() //OK按钮
{
    //传回验证码
    QString Vcode = ui->VCode->text();

    //无问题，注册成功
    QMessageBox::information(this, "tip", "Sign on success !");
    SignIn *sig = new SignIn(nullptr); //创建新登录窗口对象
    this->close(); //注册页面关掉
    sig->show(); //登录页面打开
}
