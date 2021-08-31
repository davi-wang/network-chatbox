#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>

#include "server.h"
#include "mysql.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void displayLine(const QString &);

    void on_actionopen_triggered();

    void on_actionclose_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionblank_line_triggered();

private:
    Ui::ServerWindow *ui;
    int font_size;
};
#endif // SERVERWINDOW_H
