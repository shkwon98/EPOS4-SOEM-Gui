#include "ctcp_packet.h"


CTCP_Packet::CTCP_Packet()
{
//    QHostAddress GUI_PC_Address;
//    GUI_PC_Address.setAddress(GUI_PC_IP);

    tcpClient = new QTcpSocket();

    if(tcpClient->isOpen())
    {
        tcpClient->flush();
        tcpClient->reset();
        tcpClient->disconnectFromHost();
        tcpClient->close();
    }
    tcpClient->reset();


//    tcpClient->bind(GUI_PC_Address, RASPBERRY_PI_TCPIP_PORT);


    // Initialize Variables
    encodeIndex = 0;

    for(int i=0; i<TX_BUFFER_SIZE; i++)
    {
        txBuffer[i] = 0;
    }
}
CTCP_Packet::~CTCP_Packet()
{
    if(tcpClient->isOpen())
    {
        tcpClient->disconnectFromHost();
        tcpClient->close();
        tcpClient->abort();
    }
}


bool CTCP_Packet::connect()
{
    if(tcpClient->isOpen())
    {
        tcpClient->flush();
        tcpClient->reset();
        tcpClient->disconnectFromHost();
        tcpClient->close();
    }

    tcpClient->flush();
    tcpClient->connectToHost(RASPBERRY_PI_IP, RASPBERRY_PI_TCPIP_PORT);

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


void CTCP_Packet::setCommandHeader(uint16_t header)
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


void CTCP_Packet::encode(std::vector<float> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();
}
void CTCP_Packet::encode(std::vector<double> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();
}
void CTCP_Packet::encode(std::vector<char> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();
}
void CTCP_Packet::encode(std::vector<uchar> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();
}
void CTCP_Packet::encode(std::vector<char16_t> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();
}
void CTCP_Packet::encode(std::vector<bool> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();
}
void CTCP_Packet::encode(std::vector<int8_t> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }
    vec.clear();
}
void CTCP_Packet::encode(std::vector<int16_t> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();

}
void CTCP_Packet::encode(std::vector<uint16_t> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();

}
void CTCP_Packet::encode(std::vector<int> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();

}
void CTCP_Packet::encode(std::vector<uint32_t> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();

}
void CTCP_Packet::encode(std::vector<int64_t> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();

}
void CTCP_Packet::encode(std::vector<uint64_t> &vec)
{
    uint16_t DataAmount = vec.size();

    if(DataAmount <= 0)
    {
        encodeIndex = 0;
        return;
    }

    for(int i=0; i<DataAmount; i++)
    {
        memcpy(&txBuffer[encodeIndex], &vec.at(i), sizeof(vec.at(i)));
        encodeIndex = encodeIndex + sizeof(vec.at(i));
    }

    vec.clear();

}


void CTCP_Packet::sendPacket()
{
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    // Packet Header 1,
    // Packet Header 2,
    // Command Header,
    // Data Amount,
    // Data Type,
    // Data
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    tcpClient->flush();

    if(tcpClient->isWritable())
        tcpClient->write((char*)txBuffer, encodeIndex);
}




