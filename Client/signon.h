#ifndef SIGNON_H
#define SIGNON_H

#include <QWidget>
#include "Repeater.h"
#include "QMessageBox"
#include "QSettings"
#include "QJsonObject"
#include "QJsonValue"

namespace Ui { class Signon; }

class Signon : public QWidget
{
    Q_OBJECT

public:
    explicit Signon(QWidget *parent = nullptr);
    ~Signon();

private slots:
    void on_pushButton_2_clicked(); //Finish按钮

    //与Repeater响应
    //自己的槽函数 /接收信号
    void ShowSending(); //请求已收到，正在发送
    void SendSuccess(); //发送验证邮件成功

private:
    Ui::Signon *ui;
    ClientServer * Repeater = ClientServer::GetInstance();
};

#endif // SIGNON_H
