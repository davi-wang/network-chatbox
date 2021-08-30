#ifndef VERIFICATION_H
#define VERIFICATION_H

#include <QWidget>
#include "QMessageBox"
#include"Repeater.h"
namespace Ui {
class Verification;
}

class Verification : public QWidget
{
    Q_OBJECT

public:
    explicit Verification(QWidget *parent = nullptr);
    ~Verification();

private slots:
    void on_OKBt_clicked();

private:
    Ui::Verification *ui;
};

#endif // VERIFICATION_H
