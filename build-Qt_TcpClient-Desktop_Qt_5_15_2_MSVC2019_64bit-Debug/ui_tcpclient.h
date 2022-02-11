/********************************************************************************
** Form generated from reading UI file 'tcpclient.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENT_H
#define UI_TCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpClient
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLineEdit *messageLine;
    QPushButton *sendButton;
    QPushButton *eraseButton;
    QPushButton *exitButton;
    QTextBrowser *textBrowser;
    QPushButton *connectButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TcpClient)
    {
        if (TcpClient->objectName().isEmpty())
            TcpClient->setObjectName(QString::fromUtf8("TcpClient"));
        TcpClient->resize(450, 400);
        centralwidget = new QWidget(TcpClient);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 181, 21));
        messageLine = new QLineEdit(centralwidget);
        messageLine->setObjectName(QString::fromUtf8("messageLine"));
        messageLine->setGeometry(QRect(20, 275, 231, 25));
        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        sendButton->setGeometry(QRect(140, 310, 112, 34));
        eraseButton = new QPushButton(centralwidget);
        eraseButton->setObjectName(QString::fromUtf8("eraseButton"));
        eraseButton->setGeometry(QRect(20, 310, 112, 34));
        exitButton = new QPushButton(centralwidget);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(320, 310, 112, 34));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(20, 45, 411, 221));
        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(320, 10, 112, 34));
        TcpClient->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TcpClient);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 450, 30));
        TcpClient->setMenuBar(menubar);
        statusbar = new QStatusBar(TcpClient);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        TcpClient->setStatusBar(statusbar);

        retranslateUi(TcpClient);

        QMetaObject::connectSlotsByName(TcpClient);
    } // setupUi

    void retranslateUi(QMainWindow *TcpClient)
    {
        TcpClient->setWindowTitle(QCoreApplication::translate("TcpClient", "TcpClient", nullptr));
        label->setText(QCoreApplication::translate("TcpClient", "Raspberry Pi", nullptr));
        messageLine->setText(QCoreApplication::translate("TcpClient", "Message to send", nullptr));
        sendButton->setText(QCoreApplication::translate("TcpClient", "\354\240\204\354\206\241", nullptr));
        eraseButton->setText(QCoreApplication::translate("TcpClient", "\354\247\200\354\232\260\352\270\260", nullptr));
        exitButton->setText(QCoreApplication::translate("TcpClient", "\354\242\205\353\243\214", nullptr));
        connectButton->setText(QCoreApplication::translate("TcpClient", "\354\204\234\353\262\204 \354\227\260\352\262\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpClient: public Ui_TcpClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
