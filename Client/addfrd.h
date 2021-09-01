#ifndef ADDFRD_H
#define ADDFRD_H

#include <QWidget>
#include "Repeater.h"
#include "QJsonObject"
#include "QJsonValue"
#include <QToolButton>
namespace Ui { class AddFrd; }




class AddFrd : public QWidget
{
    Q_OBJECT

public:
    explicit AddFrd(QWidget *parent = nullptr);
    ~AddFrd();

private slots:
    void on_Find_clicked(); //查找按钮
    void on_Add_clicked(); //添加

    void getF(); //获取好友列表
signals:
    void refresh();
    void request_user_info();

private:
    Ui::AddFrd *ui;
    ClientServer * Repeater = ClientServer::GetInstance();
    QVector<bool> IsShow;
    QVector<QToolButton*>vector;
};

#endif // ADDFRD_H
