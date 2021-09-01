#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    font_size = 11;
    ui->textEdit->setFont(QFont("Consolas", font_size));
    ui->textEdit->setTextColor(QColor(0,225,90));

    Server* pServer = Server::getInstance();
    MySql* pDatabase = MySql::gethand();

    connect(pServer, SIGNAL(displayText(const QString &)), this, SLOT(displayLine(const QString &)));
    connect(pDatabase, SIGNAL(dispalyUserstext(const QString &)), this, SLOT(displayLine(const QString &)));

    displayLine("[INFO] Initializing database...");
    pDatabase->openDatabase();
    displayLine("[INFO] Starting NeuTalk Server...");
    pServer->getHostInfo();
}

void ServerWindow::displayLine(const QString &plain_text)
{
    ui->textEdit->append(QTime::currentTime().toString("hh:mm:ss") + " " + plain_text);
}

ServerWindow::~ServerWindow()
{
//    QString filename = tr(QDateTime::currentDateTime().toString() + ".log");
//    QFile log_output(filename);
//    if (log_output.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        QTextStream out(&log_output);
//        out << ui->textEdit->toPlainText();
//    }
//    log_output.close();
    delete ui;
}

void ServerWindow::on_actionopen_triggered()
{
    Server* pServer = Server::getInstance();
    pServer->startServer();
}

void ServerWindow::on_actionclose_triggered()
{
    Server* pServer = Server::getInstance();
    pServer->stopServer();
}

void ServerWindow::on_actionZoom_in_triggered()
{
    if (font_size < 50) font_size = font_size + 2;
    ui->textEdit->setFont(QFont("Consolas", font_size));
}

void ServerWindow::on_actionZoom_out_triggered()
{
    if (font_size > 4) font_size = font_size - 2;
    ui->textEdit->setFont(QFont("Consolas", font_size));
}

void ServerWindow::on_actionblank_line_triggered()
{
    ui->textEdit->append(" - ");
    ui->textEdit->append(" - ");
    ui->textEdit->append(" - ");
}

void ServerWindow::on_actioninfo_triggered()
{
    Server* pServer = Server::getInstance();
    displayLine("[INFO] Server IP: " + pServer->hostadd_list.at(2).toString());
    displayLine("[INFO] Server Port: " + pServer->default_port);
}

void ServerWindow::on_actiononlines_triggered()
{
    Server* pServer = Server::getInstance();
    MySql* database = MySql::gethand();
    qDebug() << pServer->onlines;
    QList<int> onlines_uid = pServer->onlines.keys();
    qDebug() << "mapsize" << pServer->onlines.size();
    displayLine("[INFO] All logged in clients:");
    displayLine("[INFO] ---------uid-----email------------name--");
    for (const int &uid: onlines_uid) {
        QString name = database->queryUser(uid).value("nickname").toString();
        displayLine("[INFO] Online: " + QString::number(uid) + " - " +
                    pServer->onlines[uid]->peerAddress().toString() + " - " + name);
    }
    displayLine("[INFO] ------------------------------------------");
}

void ServerWindow::on_actionSocket_Receive_triggered()
{
    Server* pServer = Server::getInstance();
    pServer->display_socket_debug = !(pServer->display_socket_debug);
    if (pServer->display_socket_debug) {
        displayLine("[SETTINGS] Now display all DataType received from sockets.");
    }
    else {
        displayLine("[SETTINGS] Now hide all DataType received from sockets.");
    }
}

void ServerWindow::on_actionJSON_SENT_triggered()
{
    Server* pServer = Server::getInstance();
    pServer->display_json_sent_debug = !(pServer->display_json_sent_debug);
    if (pServer->display_json_sent_debug) {
        displayLine("[SETTINGS] Now display some json data sent.");
    }
    else {
        displayLine("[SETTINGS] Now hide some json data sent.");
    }
}
