#include "verification.h"
#include "ui_verification.h"
#include "signin.h"
#include "signon.h"

Verification::Verification(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Verification)
{
    ui->setupUi(this);

    //连接信号槽函数
    connect(Repeater, SIGNAL(Reg_Success()), this, SLOT(SignOnSuccess())); //注册成功，服务器返回账户uid
    connect(Repeater, SIGNAL(Reg_fail()), this, SLOT(SignOnFail())); //注册失败，返回失败原因
}

Verification::~Verification()
{
    delete ui;
}

void Verification::SignOnFail(){ //注册失败，返回失败原因
    ClientServer * data = ClientServer::GetInstance();
    int ableif = data->ErrorReason; //调
    if(ableif == 0){ //注册成功
        //无问题，注册成功
        QMessageBox::information(this, "tip", "Sign on success !");
        SignIn *sig = new SignIn(nullptr); //创建新登录窗口对象
        this->close(); //注册页面关掉
        sig->show(); //登录页面打开
    }
    //注册失败
    else if(ableif == 1){ //无效邮箱
        QMessageBox::information(this, "tip", "Invalid mailbox ！");
        //退回注册页面
        Signon *sig = new Signon(nullptr); //创建新注册窗口对象
        this->close(); //验证页面关闭
        sig->show(); //注册页面打开
    }
    else if(ableif == 2 || ableif == 3){ //超时 或 验证码错误
        QMessageBox::information(this, "tip", "Verification code error or failure, you can reply to resend.");
        //重新发送 /退回上一个页面
        Signon* re = new Signon(nullptr); //创建新注册窗口对象
        this->close(); //验证码页面关掉
        re->show(); //注册页面打开
    }
}

void Verification::SignOnSuccess(){ //注册成功，服务器返回账户uid
    //无问题，注册成功
    QMessageBox::information(this, "tip", "Sign on success !");
    SignIn * sig = new SignIn(nullptr); //创建新登录窗口对象
    this->close(); //注册页面关掉
    sig->show(); //登录页面打开
}

void Verification::on_OKBt_clicked() //OK按钮
{
    //传回信息
    QString Vcode = ui->VCode->text(); //获取用户填写的验证码
    if(Vcode != NULL){ //验证码不为空

        //获取用户名、密码、昵称
        QSettings settings("Temporary storage.ini", QSettings::IniFormat); //打开临时存储文件

        QString usersname = settings.value("info/username").toString(); //用户名 即email
        QString pswd = settings.value("info/passwrd").toString();//密码
        QString nick = settings.value("info/nickname").toString();//昵称


        //打包json
        QJsonObject json;

        QCryptographicHash hash(QCryptographicHash::Md5); // md5散列
        QByteArray byte_array;
        byte_array.append(pswd);
        hash.addData(byte_array);
        QByteArray result_byte_array = hash.result(); //返回最终的哈希值
        QString md5PassWord = result_byte_array.toHex();




        json.insert("user_email", QJsonValue(usersname)); // 邮箱字符串，保证纯ASCII
        json.insert("username", QJsonValue(nick));  // 用户昵称
        json.insert("password", QJsonValue(md5PassWord));  // 密码
        json.insert("verification_code", QJsonValue(Vcode)); //验证码
        //connect
        Repeater->SendMsg(Connection::R4_request_register, json); //请求注册
    }
    else{ //验证码为空
        QMessageBox::information(this, "tip", "Verification code cannot be empty ！");
    }
}
