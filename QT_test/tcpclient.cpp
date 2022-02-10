#include "tcpclient.h"
#include "ui_tcpclient.h"

static inline QByteArray IntToArray(qint32 source);

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    fd_flag = connectToHost("127.0.0.1"); // localhost

    if(!fd_flag)
        ui->textBrowser->insertPlainText("Socket connect fail\n");
}

Client::~Client()
{
    delete ui;
}

bool Client::connectToHost(QString host)
{
    socket->connectToHost(host, 1024); // ip address, port
    return socket->waitForConnected();
}

bool Client::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(IntToArray(data.size())); // write size of data
        socket->write(data); // write the data itself
        return socket->waitForBytesWritten();
    }
    else
    {
        return false;
    }
}

QByteArray IntToArray(qint32 source) // Use qint32 to ensure that the number have 4 bytes
{
    // Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

void Client::on_pushButton_clicked()
{
    if(fd_flag)
    {
        QString textData, sendData;
        textData.sprintf("Button click: %d\n", cnt);
        sendData.sprintf("Socket data: %d\n", cnt);
        ui->textBrowser->insertPlainText(textData);
        send_flag = writeData(sendData.toStdString().c_str());

        if(!send_flag)
            ui->textBrowser->insertPlainText("Socket send fail\n");
        else
            cnt++;
    }
}
