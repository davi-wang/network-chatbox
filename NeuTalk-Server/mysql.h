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
    bool login(QString email, QString password, int &);//用户登录
    QJsonObject queryUserInfo(QString email);//通过邮箱或用户名查询用户
    QJsonObject queryUser(int uid);//通过uid查询用户
    QJsonObject queryFriendlist(int user_uid);//查询某用户好友列表
    void createFriendshiptable();//创建好友关系表
    bool addFriends(int user_uid, int friend_uid);//添加好友
    void deleteFriend(int user_uid, int friend_uid);//删除好友
    bool IsBfriendtoA(int a_uid, int b_uid);//查询B是否为A的好友
    void createSinglehistorytable(int a_uid, int b_uid);//创建私聊消息记录表
    void insertSinglehistory(int sender_uid, int receiver_uid, QString words, QString time);//向特定记录表中插入消息记录
    QJsonObject queryHistorylist(int a_uid, int b_uid);//查询消息记录
    void createPublicchatroom();//创建公共聊天室
    void insertPCRhistory(int sender_uid, QString datetime, QString words);//记录公共聊天室内消息

signals:
    void dispalyUserstext(const QString &);

private:
    explicit MySql(QObject *parent = nullptr);
    static MySql* sqlhand;
    QSqlDatabase database;
    QSqlQuery* sqlquery;

};

#endif // MYSQL_H
