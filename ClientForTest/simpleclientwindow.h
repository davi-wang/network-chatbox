#ifndef SIMPLECLIENTWINDOW_H
#define SIMPLECLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QObject>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "connection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SimpleClientWindow; }
QT_END_NAMESPACE

class SimpleClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    SimpleClientWindow(QWidget *parent = nullptr);
    ~SimpleClientWindow();

private slots:
    void on_pushButton_1_clicked();
    void connectedToServer();
    void process(Connection::DataType, const QJsonObject &);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::SimpleClientWindow *ui;
    Connection *connection;
};
#endif // SIMPLECLIENTWINDOW_H
