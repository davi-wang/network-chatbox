#include "serverwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWindow w;
    w.show();
    qDebug() << 4 + 4 + 1 + 17 + 9 +6 + 7;
    return a.exec();
}
