#include "tcpclient.h"
#include "ui_tcpclient.h"

TcpClient::TcpClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    // this->setFixedSize(500, 400);
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));

    socket->connectToHost("192.168.0.10", 2000);
    connectFlag = socket-> waitForConnected();

    if(!connectFlag)
    {
        ui->textBrowser->append("Error: ");
        ui->textBrowser->insertPlainText(socket->errorString());
    }
}

void TcpClient::connected()
{
    ui->textBrowser->append("Connected!");
}

void TcpClient::disconnected()
{
    connectFlag = false;
    ui->textBrowser->append("disconnected...");
}

void TcpClient::on_connectButton_clicked()
{
    if(!connectFlag)
    {
        ui->textBrowser->append("Connecting...");
        doConnect();
    }
    else
        ui->textBrowser->append("Already connected");
}

void TcpClient::on_messageLine_returnPressed()
{
    on_sendButton_clicked();
}

void TcpClient::on_sendButton_clicked()
{
    QString qstrMessage = ui->messageLine->text();

    if(!qstrMessage.isEmpty())
    {
        if(!connectFlag)
            ui->textBrowser->append("Not connected to server yet");
        else
        {
            memset(msg.str, 0, 512);
            strcpy(msg.str, qstrMessage.toStdString().c_str());
            msg.length = strlen(msg.str);
            int total_length = sizeof(msg.length) + msg.length;
            socket->write((char*)&msg, total_length);

            ui->textBrowser->append("Message: ");
            ui->textBrowser->insertPlainText(qstrMessage);
            ui->messageLine->clear();
        }
    }
    ui->messageLine->setFocus();
}

void TcpClient::on_eraseButton_clicked()
{
    ui->messageLine->clear();
    ui->messageLine->setFocus();
}

void TcpClient::on_exitButton_clicked()
{
    this->close();
}



