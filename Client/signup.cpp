#include "signup.h"
#include<QCryptographicHash>
SignUp::SignUp(Connection*Connector)
{
    this->Connector=Connector;

}


void SignUp::SignUp_GetEmail(QString Verify)
{
    QJsonObject SignUP;
    SignUP.insert("Verify",Verify);
    Connector->sendMessage(Connection::R4_request_register,SignUP);
}

void SignUp::SignUp_GetUser(QString UsrName, QString password, QString Email)
{



    QCryptographicHash hash(QCryptographicHash::Md5);
    QByteArray byte_array;
    byte_array.append(password);
    hash.addData(byte_array);
    QByteArray result_byte_array = hash.result();  //返回最终的哈希值
    QString md5PassWord = result_byte_array.toHex();

    QJsonObject SignUP;
    SignUP.insert("Type",Connection::R1_request_email);
    SignUP.insert("Time",time.currentTime().toString("hh:mm:ss"));
    SignUP.insert("U_name",UsrName);
    SignUP.insert("Password",md5PassWord);
    SignUP.insert("Email",Email);
    Connector->sendMessage(Connection::R1_request_email,SignUP);

}

void SignUp::SignIn(QString Email, QString password)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QByteArray byte_array;
    byte_array.append(password);
    hash.addData(byte_array);
    QByteArray result_byte_array = hash.result();  //返回最终的哈希值
    QString md5PassWord = result_byte_array.toHex();

    QJsonObject SignIN;
    SignIN.insert("Type",Connection::L1_request_login);
    SignIN.insert("Time",time.currentTime().toString("hh:mm:ss"));
    SignIN.insert("Password",md5PassWord);
    SignIN.insert("Email",Email);
    Connector->sendMessage(Connection::L1_request_login,SignIN);
}


void SignUp::RecieveMsg_Reg(Connection::DataType header, const QJsonObject data)
{
    if(header==Connection::R2_verification_sending)
        emit EmailSENGING();
    if(header==Connection::R3_verification_sent)
        emit EmailVERIFY();
    if(header==Connection::R6_success)
        emit Success(data);
    if(header==Connection::R5_fail)
     {
        emit Fail(data);
    }
}

void SignUp::RecieveMsg_log(Connection::DataType header, const QJsonObject data)
{
    if(header==Connection::L2_logging)
        emit logging();
    if(header==Connection::L3_fail)
        emit Fail_log(data);
    if(header==Connection::L4_success)
        emit SuccessLog();
    if(header==Connection::L5_synchro_data)
        emit GetSynchro_Data(data);
    if(header==Connection::L6_synchronization_complete)
        emit Synchro_DataComplete();
}
