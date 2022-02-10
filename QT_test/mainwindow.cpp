#include "mainwindow.h"
#include "ui_mainwindow.h"

static inline QByteArray IntToArray(qint32 source);

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(500, 400);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    socket = new QTcpSocket(this);
    fd_flag = connectToHost("192.168.0.10"); // localhost

    if(!fd_flag)
        ui->textBrowser->insertPlainText("Socket connect failed...\n");
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::connectToHost(QString host)
{
    socket->connectToHost(host, 2000); // ip address, port
    return socket->waitForConnected();
}

bool MainWindow::writeData(QByteArray data)
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

void MainWindow::on_sendBtn_clicked()
{
    if(fd_flag)
    {
        QString text = ui->sendDataLine->text();
        qsnprintf(buf, sizeof(buf),"%s",text.toUtf8().constData());
        buf[strlen(buf) - 1] = '\0';

        text.sprintf("Data: %d\n", buf);
        ui->textBrowser->insertPlainText(text);

        send_flag = writeData(buf);

        if(!send_flag)
            ui->textBrowser->insertPlainText("Socket send fail\n");
        else
            cnt++;
    }
}
