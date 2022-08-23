#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setFixedSize(530, 600);

    pTcpPacket = new TCP_Packet();



    QStringList RunTaskTypeCategory;

    RunTaskTypeCategory.append("None");
    RunTaskTypeCategory.append("Motor Control");

    ui->comboBox_RunTask_Type->addItems(RunTaskTypeCategory);


    QStringList RunTaskMotionCategory;

//    RunTaskMotionCategory.append("X");
//    RunTaskMotionCategory.append("Y");
    RunTaskMotionCategory.append("Z");
//    RunTaskMotionCategory.append("Circle");
//    RunTaskMotionCategory.append("Custom");

    ui->comboBox_RunTask_Motion->addItems(RunTaskMotionCategory);
}
MainWindow::~MainWindow()
{
    delete pTcpPacket;
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
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

        qDebug() << "on_udpButton_clicked";

    }
    else
    {
        delete UdpSocket;

        ui->actualVelocity->setText(" --- ");
        ui->actualTorque->setText(" --- ");
        ui->actualPosition->setText(" --- ");
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


// Position Control
void MainWindow::on_pushButton_SetTaskParam_clicked()
{
    // Task Parameter
    int iRunTaskType = ui->comboBox_RunTask_Type->currentIndex();
    int iRunTaskMotion = ui->comboBox_RunTask_Motion->currentIndex();

    float fRunTaskDisp = ui->RunTask_Displacement->text().toFloat();
    float fRunTaskPeriod = ui->RunTask_Period->text().toFloat();
    int iRunTaskRepeat = ui->RunTask_Repeat->text().toInt();

    taskParam.taskType = iRunTaskType;
    taskParam.taskMotion = iRunTaskMotion;

    taskParam.disp = fRunTaskDisp;
    taskParam.period = fRunTaskPeriod;
    taskParam.repeat = iRunTaskRepeat;

//    taskParam.footSwitch = ui->checkBox_RunTask_Apply_Foot_Pedal->isChecked();
    taskParam.dataApply = ui->checkBox_RunTask_Apply_Data->isChecked();


    uint16_t header = COMMAND_SET_TASK_PARAM;

    pTcpPacket->setCommandHeader(header);
    pTcpPacket->encode(taskParam);
    pTcpPacket->sendPacket();
}
void MainWindow::on_pushButton_CSP_Run_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = COMMAND_RUN_CSP;
        bool bRunStart = true;

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(bRunStart);
        pTcpPacket->sendPacket();
    }
}
void MainWindow::on_pushButton_CSP_Stop_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = COMMAND_RUN_CSP;

        bool bRunStart = false;

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(bRunStart);
        pTcpPacket->sendPacket();

        AppendText("[Stop Motor]");
    }
}


// Velocity Control
void MainWindow::on_pushButton_CSV_Run_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header= COMMAND_RUN_CSV;
        bool bRunStart = true;

        int32_t velocity = GEAR_RATIO * ui->targetValue->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(bRunStart);
        pTcpPacket->encode(velocity);
        pTcpPacket->sendPacket();

        AppendText("[Velocity] " + ui->targetValue->text());
    }
    ui->targetValue->setFocus();
}
void MainWindow::on_pushButton_CSV_Stop_clicked()
{
    on_pushButton_CSP_Stop_clicked();
}


// Torque Control
void MainWindow::on_pushButton_CST_Run_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = COMMAND_RUN_CST;
        bool bRunStart = true;

        int16_t torque = ui->targetValue_2->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(bRunStart);
        pTcpPacket->encode(torque);
        pTcpPacket->sendPacket();

        AppendText("[Torque] " + ui->targetValue_2->text());
    }
    ui->targetValue_2->setFocus();
}
void MainWindow::on_pushButton_CST_Stop_clicked()
{
    on_pushButton_CSP_Stop_clicked();
}


void MainWindow::readPacket()
{
    QByteArray rxData;
    QHostAddress sender;
    uint16_t senderPort;

    while(UdpSocket->hasPendingDatagrams())
    {
        qDebug() << "readPacket";

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
                ui->actualVelocity->setText(QString::number(logData.actualVelocity / GEAR_RATIO));
                ui->actualTorque->setText(QString::number(logData.actualTorque));
                ui->actualPosition->setText(QString::number(logData.actualPosition));
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
