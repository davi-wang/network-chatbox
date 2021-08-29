#include "mysql.h"
MySql* MySql::sqlhand = nullptr;
MySql::MySql(QObject *parent) : QObject(parent)
{

}

bool MySql::openDatabase(){
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else{
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("mydatabase.db");
        database.setUserName("smurf");
        database.setPassword("125125");
    }
    if(database.open()){
        qDebug()<<"database open OK !";
        emit dispalyUserstext("database open OK !");
        createUserstable();
        createFriendshiptable();
        return true;
    }
    else{
        qDebug()<<"database open failed !";
        emit dispalyUserstext("database open failed !");
        return false;
    }
}

void MySql::createUserstable(){
    QStringList tablelist = database.tables();
    sqlquery = new QSqlQuery(database);
    if(!tablelist.contains("userstable")){
        QString createusers = "CREATE TABLE userstable (uid int primary key not null, email text, nickname text, password text)";
        if(sqlquery->prepare(createusers)){
            if(sqlquery->exec()){
                qDebug()<<"create userstable OK !";
                emit dispalyUserstext("create userstable OK !");
            }
        }
        else{
            qDebug()<<"create table command error !";
            emit dispalyUserstext("create table command error !");
        }
    }
    else{
        qDebug()<<"userstable exists !";
        emit dispalyUserstext("userstable exists !");
    }
}

void MySql::createFriendshiptable(){
    QStringList tablelist = database.tables();
    sqlquery = new QSqlQuery(database);
    if(!tablelist.contains("friendshiptable")){
        QString createusers = "CREATE TABLE friendshiptable (id INTEGER primary key autoincrement, user_uid int, friend_uid int)";
        if(sqlquery->prepare(createusers)){
            if(sqlquery->exec()){
                qDebug()<<"create friendshiptable OK !";
                emit dispalyUserstext("create friendshiptable OK !");
            }
        }
        else{
            qDebug()<<"create table command error !";
            emit dispalyUserstext("create table command error !");
        }
    }
    else{
        qDebug()<<"friendshiptable exists !";
        emit dispalyUserstext("friendshiptable exists !");
    }
}

int MySql::registerUser(QString email, QString nickname, QString password){
    srand((unsigned)time(NULL));
    int uid;
    uid = rand() % 9000 + 11000;
    while(queryInfo("userstable","uid",QString::number(uid))){
        uid = rand() % 9000 + 11000;
    }
    QString insert = "INSERT INTO userstable(uid,email,nickname,password) VALUES("+QString::number(uid)+",'"+email+"','"+nickname+"','"+password+"')";
    if(sqlquery->prepare(insert)){
        if(sqlquery->exec()){
            qDebug()<<"registered successfully !";
            emit dispalyUserstext("registered successfully !");
        }
        else{
            qDebug()<<"primary key cannot same!";
            emit dispalyUserstext("primary key cannot same!");
        }
    }
    else{
        qDebug()<<"registered fail !";
        emit dispalyUserstext("registered fail !");
    }
    return uid;
}

bool MySql::login(QString email, QString password){
    QString str_select = "SELECT * FROM userstable WHERE email = '"+email+"'";
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            int flag = 0;
            while(sqlquery->next()){
                if(password == sqlquery->value(3).toString()){
                    qDebug()<<"denglu successfully !";
                    emit dispalyUserstext("denglu successfully !");
                    //登陆成功，跳转页面
                    flag = 1;
                    return true;
                }
                else{
                    qDebug()<<"wrong password !";
                    emit dispalyUserstext("wrong password !");
                    flag = -1;
                }
            }
            if(flag == 0)
                qDebug()<<"email does not exist !";
                emit dispalyUserstext("email does not exist !");
        }
    }
    else{
        qDebug()<<"denglu failed !";
        emit dispalyUserstext("denglu failed !");
    }
    return false;
}

bool MySql::queryInfo(QString tableName, QString column, QString info){
    int flag = 0;
    QString str_select = "SELECT * FROM "+tableName+" WHERE "+column+" = "+info;
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            while(sqlquery->next()){
                qDebug()<<"selected successfully !";
                emit dispalyUserstext("selected successfully !");
                flag = 1;
            }
        }
    }
    else{
        qDebug()<<"select command error !";
        emit dispalyUserstext("select command error !");
    }
    return flag;
}

