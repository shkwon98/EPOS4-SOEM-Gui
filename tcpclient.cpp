#include "tcpclient.h"
#include "ui_tcpclient.h"

TcpClient::TcpClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
//    this->setFixedSize(510, 380);
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

QString TcpClient::value(QString num)
{
    return QString("%1").arg(num.toInt(), 8, 10, QLatin1Char('0'));
}

void TcpClient::on_generateButton_clicked()
{
    mode = "s001";
    QString qstrMessage = mode + value(ui->amplitude->text()) + value(ui->frequency->text());

    if(!connectFlag)
        ui->textBrowser->append("Not connected");
    else
    {
        memset(msg.str, 0, 512);
        strcpy(msg.str, qstrMessage.toStdString().c_str());
        msg.length = strlen(msg.str);
        int total_length = sizeof(msg.length) + msg.length;
        socket->write((char*)&msg, total_length);

        ui->textBrowser->append("(Amp, Freq) : (" + ui->amplitude->text() + ", " + ui->frequency->text() + ")");
    }
}

void TcpClient::on_targetMode_currentIndexChanged(int index)
{
    ui->targetValue->setEnabled(true);
    ui->targetValue->setValue(0);
    ui->targetValue->setFocus();

    if(ui->targetMode->currentText() == "Velocity")
    {
        ui->targetValue->setMinimum(-20000);
        ui->targetValue->setMaximum(20000);
    }
    else if(ui->targetMode->currentText() == "Torque")
    {
        ui->targetValue->setMinimum(-200);
        ui->targetValue->setMaximum(200);
    }
    else if(ui->targetMode->currentText() == "B&F")
    {
        ui->targetValue->clear();
        ui->targetValue->setDisabled(true);
    }
}

void TcpClient::on_activateButton_clicked()
{
    if(ui->targetMode->currentText() == "Velocity")
        mode = "v001";

    else if(ui->targetMode->currentText() == "Torque")
        mode = "t001";

    else if(ui->targetMode->currentText() == "B&F")
        mode = "v002";

    QString qstrMessage = mode + value(ui->targetValue->text());

    if(!connectFlag)
        ui->textBrowser->append("Not connected");
    else
    {
        memset(msg.str, 0, 512);
        strcpy(msg.str, qstrMessage.toStdString().c_str());
        msg.length = strlen(msg.str);
        int total_length = sizeof(msg.length) + msg.length;
        socket->write((char*)&msg, total_length);

        ui->textBrowser->append("[" + ui->targetMode->currentText()+ "] " + value(ui->targetValue->text()));
    }
    ui->targetValue->setFocus();
}

void TcpClient::on_dial_valueChanged(int value)
{
    ui->velocity->setText(QString::number(value));
}

void TcpClient::on_cwButton_pressed()
{
    mode = "v001";
    QString qstrMessage = mode + value(ui->velocity->text());

    if(!connectFlag)
        ui->textBrowser->append("Not connected");
    else
    {
        memset(msg.str, 0, 512);
        strcpy(msg.str, qstrMessage.toStdString().c_str());
        msg.length = strlen(msg.str);
        int total_length = sizeof(msg.length) + msg.length;
        socket->write((char*)&msg, total_length);
    }
}

void TcpClient::on_cwButton_released()
{
    mode = "v001";
    QString qstrMessage = mode + value("0");

    if(!connectFlag)
        ui->textBrowser->append("Not connected");
    else
    {
        memset(msg.str, 0, 512);
        strcpy(msg.str, qstrMessage.toStdString().c_str());
        msg.length = strlen(msg.str);
        int total_length = sizeof(msg.length) + msg.length;
        socket->write((char*)&msg, total_length);
    }
}

void TcpClient::on_ccwButton_pressed()
{
    mode = "v001";
    QString qstrMessage = mode + value("-" + ui->velocity->text());

    if(!connectFlag)
        ui->textBrowser->append("Not connected");
    else
    {
        memset(msg.str, 0, 512);
        strcpy(msg.str, qstrMessage.toStdString().c_str());
        msg.length = strlen(msg.str);
        int total_length = sizeof(msg.length) + msg.length;
        socket->write((char*)&msg, total_length);
    }
}

void TcpClient::on_ccwButton_released()
{
    on_cwButton_released();
}
