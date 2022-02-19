#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "Macro.h"
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
    void readPacket();

    void on_tcpConnectButton_clicked();
    void on_exitButton_clicked();

    void on_stopButton_clicked();

    void on_targetMode_currentIndexChanged(int index);
    void on_activateButton_clicked();

    void on_cwButton_pressed();
    void on_ccwButton_pressed();
    void on_cwButton_released();
    void on_ccwButton_released();

    void on_generateButton_clicked();


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
};
#endif // MAINWINDOW_H
