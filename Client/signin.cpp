#include "signin.h"
#include "login.h"
#include "signon.h"

SignIn::SignIn(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::SignIn)
{
    ui->setupUi(this);
    setWindowTitle("MyChat"); //设置标题
    setWindowIcon(QPixmap(":/image/TuBiao.png")); //设置图标

    //初始化
    QSettings settings("Savestate.ini", QSettings::IniFormat); //打开文件

    QStringList ed = settings.childGroups(); //获取所有的组

    for (auto i : ed){ //循环读
        settings.beginGroup(i); //打开组 info i
        QString usersname = settings.value("username").toString(); //读取数据
        QString password = settings.value("passwrd").toString();
        settings.endGroup(); //关闭组 info i

        if (!usersname.isEmpty())
        {
            ui->userEdit->addItem(usersname, password); //填充用户名密码
        }
    }
    ui->checkBox->setChecked(true); //初始化为默认勾选

    if (ed.count() == 0) { //如果从未记住过密码
        ui->userEdit->setCurrentText("Email"); //初始化文字为"Email"
    }
    else
    {
        ui->userEdit->setCurrentIndex(0); //初始化索引为 0
    }

    //连接信号槽函数
    connect(Repeater, SIGNAL(Logging()), this, SLOT(ShowLoggingProcess()));  //服务器已收到登录请求
    connect(Repeater, SIGNAL(SignInSuccess()), this, SLOT(SignInSuccess())); //登录成功，同步数据
    connect(Repeater, SIGNAL(SignInFail()), this, SLOT(SignInFail())); //登录失败

    connect(Repeater, SIGNAL(synchro_data()), this, SLOT(synchro_data())); //同步数据（用户个人profile+好友列表+群组列表）
    connect(Repeater, SIGNAL(synchronization_complete()), this, SLOT(synchronization_complete())); //同步数据完成
}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::on_SigninBt_clicked() //登录
{

    usersname = ui->userEdit->currentText(); //获取用户名
    password = ui->pswdEdit->text(); //获取密码

    QJsonObject json; //打包json
    json.insert("user_email", QJsonValue(usersname));
        QCryptographicHash hash(QCryptographicHash::Md5); // md5散列
        QByteArray byte_array;
        byte_array.append(password);
        hash.addData(byte_array);
        QByteArray result_byte_array = hash.result(); //返回最终的哈希值
        QString md5PassWord = result_byte_array.toHex(); // 转为QString类型填入JSON
    json.insert("password", QJsonValue(md5PassWord));

    Repeater->SendMsg(Connection::L1_request_login, json); //发送msg

}

void SignIn::on_SignonBt_clicked() //注册按钮
{
    Signon *sig = new Signon(nullptr,this); //创建新注册窗口对象
    this->close(); //登录页面关掉
    sig->show(); //注册页面打开
}

void SignIn::on_userEdit_currentTextChanged(QString arg1) //通过userEdit索引自动填充密码
{
    int indx = ui->userEdit->findText(arg1); //获取userEdit索引
    QString strpswd = ui->userEdit->itemData(indx).toString(); //找到对应的密码
    ui->pswdEdit->setText(strpswd); //填充
}

void SignIn::ShowLoggingProcess() //服务器已收到登录请求
{
    QMessageBox::information(this, "Information", "Logging. Please wait a moment.", QMessageBox::Ok,
                             QMessageBox::Ok);
}

void SignIn::SignInSuccess() //登录成功，同步数据
{

    if (ui->checkBox->isChecked())
    {
        //此时为记住密码的勾选状态

        //查看是否已经保存到本地
        int ix = ui->userEdit->findText(usersname); //查看是否存在
        if (ix != -1)
            goto t; //存在，不用存储，直接跳过

        //分组记录
        int cnt = ui->userEdit->count() + 1; //计数
        QSettings settings("Savestate.ini", QSettings::IniFormat); //保存在文件中

        QString strInfoName = QString("info%1").arg(cnt); //组
        settings.beginGroup(strInfoName); //打开组
        settings.setValue("username", usersname); //存到当前组
        settings.setValue("passwrd", password);
        settings.endGroup(); //关闭组
    }
t:
    usersname.clear(); //自动清空
    password.clear();
    QMessageBox::information(this, "Information", "Success.");
}

void SignIn::synchro_data() //同步数据（用户个人profile+好友列表+群组列表）
{
    QMessageBox::information(this, "Information", "Sychronizing Data.");
}

void SignIn::SignInFail() //登录失败
{
    QMessageBox::information(this, "Information", "The mailbox does not exist or the password is incorrect.");
}

void SignIn::synchronization_complete() //同步数据完成
{
    login * lis = new login(nullptr); //创建新好友列表窗口对象
    this->close(); //登录页面关掉
    lis->show(); //好友列表页面打开
}
