#ifndef LOGON_H
#define LOGON_H

#include <QWidget>

namespace Ui { class SignIn; }

class SignIn : public QWidget
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent = nullptr);
    ~SignIn();

private slots:
    void on_SigninBt_clicked(); //登录
    void on_SignonBt_clicked(); //注册

    void on_userEdit_currentTextChanged(QString arg1); //通过userEdit索引自动填充密码

private:
    Ui::SignIn *ui;
};

#endif // LOGON_H
