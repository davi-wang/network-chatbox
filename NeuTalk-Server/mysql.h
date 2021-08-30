#ifndef MYSQL_H
#define MYSQL_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include <QDebug>


class MySql : public QObject
{
    Q_OBJECT
public:
    static MySql* gethand(){
        if(sqlhand == nullptr)
            sqlhand = new MySql();
        return sqlhand;
    }

    bool openDatabase();//打开数据库
    bool queryInfo(QString tableName, QString column, QString info);//查询信息
    void deleteData(QString tableName, QString column, QString info);//删除信息
    void dropTable(QString tableName);//删除表
    void createUserstable();//创建用户信息表
    int registerUser(QString email, QString nickname, QString password);//用户注册
    bool login(QString email, QString password);//用户登录
    bool queryUserInfo(QString email);//查询用户_通过email
    QJsonObject queryFriendlist(int user_uid);//查询某用户好友列表
    void createFriendshiptable();//创建好友关系表
    void addFriends(int user_uid, int friend_uid);//添加好友
    void deleteFriend(int user_uid, int friend_uid);//删除好友
    bool IsBfriendtoA(int a_uid, int b_uid);//查询B是否为A的好友

signals:
    void dispalyUserstext(const QString &);

private:
    explicit MySql(QObject *parent = nullptr);
    static MySql* sqlhand;
    QSqlDatabase database;
    QSqlQuery* sqlquery;

};

#endif // MYSQL_H
