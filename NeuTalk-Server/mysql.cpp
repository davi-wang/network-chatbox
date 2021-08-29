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
//        qDebug()<<"database open OK !";
        emit dispalyUserstext("database open OK !");
        createUserstable();
        return true;
    }
    else{
//        qDebug()<<"database open failed !";
        emit dispalyUserstext("database open failed !");
        return false;
    }
}

void MySql::createUserstable(){
    QStringList tablelist = database.tables();
    sqlquery = new QSqlQuery(database);
    if(!tablelist.contains("userstable")){
        QString createusers = "CREATE TABLE userstable (num int, email text, password text, primary key(num, email))";
        if(sqlquery->prepare(createusers)){
            if(sqlquery->exec()){
//                qDebug()<<"create userstable OK !";
                emit dispalyUserstext("create userstable OK !");
            }
        }
        else{
//            qDebug()<<"create table command error !";
            emit dispalyUserstext("create table command error !");
        }
    }
    else{
//        qDebug()<<"userstable exists !";
        emit dispalyUserstext("userstable exists !");
    }
}

void MySql::registerUser(client &a){
    QString num = QString::number(a.num);
    QString insert = "INSERT INTO userstable(num,email,password) VALUES("+num+",'"+a.email+"','"+a.password+"')";
    if(sqlquery->prepare(insert)){
        if(sqlquery->exec()){
//            qDebug()<<"registered successfully !";
            emit dispalyUserstext("registered successfully !");
        }
        else{
//            qDebug()<<"primary key cannot same!";
            emit dispalyUserstext("primary key cannot same!");
        }
    }
    else{
//        qDebug()<<"registered fail !";
        emit dispalyUserstext("registered fail !");
    }
}

bool MySql::login(QString email, QString password){
    QString str_select = "SELECT * FROM userstable WHERE email = '"+email+"'";
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            int flag = 0;
            while(sqlquery->next()){
                if(password == sqlquery->value(2).toString()){
//                    qDebug()<<"denglu successfully !";
                    emit dispalyUserstext("denglu successfully !");
                    //登陆成功，跳转页面
                    flag = 1;
                    return true;
                }
                else{
//                    qDebug()<<"wrong password !";
                    emit dispalyUserstext("wrong password !");
                    flag = -1;
                }
            }
            if(flag == 0)
//                qDebug()<<"email does not exist !";
                emit dispalyUserstext("email does not exist !");
        }
    }
    else{
//        qDebug()<<"denglu failed !";
        emit dispalyUserstext("denglu failed !");
    }
    return false;
}

void MySql::queryUserinfo(QString email){
    QString str_select = "SELECT * FROM userstable WHERE email = '"+email+"'";
    if(sqlquery->prepare(str_select)){
        if(sqlquery->exec()){
            while(sqlquery->next()){
//                qDebug()<<"selected successfully !";
                emit dispalyUserstext("selected successfully !");
//                qDebug()<<"num:"<<sqlquery->value(0).toString()
//                       <<"email:"<<sqlquery->value(1).toString()
//                      <<"password:"<<sqlquery->value(2).toString();
                emit dispalyUserstext("num:"+sqlquery->value(0).toString()
                                      +"email:"+sqlquery->value(1).toString()
                                      +"password"+sqlquery->value(2).toString());
            }
        }
    }
    else{
//        qDebug()<<"select command error !";
        emit dispalyUserstext("select command error !");
    }
}

void MySql::dropTable(QString tableName){
    QString str_drop = "DROP TABLE "+tableName;
    if(sqlquery->prepare(str_drop)){
        if(sqlquery->exec()){
//            qDebug()<<"drop table OK !";
            emit dispalyUserstext("drop table OK !");
        }
    }
    else{
//        qDebug()<<"drop command error !";
        emit dispalyUserstext("drop command error !");
    }
}
