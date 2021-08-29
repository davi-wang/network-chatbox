#ifndef PROCESSINGMSG_H
#define PROCESSINGMSG_H
#include"connection.h"
#include <QObject>
#include<QJsonObject>
#include<QJsonDocument>
#include<QBitArray>
class ProcessingMsg:public QObject
{
    Q_OBJECT
public:
    ProcessingMsg();

};

#endif // PROCESSINGMSG_H
