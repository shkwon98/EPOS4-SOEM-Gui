#include "tcpclient.h"
#include "ui_tcpclient.h"

TcpClient::TcpClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
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
    QString message = ui->messageLine->text();

    if(!message.isEmpty())
    {
        if(!connectFlag)
            ui->textBrowser->append("Not connected to server yet");
        else
        {
            memset(buf, 0, BUFSIZE);
            strcpy(buf, (char*)message.toStdString().c_str());
            buf[strlen(buf) - 1] = '\0';

            packetST *pMessage = (packetST*)buf;
            strcpy(pMessage->str, buf);
            pMessage->length = strlen(pMessage->str);
            int total_length = sizeof(pMessage->length) + pMessage->length;

            socket->write((char*)&buf, total_length);

            ui->textBrowser->append("Message: ");
            ui->textBrowser->insertPlainText(message);
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



