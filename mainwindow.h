#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

#include "Macro.h"

#include "cdatalogger.h"

#include "tcp_packet.h"
//#include "udp_packet.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void AppendText(QString str);

    template<class T>
    void decode(T &val)
    {
        memcpy(&val, &packetBuffer[decodeIndex], sizeof(val));
        decodeIndex = decodeIndex + sizeof(val);
    }

private slots:
    void on_exitButton_clicked();

    void on_udpButton_clicked(bool checked);
    void on_tcpConnectButton_clicked();

    void on_stopButton_clicked();
    void on_setZeroButton_clicked();

    void on_runButton_clicked();
    void on_runButton_2_clicked();
    void on_runButton_3_clicked();
    void on_generateButton_clicked();

    void on_cwButton_pressed();
    void on_ccwButton_pressed();
    void on_cwButton_released();
    void on_ccwButton_released();

    void readPacket();
    void on_pushButton_Data_Logging_clicked();


private:
    Ui::MainWindow *ui;

    TCP_Packet *pTcpPacket;
    bool connectFlag = false;

    //    UDP_Packet *pUdpPacket;
    QUdpSocket *UdpSocket;
    short header;
    int decodeIndex;
    unsigned char rxBuffer[RX_BUFFER_SIZE];
    unsigned char packetBuffer[PACKET_BUFFER_SIZE];
    int rxCount = 0;

    LOG_DATA logData;
    bool bRegLogging = false;
    CDataLogger mDataLogger;
    bool bLogCheck;
};
#endif // MAINWINDOW_H
