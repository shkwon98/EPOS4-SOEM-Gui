#include "CTcpPacket.hpp"


CTcpPacket::CTcpPacket()
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
CTcpPacket::~CTcpPacket()
{
    if(tcpClient->isOpen())
    {
        tcpClient->disconnectFromHost();
        tcpClient->close();
        tcpClient->abort();
    }
}


bool CTcpPacket::connect()
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


void CTcpPacket::setCommandHeader(uint16_t header)
{
    // Initialize Variable
    bInitData = true;
    encodeIndex = 0;
    dataSize = 0;


    for (int i = 0; i < TX_BUFFER_SIZE; i++)
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
    this->header = header;
    memcpy(&txBuffer[4], &header, sizeof(header));
    encodeIndex += sizeof(header);

}


void CTcpPacket::sendPacket()
{
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    // Packet Header 1,
    // Packet Header 2,
    // Size,
    // Command Header,
    // Data
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    tcpClient->flush();

    // Data Size = Command Header(2 byte) + Data
    dataSize += 2;
    memcpy(&txBuffer[2], &dataSize, sizeof(dataSize));
    encodeIndex += sizeof(dataSize);

    if(tcpClient->isWritable())
        tcpClient->write((char*)txBuffer, encodeIndex);
}




