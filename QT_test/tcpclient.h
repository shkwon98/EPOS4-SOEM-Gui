#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMainWindow>
#include <QtCore>#include <QtNetwork>

namespace Ui {
class TcpClient;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);

private slots:
    void on_pushButton_clicked();

private:
    Ui::TcpClient *ui;
    QTcpSocket *socket;
    bool fd_flag = false;
    bool send_flag = false;
    int cnt = 0;
};

#endif // TCPCLIENT_H
