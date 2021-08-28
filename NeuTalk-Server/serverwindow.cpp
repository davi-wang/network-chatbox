#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setFont(QFont("Consolas", 11));
    ui->textEdit->setTextColor(QColor(105,255,195));

    Server* pServer = Server::getInstance();

    connect(pServer, SIGNAL(displayText(const QString &)), this, SLOT(displayLine(const QString &)));

    // 默认自动开启服务
    displayLine("[INFO] Starting NeuTalk Server...");
    pServer->getHostInfo();
}

void ServerWindow::displayLine(const QString &plain_text)
{
    QTime *time = new QTime();
    ui->textEdit->append(time->currentTime().toString("hh:mm:ss") + " " + plain_text);
    delete time;
}

ServerWindow::~ServerWindow()
{
    delete ui;
}
