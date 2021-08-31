#include "signon.h"
#include "ui_signon.h"
#include "verification.h"

Signon::Signon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signon)
{
    ui->setupUi(this);
    setWindowTitle("MyChat"); //设置标题
    setWindowIcon(QPixmap(":/image/TuBiao.png")); //设置图标

    //连接信号槽函数
    connect(Repeater, SIGNAL(verification_sending()), this, SLOT(ShowSending())); //请求已收到，正在发送
    connect(Repeater, SIGNAL(verification_sent()), this, SLOT(SendSuccess())); //发送验证邮件成功
}

Signon::~Signon()
{
    delete ui;
}

void Signon::ShowSending(){ //请求已收到，正在发送
    QMessageBox::information(this, "Information", "Request received, sending ...");
}

void Signon::SendSuccess(){ //发送验证邮件成功
    QMessageBox::information(this, "Information", "Send verification email successfully !");
}

void Signon::on_pushButton_2_clicked() //finish注册按钮 /若正确跳转登录
{
    QString fir = ui->pswd_1->text(); //获取第一次密码
    QString sec = ui->pswd_2->text(); //获取第二次密码

    //判断两次密码是否相同
    if(fir == sec){
        QString usersname = ui->set_usrname->text(); //获取用户名 /邮箱
        QString nick = ui->nickname->text(); //获取昵称

        //检测昵称是否不为空
        if(nick != NULL){

            //检测密码是否大于6位
            if(fir.count() > 5){

                //第一阶段成功
                QJsonObject json; //json
                json.insert("user_email", QJsonValue(usersname)); //打包json
                //connect
                Repeater->SendMsg(Connection::R1_request_email, json); //请求发送邮件

                //临时存储信息
                QSettings settings("Temporary storage.ini", QSettings::IniFormat); //保存在文件中
                QString temporary = QString("info");
                settings.beginGroup(temporary); //打开组
                //存到组
                settings.setValue("username", usersname); //用户名 即email
                settings.setValue("passwrd", fir); //密码
                settings.setValue("nickname", nick); //昵称
                settings.endGroup(); //关闭组

                //弹出验证码界面
                Verification *ver = new Verification(nullptr); //创建新验证码窗口对象
                this->close(); //注册页面关掉
                ver->show(); //验证码页面打开
            }
            else{ //密码少于6位
                QMessageBox::information(this, "tip", "The password less than 6 digits ！");
            }
        }
        else{//昵称为空
            QMessageBox::information(this, "tip", "Nickname cannot be empty ！");
        }
    }
    else{ //告知密码不同错误
        QMessageBox::information(this, "tip", "The two passwords are different !");
    }
}
