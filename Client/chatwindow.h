#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QObject>
#include <QDebug>

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

private slots:
    void closeEvent(QCloseEvent *event) override;
    void newMessage();
  //  void displayHistory(const QJsonObject &);
    void LoadHistory();
    void on_sendBtn_clicked();


private:
    void sendMessage();
    void displayMessage(QString, const QString&, const QString&);
    Ui::ChatWindow *ui;
    QString friend_nickname;
    int to_uid;
    int from_uid;
    ClientServer* client ;



};

#endif // CHATWINDOW_H
