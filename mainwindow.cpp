#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setFixedSize(530, 600);

    pTcpPacket = new TCP_Packet();
}

MainWindow::~MainWindow()
{
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


void MainWindow::on_udpButton_clicked(bool checked)
{
    if(checked)
    {
        UdpSocket = new QUdpSocket();

        //    UdpSocket->bind(QHostAddress::Any, UDP_PORT);
        QHostAddress GUI_PC_Address;
        GUI_PC_Address.setAddress(QStringLiteral(GUI_PC_IP));
        UdpSocket->bind(GUI_PC_Address, UDP_PORT);

        connect(UdpSocket,SIGNAL(readyRead()),this,SLOT(readPacket()));
    }
    else
    {
        delete UdpSocket;

        ui->actualVelocity->setText(" --- ");
        ui->actualTorque->setText(" --- ");
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
        ui->targetValue->setRange(-20000, 20000);
    }
    else if(ui->targetMode->currentText() == "Torque")
    {
        ui->targetValue->setRange(-200, 200);
    }
    else if(ui->targetMode->currentText() == "Position")
    {
        ui->targetValue->setRange(-99999,99999);
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
            header = COMMAND_MODE_CSV;
        else if(ui->targetMode->currentText() == "Torque")
            header = COMMAND_MODE_CST;
        else if(ui->targetMode->currentText() == "Position")
            header = COMMAND_MODE_PPM;
        else if(ui->targetMode->currentText() == "B&F")
            header = COMMAND_MODE_BACK_AND_FORTH_VELOCITY;
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
        short header = COMMAND_MODE_CSV;
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
        short header = COMMAND_MODE_CSV;
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
        short header = COMMAND_MODE_SINUSOIDAL_VELOCITY;

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
        case(STREAM_MODE):
            this->decode(logData);

            if(rxCount == (1000/30))    // 1kHz --> 30Hz
            {
                ui->actualVelocity->setText(QString::number(logData.velocity_actual_value));
                ui->actualTorque->setText(QString::number(logData.torque_actual_value));
                rxCount = 0;
            }


            // Logging State
            static int PlotCnt = 0;
            static unsigned int timeMsLogInit = 0;
            static bool timeLogFlag = true;

//            if(bLogCheck)
//            {
////                mMutex.lock();

//                if(timeLogFlag)
//                {
//                    timeMsLogInit = logData.timeStamp;
//                    timeLogFlag = false;
//                }

//                unsigned int timeMsLog = logData.timeStamp - timeMsLogInit;
//                unsigned int timeSecLog = (unsigned int)(((float)timeMsLog)/1000.0);
//                unsigned int timeMinuteLog = (unsigned int)(((float)timeSecLog)/60.0);

//                if(PlotCnt % 999 == 0)
//                {
//                    QString qTimeLog;
//                    qTimeLog.sprintf("%02d Minute %02d Sec",timeMinuteLog,timeSecLog-timeMinuteLog*60);

//                    ui->textEdit_LogTimer->setText(qTimeLog);
//                }
//                mDataLogger.setQueuePacket(logData);

////                mMutex.unlock();
//            }
//            else
//            {
//                timeLogFlag = true;
//            }


            break;
        }

        rxCount++;
    }
}

void MainWindow::on_pushButton_Data_Logging_clicked()
{
    qDebug() << "Logging Button Click!";

    if(bRegLogging == false)
    {
        QString logName = ui->textEdit_LogFileName->toPlainText();
        mDataLogger.setLogFileName(logName);
    }

    static bool bCheck = ui->pushButton_Data_Logging->isChecked();

    bCheck = bCheck ^ 1;

    if(bCheck)
    {
        bLogCheck = bCheck;
        qDebug() << "Click State 111!" << bLogCheck;

        this->ui->pushButton_Data_Logging->setText("Logging...");
        this->ui->pushButton_Data_Logging->setStyleSheet("QPushButton{ background-color: red }");

        AppendText("Logging Start!");

        mDataLogger.LogStart();

        //        mDataLogger.start();
        mDataLogger.bRun = true;

    }
    else
    {
        bLogCheck = bCheck;
        qDebug() << "Click State 222!" << bLogCheck;

        this->ui->pushButton_Data_Logging->setText("Data Logging");
        this->ui->pushButton_Data_Logging->setStyleSheet("QPushButton{ background-color: white }");

        AppendText("Loggint Stop!");

        mDataLogger.bRun = false;

        mDataLogger.LogStop();

        //        mDataLogger.quit();


    }
}
