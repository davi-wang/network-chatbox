#include "mysql.h"
#include "qdatetime.h"

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
        qDebug() << "[DB] " << "database open OK !";
        emit dispalyUserstext("[DB]  database open OK !");
        createUserstable();
        createFriendshiptable();
        return true;
    }
    else{
        qDebug() << "[DB] " << "database open failed !";
        emit dispalyUserstext("[DB]  database open failed !");
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
                qDebug() << "[DB] " << "create userstable OK !";
                emit dispalyUserstext("[DB]  create userstable OK !");
            }
        }
        else{
            qDebug() << "[DB] " << "create table command error !";
            emit dispalyUserstext("[DB]  create table command error !");
        }
    }
    else{
        qDebug() << "[DB] " << "userstable exists !";
        emit dispalyUserstext("[DB]  userstable exists !");
    }
}

void MySql::createFriendshiptable(){
    QStringList tablelist = database.tables();
    sqlquery = new QSqlQuery(database);
    if(!tablelist.contains("friendshiptable")){
        QString createusers = "CREATE TABLE friendshiptable (id INTEGER primary key autoincrement, user_uid int, friend_uid int)";
        if(sqlquery->prepare(createusers)){
            if(sqlquery->exec()){
                qDebug() << "[DB] " << "create friendshiptable OK !";
                emit dispalyUserstext("[DB]  create friendshiptable OK !");
            }
        }
        else{
            qDebug() << "[DB] " << "create table command error !";
            emit dispalyUserstext("[DB]  create table command error !");
        }
    }
    else{
        qDebug() << "[DB] " << "friendshiptable exists !";
        emit dispalyUserstext("[DB]  friendshiptable exists !");
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
            qDebug() << "[DB] " << "registered successfully !";
            emit dispalyUserstext("[DB]  registered successfully !");
        }
        else{
            qDebug() << "[DB] " << "primary key cannot same!";
            emit dispalyUserstext("[DB]  primary key cannot same!");
        }
    }
    else{
        qDebug() << "[DB] " << "registered fail !";
        emit dispalyUserstext("[DB]  registered fail !");
    }
    return uid;
}

bool MySql::login(QString email, QString password, int &uid){
    QString str_select = "SELECT * FROM userstable WHERE email = '"+email+"'";
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            int flag = 0;
            while(sqlquery->next()){
                if(password == sqlquery->value(3).toString()){
                    qDebug() << "[DB] " << "denglu successfully !";
                    emit dispalyUserstext("[DB]  denglu successfully !");
                    //登陆成功，跳转页面
                    uid = sqlquery->value(0).toInt();
                    flag = 1;
                    return true;
                }
                else{
                    qDebug() << "[DB] " << "wrong password !";
                    emit dispalyUserstext("[DB]  wrong password !");
                    flag = -1;
                }
            }
            if(flag == 0)
                qDebug() << "[DB] " << "email does not exist !";
                emit dispalyUserstext("[DB]  email does not exist !");
        }
    }
    else{
        qDebug() << "[DB] " << "denglu failed !";
        emit dispalyUserstext("[DB]  denglu failed !");
    }
    return false;
}

bool MySql::queryInfo(QString tableName, QString column, QString info){
    int flag = 0;
    QString str_select = "SELECT * FROM "+tableName+" WHERE "+column+" = "+info;
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            while(sqlquery->next()){
                qDebug() << "[DB] " << "selected successfully !";
                emit dispalyUserstext("[DB]  selected successfully !");
                flag = 1;
            }
        }
    }
    else{
        qDebug() << "[DB] " << "select command error !";
        emit dispalyUserstext("[DB]  select command error !");
    }
    return flag;
}

