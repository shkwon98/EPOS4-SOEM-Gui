#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMainWindow>
#include <QtDebug>
#include <QTcpSocket>

#pragma pack(push, 1)
struct packetST
{
    int length;
    char str[512];
};
#pragma pack(pop)


QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QMainWindow
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();

public slots:
    void doConnect();
    void connected();
    void disconnected();

private slots:
    void on_connectButton_clicked();
    void on_messageLine_returnPressed();
    void on_sendButton_clicked();
    void on_eraseButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::TcpClient *ui;

    QTcpSocket *socket;
    bool connectFlag = false;
    packetST msg;
};
#endif // TCPCLIENT_H
