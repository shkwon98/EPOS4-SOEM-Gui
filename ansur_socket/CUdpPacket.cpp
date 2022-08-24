#include "CUdpPacket.hpp"

CUdpPacket::CUdpPacket(QObject *parent) : QObject(parent)
{
    UdpSocket = new QUdpSocket();

    QHostAddress Raspberry_Pi_Address;
    Raspberry_Pi_Address.setAddress(RASPBERRY_PI_IP);
    UdpSocket->bind(Raspberry_Pi_Address, UDP_PORT);
    //    connect(UdpSocket,SIGNAL(readyRead()),this,SLOT(readPacket()));
}
CUdpPacket::~CUdpPacket()
{
    if(UdpSocket->isOpen())
    {
        UdpSocket->disconnectFromHost();
        UdpSocket->close();
        UdpSocket->abort();
    }
}

void CUdpPacket::readPacket()
{
    QByteArray rxData;
    QHostAddress sender;
    uint16_t senderPort;

    qDebug() << "Hello";

    while(UdpSocket->hasPendingDatagrams())
    {
        rxData.fill(0, UdpSocket->pendingDatagramSize());
        UdpSocket->readDatagram(rxData.data(), rxData.size(), &sender, &senderPort);

        memcpy(rxBuffer, rxData.data(), rxData.size());

        for (int i = 0; i < RX_BUFFER_SIZE; i++)
        {
            if (rxBuffer[i] == 13 && rxBuffer[i + 1] == 10)
            {
                memcpy(packetBuffer, &rxBuffer[i + 2], sizeof(packetBuffer));
                break;
            }
        }

        header = 0;
        decodeIndex = 2;

        memcpy(&header, &packetBuffer[0], sizeof(short));

        switch(header)
        {
        case(1):
            int velocity;
            int16_t torque;

            this->decode(velocity);
            this->decode(torque);

            qDebug() << QString::number(velocity);
            break;
        }
    }
}
