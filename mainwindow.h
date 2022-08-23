#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

#include "socketDef.h"
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

    void on_pushButton_SetTaskParam_clicked();
    void on_pushButton_CSP_Run_clicked();
    void on_pushButton_CSP_Stop_clicked();

    void on_pushButton_CSV_Run_clicked();
    void on_pushButton_CSV_Stop_clicked();

    void on_pushButton_CST_Run_clicked();
    void on_pushButton_CST_Stop_clicked();

    void readPacket();


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
    TASK_PARAM taskParam;
};

#endif // MAINWINDOW_H
