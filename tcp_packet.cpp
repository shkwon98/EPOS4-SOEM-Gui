#include "tcp_packet.h"


TCP_Packet::TCP_Packet()
{
    tcpClient = new QTcpSocket();

    if(tcpClient->isOpen())
    {
        tcpClient->flush();
        tcpClient->reset();
        tcpClient->disconnectFromHost();
        tcpClient->close();
    }
    tcpClient->reset();
}
TCP_Packet::~TCP_Packet()
{
    if(tcpClient->isOpen())
    {
        tcpClient->disconnectFromHost();
        tcpClient->close();
        tcpClient->abort();
    }
}


bool TCP_Packet::connect()
{
    if(tcpClient->isOpen())
    {
        tcpClient->flush();
        tcpClient->reset();
        tcpClient->disconnectFromHost();
        tcpClient->close();
    }

    tcpClient->flush();
    tcpClient->connectToHost(RASPBERRY_PI_IP, TCP_PORT);

    if(tcpClient->waitForConnected(10000))
    {
        qDebug() << "Connected!";
        return true;
    }
    else
    {
        qDebug() << "Not Connected!";
        return false;
    }

}


void TCP_Packet::setCommandHeader(uint16_t header)
{
    // Initialize Variable
    encodeIndex = 0;

    for(int i=0; i<TX_BUFFER_SIZE; i++)
    {
        txBuffer[i] = 0;
    }

    // Packet Header 1
    txBuffer[encodeIndex] = 13;
    encodeIndex++;

    // Packet Header 2
    txBuffer[encodeIndex] = 10;
    encodeIndex++;

    // Command Header
    memcpy(&txBuffer[encodeIndex], &header, sizeof(header));
    encodeIndex = encodeIndex + sizeof(header);
}


void TCP_Packet::sendPacket()
{
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    // Packet Header 1,
    // Packet Header 2,
    // Command Header,
    // Data
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    tcpClient->flush();

    if(tcpClient->isWritable())
        tcpClient->write((char*)txBuffer, encodeIndex);
}




