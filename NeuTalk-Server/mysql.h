#ifndef MYSQL_H
#define MYSQL_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

struct client
{
    int num;
    QString email;
    QString password;
};

class MySql : public QObject
{
    Q_OBJECT
public:
    static MySql* gethand(){
        if(sqlhand == nullptr)
            sqlhand = new MySql();
        return sqlhand;
    }

    bool openDatabase();  //打开数据库
    void createUserstable();  //创建用户信息表
    void dropTable(QString tableName);  //删除表
    void registerUser(client& a);  //用户注册
    bool login(QString email, QString password);  //用户登录
    void queryUserinfo(QString info);  //查询信息
    void deleteData(QString info);  //删除信息

signals:
    void dispalyUserstext(const QString &);

private:
    explicit MySql(QObject *parent = nullptr);
    static MySql* sqlhand;
    QSqlDatabase database;
    QSqlQuery* sqlquery;

signals:

};

#endif // MYSQL_H
