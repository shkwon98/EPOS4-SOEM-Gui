#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setFixedSize(510, 380);

    pTcpPacket = new CTCP_Packet();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AppendText(QString str)
{
    QString appendStr;

    appendStr.append(str);

    ui->textBrowser->append(appendStr);

//    static int lineCnt = 0;
//    lineCnt++;

//    if(lineCnt > 50)
//    {
//        ui->textBrowser->clear();
//        lineCnt = 0;
//    }
}


void MainWindow::on_connectButton_clicked()
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
        header = 0000;
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
    {
        if(ui->targetMode->currentText() == "Velocity")
            header = 0001;
        else if(ui->targetMode->currentText() == "Torque")
            header = 0002;
        else if(ui->targetMode->currentText() == "B&F")
            header = 0003;

        int iData = ui->targetValue->text().toInt();

        pTcpPacket->setCommandHeader(header);
        pTcpPacket->encode(iData);
        pTcpPacket->sendPacket();

        AppendText("[" + ui->targetMode->currentText()+ "] " + ui->targetValue->text());
    }
    ui->targetValue->setFocus();
}

void MainWindow::on_dial_valueChanged(int value)
{
    ui->velocity->setText(QString::number(value));
}

void MainWindow::on_cwButton_pressed()
{
    if(!connectFlag)
        AppendText("Not connected");
    else
    {
        header = 0001;
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
        header = 0001;
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
        header = 0004;

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

