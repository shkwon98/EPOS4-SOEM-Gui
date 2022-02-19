#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setFixedSize(510, 380);

    pTcpPacket = new TCP_Packet();

    UdpSocket = new QUdpSocket();
//    UdpSocket->bind(QHostAddress::Any, UDP_PORT);

    QHostAddress Raspberry_Pi_Address;
    Raspberry_Pi_Address.setAddress(RASPBERRY_PI_IP);
    UdpSocket->bind(Raspberry_Pi_Address, UDP_PORT);
    connect(UdpSocket,SIGNAL(readyRead()),this,SLOT(readPacket()));
}

MainWindow::~MainWindow()
{
    if(UdpSocket->isOpen())
    {
        UdpSocket->disconnectFromHost();
        UdpSocket->close();
        UdpSocket->abort();
    }

    delete pTcpPacket;
    delete ui;
}

void MainWindow::AppendText(QString str)
{
    QString appendStr;

    appendStr.append(str);

    ui->inputBrowser->append(appendStr);

//    static int lineCnt = 0;
//    lineCnt++;

//    if(lineCnt > 50)
//    {
//        ui->inputBrowser->clear();
//        lineCnt = 0;
//    }
}

void MainWindow::readPacket()
{
    QByteArray rxData;
    QHostAddress sender;
    uint16_t senderPort;

    while(UdpSocket->hasPendingDatagrams())
    {
        rxData.fill(0, UdpSocket->pendingDatagramSize());
        UdpSocket->readDatagram(rxData.data(), rxData.size(), &sender, &senderPort);

        memcpy(rxBuffer, rxData.data(), rxData.size());

        for (int i = 0; i < RX_BUFFER_SIZE; i++)
        {
            if (rxBuffer[i] == 13 && rxBuffer[i + 1] == 10)
            {
                memcpy(packetBuffer, &rxBuffer[i + 2], sizeof(packetBuffer));
                break;
            }
        }

        header = 0;
        decodeIndex = 2;

        memcpy(&header, &packetBuffer[0], sizeof(short));

        switch(header)
        {
        case(1):
            int velocity;
            int16_t torque;

            this->decode(velocity);
            this->decode(torque);

            ui->actualVelocity->setText(QString::number(velocity));
            ui->actualTorque->setText(QString::number(torque));
            break;
        }
    }
}

void MainWindow::on_tcpConnectButton_clicked()
{
    connectFlag = pTcpPacket->connect();

    if(connectFlag)
        AppendText("Raspberry Pi Connect Success!");
    else
        AppendText("Raspberry Pi Connect Fail!");
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::on_stopButton_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = 0000;
        int iData = 0;

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(iData);
        pTcpPacket->sendPacket();
    }
}


void MainWindow::on_targetMode_currentIndexChanged(int index)
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

void MainWindow::on_activateButton_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {   short header;

        if(ui->targetMode->currentText() == "Velocity")
            header = 0001;
        else if(ui->targetMode->currentText() == "Torque")
            header = 0002;
        else if(ui->targetMode->currentText() == "B&F")
            header = 0003;
        else
            header = 0000;

        int iData = ui->targetValue->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(iData);
        pTcpPacket->sendPacket();

        AppendText("[" + ui->targetMode->currentText()+ "] " + ui->targetValue->text());
    }
    ui->targetValue->setFocus();
}

void MainWindow::on_cwButton_pressed()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = 0001;
        int iData = ui->velocity->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(iData);
        pTcpPacket->sendPacket();
    }
}
void MainWindow::on_ccwButton_pressed()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = 0001;
        int iData = ui->velocity->text().toInt() * (-1);

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(iData);
        pTcpPacket->sendPacket();
    }
}
void MainWindow::on_cwButton_released()
{
    on_stopButton_clicked();
}
void MainWindow::on_ccwButton_released()
{
    on_stopButton_clicked();
}

void MainWindow::on_generateButton_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = 0004;

        SINUSOIDAL sineData;
        sineData.amplitude = ui->amplitude->text().toInt();
        sineData.frequency = ui->frequency->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(sineData.amplitude);
        pTcpPacket->encode(sineData.frequency);
        pTcpPacket->sendPacket();

        AppendText("(Amp, Freq) : (" + ui->amplitude->text() + ", " + ui->frequency->text() + ")");
    }
}
