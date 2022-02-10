#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 정적배열
    qDebug() << "static Array :";
    int number[10];
    for (int i = 0; i < 10; i++)
    {
        number[i] = i;
        qDebug() << "i = " << number[i];
    }

    // 동적배열
    qDebug() << "dynamic Array :";
    int* num = new int[10];
    for (int i = 0; i < 10; i++)
    {
        num[i] = i;
        qDebug() << "i = " << num[i];
    }
    delete[] num;
}

MainWindow::~MainWindow()
{
    delete ui;
}


//입력
void MainWindow::on_pushButton_clicked()
{
    ui->lineEdit->setText("hello");
}

//지우기
void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->setText("");
}

//종료
void MainWindow::on_pushButton_3_clicked()
{
    this->close();
}
