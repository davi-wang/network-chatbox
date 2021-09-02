#ifndef PCR_H
#define PCR_H

#include <QWidget>
#include <QObject>
#include <QDebug>

#include "Repeater.h"

namespace Ui {
class PCR;
}

class PCR : public QWidget
{
    Q_OBJECT

public:
    explicit PCR(QWidget *parent = nullptr,
                 int my_uid = -1);
    ~PCR();

private slots:
    void closeEvent(QCloseEvent *event) override;
    void newMessage(const QJsonObject &);

    void updateList(const QJsonObject &);

    void on_sendBtn_clicked();

private:
    void sendMessage();
    void displayMessage(QString, const QString&, const QString&);
    Ui::PCR *ui;
    int my_uid;
};

#endif // PCR_H