bool MySql::queryUserInfo(QString email){
    int flag = 0;
    QString str_select = "SELECT * FROM userstable WHERE email = "+email;
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            while(sqlquery->next()){
                qDebug()<<"uid:"<<sqlquery->value(0).toString()
                       <<"email:"<<sqlquery->value(1).toString()
                      <<"nickname:"<<sqlquery->value(2).toString();
                emit dispalyUserstext("uid:"+sqlquery->value(0).toString()
                                      +"email:"+sqlquery->value(1).toString()
                                      +"nickname"+sqlquery->value(2).toString());
                flag = 1;
            }
        }
    }
    if(flag){
        qDebug()<<"selected successfully !";
        emit dispalyUserstext("selected successfully !");
        return true;
    }
    else{
        qDebug()<<"no one was found !";
        emit dispalyUserstext("no one was found !");
        return false;
    }
}

QJsonObject MySql::queryFriendlist(int user_uid){
    QString str_select = "SELECT * FROM userstable WHERE uid IN (SELECT friend_uid FROM friendshiptable WHERE user_uid = "+QString::number(user_uid)+")";
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            QJsonArray friend_list;
            while(sqlquery->next()){
                QJsonObject friend_info;
                friend_info.insert("uid", QJsonValue(sqlquery->value(0).toString()));
                friend_info.insert("email", QJsonValue(sqlquery->value(1).toString()));
                friend_info.insert("nickname", QJsonValue(sqlquery->value(2).toString()));
                friend_list.append(QJsonValue(friend_info));
            }
            QJsonObject json;
            json.insert("friend list", QJsonValue(friend_list));
            return json;
        }
        else{
            qDebug()<<"wrong here !";
        }
    }
    else{
        qDebug()<<"wrong user's uid !";
        emit dispalyUserstext("wrong user's uid !");
    }
    return QJsonObject();
}

bool MySql::IsBfriendtoA(int a_uid, int b_uid){
    QString str_select = "SELECT * FROM friendshiptable WHERE user_uid = "+QString::number(a_uid)+" AND friend_uid = "+QString::number(b_uid);
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            while(sqlquery->next()){
                qDebug()<<"B is a friend of A !";
                emit dispalyUserstext("B is a friend of A !");
                return true;
            }
        }
    }
    qDebug()<<"B is not a friend of A !";
    emit dispalyUserstext("B is not a friend of A !");
    return false;
}

void MySql::dropTable(QString tableName){
    QString str_drop = "DROP TABLE "+tableName;
    if(sqlquery->prepare(str_drop)){
        if(sqlquery->exec()){
            qDebug()<<"drop table OK !";
            emit dispalyUserstext("drop table OK !");
        }
    }
    else{
        qDebug()<<"drop command error !";
        emit dispalyUserstext("drop command error !");
    }
}

void MySql::deleteData(QString tableName, QString column, QString info){
    QString str_delete = "DELETE FROM "+tableName+" WHERE "+column+" = "+info;
    if(sqlquery->prepare(str_delete)){
        if(sqlquery->exec()){
            qDebug()<<"delete info OK !";
            emit dispalyUserstext("delete info OK !");
        }
    }
    else{
        qDebug()<<"delete command error !";
        emit dispalyUserstext("delete command error !");
    }
}

void MySql::addFriends(int user_uid, int friend_uid){
    if(!IsBfriendtoA(user_uid, friend_uid)){
        QString insert = "INSERT INTO friendshiptable(user_uid,friend_uid) VALUES("+QString::number(user_uid)+","+QString::number(friend_uid)+")";
        if(sqlquery->prepare(insert)){
            if(sqlquery->exec()){
                qDebug()<<"addfriend successfully !";
                emit dispalyUserstext("addfriend successfully !");
            }
        }
        else{
            qDebug()<<"add fail !";
            emit dispalyUserstext("add fail !");
        }
    }
    else{
        qDebug()<<"B is already a friend of A !";
        emit dispalyUserstext("B is already a friend of A !");
    }
}

void MySql::deleteFriend(int user_uid, int friend_uid){
    QString str_delete = "DELETE FROM friendshiptable WHERE user_uid = "+QString::number(user_uid)+" AND friend_uid = "+QString::number(friend_uid);
    if(sqlquery->prepare(str_delete)){
        if(sqlquery->exec()){
            qDebug()<<"delete friend OK !";
            emit dispalyUserstext("delete friend OK !");
        }
    }
    else{
        qDebug()<<"delete command error !";
        emit dispalyUserstext("delete command error !");
    }
}
