#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QObject>
#include <QDebug>
#include<DataTypes.h>
#include "connection.h"
#include "Repeater.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr,
                        QString friend_name = "", int friend_uid = -1, int my_uid = -1);
    ~ChatWindow();
    void newMessage(const QJsonObject &);
    void LoadHistory(const QJsonObject &);

private slots:
    void closeEvent(QCloseEvent *event) override;
    void on_sendBtn_clicked();

private:
    void sendMessage();
    void displayMessage(QString, const QString &, const QString &,const QString &,const int &,const int,const int);
    Ui::ChatWindow *ui;
    QString friend_nickname;
    int to_uid;
    int from_uid;
    ClientServer* client ;
    QVector<QJsonObject> his;



};

#endif // CHATWINDOW_H
