#ifndef SIGNON_H
#define SIGNON_H

#include <QWidget>
#include <signin.h>
#include <QMessageBox>

namespace Ui { class Signon; }

class Signon : public QWidget
{
    Q_OBJECT

public:
    explicit Signon(QWidget *parent = nullptr);
    ~Signon();

private slots:
    void on_pushButton_2_clicked(); //Finish按钮

private:
    Ui::Signon *ui;
};

#endif // SIGNON_H
