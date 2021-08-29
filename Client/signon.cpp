#include "signon.h"
#include "ui_signon.h"
#include "signin.h"
#include "QSqlQuery"
#include "verification.h"

Signon::Signon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signon)
{
    ui->setupUi(this);
    setWindowTitle("MyChat"); //设置标题
    setWindowIcon(QPixmap(":/image/TuBiao.png")); //设置图标
}

Signon::~Signon()
{
    delete ui;
}

void Signon::on_pushButton_2_clicked() //finish注册按钮 /若正确跳转登录
{
    QString fir = ui->pswd_1->text(); //获取第一次密码
    QString sec = ui->pswd_2->text(); //获取第二次密码
    QString uid = "1"; //从server端获取uid

    //判断两次密码是否相同
    if(fir == sec){
        QString usersname = ui->set_usrname->text(); //获取用户名
        QString cmd = QString("insert into userinfo values('%1', '%2', '%3')") //插入用户名密码uid
                .arg(usersname) .arg(fir) .arg(uid);
        QSqlQuery query;

        //检测昵称是否不为空
        if(ui->nickname->text() != NULL){

            //检测密码是否大于6位
            if(fir.count() > 5){

                //检测用户名是否已经被注册
                if(query.exec(cmd)){

                    //第一阶段成功，弹出验证码界面
                    Verification* ver = new Verification(nullptr); //创建新验证码窗口对象
                    this->close(); //注册页面关掉
                    ver->show(); //验证码页面打开
                }
                else{ //插入失败，用户名重复
                    QMessageBox::information(this, "tip", "This user name is already registered ！");
                }
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
