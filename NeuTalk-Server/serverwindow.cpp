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

void ServerWindow::resizeEvent(QResizeEvent* event)
{
    qDebug() << "resizing????";
    ui->textEdit->update();
}
