#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QSizePolicy>
#include <QtNetwork>
#include "tcpclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);

private slots:
    void on_sendBtn_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    bool fd_flag = false;
    bool send_flag = false;
    int cnt = 0;
    char buf[1024] = {0,};
};
#endif // MAINWINDOW_H
