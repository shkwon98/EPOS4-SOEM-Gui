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

    RunTaskMotionCategory.append("X");
    RunTaskMotionCategory.append("Y");
    RunTaskMotionCategory.append("Z");
    RunTaskMotionCategory.append("Circle");
    RunTaskMotionCategory.append("Custom");

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


void MainWindow::on_stopButton_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = COMMAND_MODE_STOP_MOTOR;

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->sendPacket();

        AppendText("[Stop Motor]");
    }
}
void MainWindow::on_setZeroButton_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {   short header = COMMAND_MODE_SET_ZERO;

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->sendPacket();

        AppendText("[Set Zero]");
    }
}

void MainWindow::on_runButton_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {   short header= COMMAND_MODE_CSV;

        int32_t velocity = GEAR_RATIO * ui->targetValue->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(velocity);
        pTcpPacket->sendPacket();

        AppendText("[Velocity] " + ui->targetValue->text());
    }
    ui->targetValue->setFocus();
}
void MainWindow::on_runButton_2_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {   short header = COMMAND_MODE_CST;

        int16_t torque = ui->targetValue_2->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(torque);
        pTcpPacket->sendPacket();

        AppendText("[Torque] " + ui->targetValue_2->text());
    }
    ui->targetValue_2->setFocus();
}
void MainWindow::on_runButton_3_clicked()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {   short header = COMMAND_MODE_PPM;

        int32_t position = ui->targetValue_3->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(position);
        pTcpPacket->sendPacket();

        AppendText("[Position] " + ui->targetValue_3->text());
    }
    ui->targetValue_3->setFocus();
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

void MainWindow::on_cwButton_pressed()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = COMMAND_MODE_CSV;
        int32_t velocity = GEAR_RATIO * ui->velocity->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(velocity);
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
        int32_t velocity = GEAR_RATIO * ui->velocity->text().toInt() * (-1);

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(velocity);
        pTcpPacket->sendPacket();
    }
}
void MainWindow::on_cwButton_released()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        short header = COMMAND_MODE_STOP_MOTOR;

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->sendPacket();
    }
}
void MainWindow::on_ccwButton_released()
{
    on_cwButton_released();
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
                ui->actualVelocity->setText(QString::number(logData.velocity_actual_value / GEAR_RATIO));
                ui->actualTorque->setText(QString::number(logData.torque_actual_value));
                ui->actualPosition->setText(QString::number(logData.position_actual_value));
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

void MainWindow::on_runButton_CSP_clicked()
{

    if(!connectFlag)
        AppendText("Not connected");
    else
    {   short header = COMMAND_MODE_CSP;

        int32_t position = ui->targetValue_CSP->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(position);
        pTcpPacket->sendPacket();

        AppendText("[Position] " + ui->targetValue_CSP->text());
    }
    ui->targetValue_CSP->setFocus();

}

void MainWindow::on_pushButton_RunTask_SetTaskParam_clicked()
{
    // Task Parameter
    int iRunTaskType = ui->comboBox_RunTask_Type->currentIndex();
    int iRunTaskMotion = ui->comboBox_RunTask_Motion->currentIndex();

    QString sRunTaskDisp = ui->textEdit_RunTask_Displacement->toPlainText();
    QString sRunTaskPeriod = ui->textEdit_RunTask_Period->toPlainText();
    QString sRunTaskRepeat = ui->textEdit_RunTask_Repeat->toPlainText();

    float fRunTaskDisp = sRunTaskDisp.toFloat();
    float fRunTaskPeriod = sRunTaskPeriod.toFloat();
    int iRunTaskRepeat = sRunTaskRepeat.toInt();

    taskParam.disp = fRunTaskDisp;
    taskParam.period = fRunTaskPeriod;
    taskParam.repeat = iRunTaskRepeat;

    taskParam.taskType = iRunTaskType;
    taskParam.taskMotion = iRunTaskMotion;

//    taskParam.footSwitch = ui->checkBox_RunTask_Apply_Foot_Pedal->isChecked();
    taskParam.dataApply = ui->checkBox_RunTask_Apply_Data->isChecked();


    uint16_t header = COMMAND_CODE_SET_TASK_PARAM;

    pTcpPacket->setCommandHeader(header);
    pTcpPacket->encode(taskParam);
    pTcpPacket->sendPacket();

}

void MainWindow::on_pushButton_RunTask_RUN_clicked()
{
    uint16_t header = COMMAND_CODE_RUN_TASK;

    bool bStart = true;

    pTcpPacket->setCommandHeader(header);
    pTcpPacket->encode(bStart);
    pTcpPacket->sendPacket();
}

void MainWindow::on_pushButton_RunTask_STOP_clicked()
{
    uint16_t header = COMMAND_CODE_RUN_TASK;

    bool bStart = false;

    pTcpPacket->setCommandHeader(header);
    pTcpPacket->encode(bStart);
    pTcpPacket->sendPacket();
}
