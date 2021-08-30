#include "signin.h"
#include "ui_signin.h"
#include "QMessageBox"
#include "QSqlQuery"
#include "QSqlTableModel"
#include "signon.h"
#include "login.h"
#include "connection.h"
#include "QSettings"
#include "qdebug.h"

SignIn::SignIn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
    setWindowTitle("MyChat"); //设置标题
    setWindowIcon(QPixmap(":/image/TuBiao.png")); //设置图标

    //初始化
    QSettings settings("Savestate.ini", QSettings::IniFormat); //打开文件

    QStringList ed = settings.childGroups(); //获取所有的组

    for(auto i :ed){ //循环读
        settings.beginGroup(i); //打开组 info i
        QString usersname = settings.value("username").toString(); //读取数据
        QString password = settings.value("passwrd").toString();
        settings.endGroup(); //关闭组 info i

        if(!usersname.isEmpty()){
            ui->userEdit->addItem(usersname, password); //填充用户名密码
        }
    }
    ui->checkBox->setChecked(true); //初始化为默认勾选

    if(ed.count()==0){ //如果从未记住过密码
        ui->userEdit->setCurrentText("Email"); //初始化文字为"Email"
    }
    else{
        ui->userEdit->setCurrentIndex(0); //初始化索引为 0
    }
}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::on_SigninBt_clicked() //登录
{
    QString usersname = ui->userEdit->currentText(); //获取用户名
    QString password = ui->pswdEdit->text(); //获取密码

    //记住密码与否都要走的正常登录流程
    //查询数据库，验证密码是否正确
    QSqlTableModel *psdata = new QSqlTableModel;
    psdata->setTable("userinfo");
    psdata->setFilter(QString("usersname = '%1' and password = '%2'")
                     .arg(usersname).arg(password)); //过滤
    psdata->select();

    // 检测是否查询到数据
    int row = psdata->rowCount();
    if(row > 0){ //查询结果存在，登陆成功
        QMessageBox::information(this, "tip", "sign in success !"); //登陆成功

        //此时为记住密码的勾选状态
        if(ui->checkBox->isChecked()){

            //查看是否已经保存到本地
            int ix = ui->userEdit->findText(usersname); //查看是否存在
            if(ix != -1) goto t; //存在，不用存储，直接跳过

            //分组记录
            int cnt = ui->userEdit->count() + 1; //计数
            QSettings settings("Savestate.ini", QSettings::IniFormat); //保存在文件中

            QString strInfoName = QString("info%1").arg(cnt); //组
            settings.beginGroup(strInfoName); //打开组
            settings.setValue("username", usersname); //存到当前组
            settings.setValue("passwrd", password);
            settings.endGroup(); //关闭组

            qDebug()<<"Already remember the password !";
        }
t:
        login *lis = new login(nullptr); //创建新好友列表窗口对象
        this->close(); //登录页面关掉
        lis->show(); //好友列表页面打开
    }
    else{ //失败
        QMessageBox::information(this, "tip", "Username or password wrong !"); //中文的话，QString::fromLocal8Bit("")
    }

    delete psdata; //释放
}

void SignIn::on_SignonBt_clicked() //注册按钮
{
    Signon *sig = new Signon(nullptr); //创建新注册窗口对象
    this->close(); //登录页面关掉
    sig->show(); //注册页面打开
}


void SignIn::on_userEdit_currentTextChanged(QString arg1) //通过userEdit索引自动填充密码
{
    int indx = ui->userEdit->findText(arg1); //获取userEdit索引
    QString strpswd = ui->userEdit->itemData(indx).toString(); //找到对应的密码
    ui->pswdEdit->setText(strpswd); //填充
}


void SignIn::on_toolButton_clicked() //找回密码
{
    //发送回server端，得到密码

}