bool MySql::queryUserInfo(QString email){
    int flag = 0;
    QString str_select = "SELECT * FROM userstable WHERE email = "+email;
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            while(sqlquery->next()){
                qDebug() << "[DB] " << "uid:"<<sqlquery->value(0).toString()
                       <<"email:"<<sqlquery->value(1).toString()
                      <<"nickname:"<<sqlquery->value(2).toString();
                emit dispalyUserstext("[DB]  uid:"+sqlquery->value(0).toString()
                                      +"email:"+sqlquery->value(1).toString()
                                      +"nickname"+sqlquery->value(2).toString());
                flag = 1;
            }
        }
    }
    if(flag){
        qDebug() << "[DB] " << "selected successfully !";
        emit dispalyUserstext("[DB]  selected successfully !");
        return true;
    }
    else{
        qDebug() << "[DB] " << "no one was found !";
        emit dispalyUserstext("[DB]  no one was found !");
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
                friend_info.insert("uid", QJsonValue(sqlquery->value(0).toInt()));
                friend_info.insert("email", QJsonValue(sqlquery->value(1).toString()));
                friend_info.insert("nickname", QJsonValue(sqlquery->value(2).toString()));
                friend_list.append(QJsonValue(friend_info));
            }
            QJsonObject json;
            json.insert("friend_list", QJsonValue(friend_list));
            return json;
        }
        else{
            qDebug() << "[DB] " << "wrong here !";
        }
    }
    else{
        qDebug() << "[DB] " << "wrong user's uid !";
        emit dispalyUserstext("[DB]  wrong user's uid !");
    }
    return QJsonObject();
}

bool MySql::IsBfriendtoA(int a_uid, int b_uid){
    QString str_select = "SELECT * FROM friendshiptable WHERE user_uid = "+QString::number(a_uid)+" AND friend_uid = "+QString::number(b_uid);
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            while(sqlquery->next()){
                qDebug() << "[DB] " << "B is a friend of A !";
                emit dispalyUserstext("[DB]  B is a friend of A !");
                return true;
            }
        }
    }
    qDebug() << "[DB] " << "B is not a friend of A !";
    emit dispalyUserstext("[DB]  B is not a friend of A !");
    return false;
}

void MySql::dropTable(QString tableName){
    QString str_drop = "DROP TABLE "+tableName;
    if(sqlquery->prepare(str_drop)){
        if(sqlquery->exec()){
            qDebug() << "[DB] " << "drop table OK !";
            emit dispalyUserstext("[DB]  drop table OK !");
        }
    }
    else{
        qDebug() << "[DB] " << "drop command error !";
        emit dispalyUserstext("[DB]  drop command error !");
    }
}

void MySql::deleteData(QString tableName, QString column, QString info){
    QString str_delete = "DELETE FROM "+tableName+" WHERE "+column+" = "+info;
    if(sqlquery->prepare(str_delete)){
        if(sqlquery->exec()){
            qDebug() << "[DB] " << "delete info OK !";
            emit dispalyUserstext("[DB]  delete info OK !");
        }
    }
    else{
        qDebug() << "[DB] " << "delete command error !";
        emit dispalyUserstext("[DB]  delete command error !");
    }
}

void MySql::addFriends(int user_uid, int friend_uid){
    if(!IsBfriendtoA(user_uid, friend_uid)){
        QString insert = "INSERT INTO friendshiptable(user_uid,friend_uid) VALUES("+QString::number(user_uid)+","+QString::number(friend_uid)+")";
        if(sqlquery->prepare(insert)){
            if(sqlquery->exec()){
                qDebug() << "[DB] " << "addfriend successfully !";
                emit dispalyUserstext("[DB]  addfriend successfully !");
            }
        }
        else{
            qDebug() << "[DB] " << "add fail !";
            emit dispalyUserstext("[DB]  add fail !");
        }
    }
    else{
        qDebug() << "[DB] " << "B is already a friend of A !";
        emit dispalyUserstext("[DB]  B is already a friend of A !");
    }
}

