#ifndef VERIFICATION_H
#define VERIFICATION_H

#include <QWidget>
#include "Repeater.h"
#include "QMessageBox"
#include "QJsonObject"
#include "QJsonValue"

namespace Ui { class Verification; }

class Verification : public QWidget
{
    Q_OBJECT

public:
    explicit Verification(QWidget *parent = nullptr);
    ~Verification();

private slots:
    void on_OKBt_clicked(); //点击OK

    //与Repeater响应
    //自己的槽函数 /接收信号
    void SignOnFail(); //注册失败，返回失败原因
    void SignOnSuccess(); //注册成功，服务器返回账户uid

private:
    Ui::Verification *ui;
    ClientServer * Repeater = ClientServer::GetInstance();
};

#endif // VERIFICATION_H
