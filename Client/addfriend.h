#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include"Repeater.h"
namespace Ui {
class addfriend;
}

class addfriend : public QWidget
{
    Q_OBJECT

public:
    explicit addfriend(QWidget *parent = nullptr);
    ~addfriend();

private:
    Ui::addfriend *ui;
};

#endif // ADDFRIEND_H