void MySql::deleteFriend(int user_uid, int friend_uid){
    QString str_delete = "DELETE FROM friendshiptable WHERE user_uid = "+QString::number(user_uid)+" AND friend_uid = "+QString::number(friend_uid);
    if(sqlquery->prepare(str_delete)){
        if(sqlquery->exec()){
            qDebug() << "[DB] " << "delete friend OK !";
            emit dispalyUserstext("[DB]  delete friend OK !");
        }
    }
    else{
        qDebug() << "[DB] " << "delete command error !";
        emit dispalyUserstext("[DB]  delete command error !");
    }
}

void MySql::createSinglehistorytable(int a_uid, int b_uid){
    QStringList tablelist = database.tables();
    sqlquery = new QSqlQuery(database);
    if(a_uid > b_uid){
        int temp = a_uid;
        a_uid = b_uid;
        b_uid = temp;
    }
    if(!tablelist.contains("singlehistorytable"+QString::number(a_uid)+QString::number(b_uid))){
        QString createusers = "CREATE TABLE singlehistorytable"+QString::number(a_uid)+QString::number(b_uid)+" (sender_uid int, receiver_uid int, time text, record text not null)";
        if(sqlquery->prepare(createusers)){
            if(sqlquery->exec()){
                qDebug() << "[DB] " << "create singlehistorytable"+QString::number(a_uid)+QString::number(b_uid)+" OK !";
                emit dispalyUserstext("[DB]  create singlehistorytable"+QString::number(a_uid)+QString::number(b_uid)+" OK !");
            }
        }
        else{
            qDebug() << "[DB] " << "create table command error !";
            emit dispalyUserstext("[DB]  create table command error !");
        }
    }
    else{
        qDebug() << "[DB] " << "singlehistorytable"+QString::number(a_uid)+QString::number(b_uid)+" exists !";
        emit dispalyUserstext("[DB]  singlehistorytable"+QString::number(a_uid)+QString::number(b_uid)+" exists !");
    }
}

void MySql::insertSinglehistory(int sender_uid, int receiver_uid, QString words, QString datetime){
    int a_uid, b_uid;
    if(sender_uid < receiver_uid){
        a_uid = sender_uid;
        b_uid = receiver_uid;
    }
    else{
        a_uid = receiver_uid;
        b_uid = sender_uid;
    }
    createSinglehistorytable(a_uid, b_uid);
    QString insert = "INSERT INTO singlehistorytable" + QString::number(a_uid)+QString::number(b_uid)+"(sender_uid, receiver_uid, time, record) VALUES("+QString::number(sender_uid)+","+QString::number(receiver_uid)+",'"+datetime+"','"+words+"')";
    if(sqlquery->prepare(insert)){
        if(sqlquery->exec()){
            qDebug() << "[DB] " << "insert record successfully !";
            emit dispalyUserstext("[DB]  insert record successfully !");
        }
    }
    else{
        qDebug() << "[DB] " << "insert fail !";
        emit dispalyUserstext("[DB]  insert fail !");
    }
}

QJsonObject MySql::queryHistorylist(int a_uid, int b_uid){
    if(a_uid > b_uid){
        int temp = a_uid;
        a_uid = b_uid;
        b_uid = temp;
    }
    QString str_select = "SELECT * FROM singlehistorytable"+QString::number(a_uid)+QString::number(b_uid);
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            QJsonArray history_list;
            while(sqlquery->next()){
                QJsonObject history_info;
                history_info.insert("sender_uid", QJsonValue(sqlquery->value(0).toString()));
                history_info.insert("receiver_uid", QJsonValue(sqlquery->value(1).toString()));
                history_info.insert("datetime", QJsonValue(sqlquery->value(2).toString()));
                history_info.insert("message",QJsonValue(sqlquery->value(3).toString()));
                history_list.append(QJsonValue(history_info));
            }
            QJsonObject json;
            json.insert("history_list", QJsonValue(history_list));
            qDebug() << "[DB] " << "show succeeded !";
            return json;

        }
        else{
            qDebug() << "[DB] " << "wrong here !";
        }
    }
    else{
        qDebug() << "[DB] " << "wrong user's uid !";
        emit dispalyUserstext("[DB]  wrong user's uid !");
    }
    return QJsonObject();
}
