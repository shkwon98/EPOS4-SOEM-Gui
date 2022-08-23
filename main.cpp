#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("./images/AnsurLab.png"));
    //    w.setFixedSize(530, 600);
    w.show();

    return a.exec();
}
