#include "tcpclient.h"
#include "ui_tcpclient.h"

TcpClient::TcpClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    this->setFixedSize(510, 380);
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

void TcpClient::on_eraseButton_clicked()
{
    ui->messageLine->clear();
    ui->messageLine->setFocus();
}

void TcpClient::on_exitButton_clicked()
{
    this->close();
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
            ui->textBrowser->append("Not connected");
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

void TcpClient::on_generateButton_clicked()
{
    QString qstrMessage = "(" + ui->amplitude->text() + "," + ui->frequency->text() + ")";

    if(!qstrMessage.isEmpty())
    {
        if(!connectFlag)
            ui->textBrowser->append("Not connected");
        else
        {
            memset(msg.str, 0, 512);
            strcpy(msg.str, qstrMessage.toStdString().c_str());
            msg.length = strlen(msg.str);
            int total_length = sizeof(msg.length) + msg.length;
            socket->write((char*)&msg, total_length);

            ui->textBrowser->append("(Amp,Freq) : ");
            ui->textBrowser->insertPlainText(qstrMessage);
        }
    }
}

void TcpClient::on_activateButton_clicked()
{
    QString qstrMessage = mode + ui->targetValue->text();

    if(!qstrMessage.isEmpty())
    {
        if(!connectFlag)
            ui->textBrowser->append("Not connected");
        else
        {
            memset(msg.str, 0, 512);
            strcpy(msg.str, qstrMessage.toStdString().c_str());
            msg.length = strlen(msg.str);
            int total_length = sizeof(msg.length) + msg.length;
            socket->write((char*)&msg, total_length);

            ui->textBrowser->append(ui->targetMode->currentText() +": "+ ui->targetValue->text());
        }
    }
    ui->targetValue->setFocus();
}

void TcpClient::on_targetMode_currentIndexChanged(int index)
{
    ui->targetValue->setValue(0);
    ui->targetValue->setFocus();

    if(ui->targetMode->currentText() == "Velocity")
    {
        ui->targetValue->setMaximum(20000);
        mode = "modeV";
    }
    else
    {
        ui->targetValue->setMaximum(200);
        mode = "modeT";
    }
}

