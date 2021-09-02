#ifndef DATATYPES_H
#define DATATYPES_H
#include<QString>
#include<QDateTime>
#include<QFont>

enum VerificationError
{
    NoError=0,
    InvalidEmail=1,
    Timeout=2,
    WrongCode=3
};


struct ReceieveText
{
    int uid ;//uid
    QString name;//昵称
    int friend_uid;  // 是谁发给我的消息
    QString new_message;  // 聊天内容
    QString datetime_str;  // 什么时候发的，字符串格式
    QDateTime datetime ; // 什么时候发的格式处理



};

struct ChatRecord
{
    int sender_uid ;
    int receiver_uid;
    QString datetime;
    QString message;
};

struct UsrInfo
{
    int u_id;
    QString Email;
    QString NickName;
};



#endif // DATATYPES_H
