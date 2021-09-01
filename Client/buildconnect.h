#ifndef BUILDCONNECT_H
#define BUILDCONNECT_H

#include <QWidget>
#include "Repeater.h"
#include "QMessageBox"

namespace Ui {
class BuildConnect;
}

class BuildConnect : public QWidget
{
    Q_OBJECT

public:
    explicit BuildConnect(QWidget *parent = nullptr);
    ~BuildConnect();

private slots:
    void on_Build_clicked();

private:
    Ui::BuildConnect *ui;
    ClientServer * Repeater;
    QString Ip;
    QString Port;
};

#endif // BUILDCONNECT_H
