#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>

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

    void on_actioninfo_triggered();

    void on_actiononlines_triggered();

    void on_actionSocket_Receive_triggered();

    void on_actionJSON_SENT_triggered();

private:
    Ui::ServerWindow *ui;
    int font_size;
};
#endif // SERVERWINDOW_H
