#include "mainwindow.h"
#include "tcpclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TcpClient c;

    c.PrintThis();
    qDebug() << "my address: " << &c << endl;

    return a.exec();
}
