#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtNetwork>

#include "ctcp_packet.h"


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

private slots:
    void on_connectButton_clicked();
    void on_exitButton_clicked();

    void on_stopButton_clicked();

    void on_targetMode_currentIndexChanged(int index);
    void on_activateButton_clicked();

    void on_dial_valueChanged(int value);
    void on_cwButton_pressed();
    void on_ccwButton_pressed();
    void on_cwButton_released();
    void on_ccwButton_released();

    void on_generateButton_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket *tcpClient;

    CTCP_Packet *pTcpPacket;

    bool connectFlag = false;
    uint16_t header;
};
#endif // MAINWINDOW_H
