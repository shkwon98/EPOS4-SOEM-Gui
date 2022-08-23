#ifndef TCP_PACKET_H
#define TCP_PACKET_H

#include <QtNetwork>
#include <qdebug.h>
#include "socketDef.h"


class TCP_Packet
{
public:
    TCP_Packet();
    ~TCP_Packet();

    bool connect();

    void setCommandHeader(uint16_t header);

    template <class T>
    void encode(T &val)
    {
        if (bInitData == true)
        {
            memcpy(&txBuffer[6], &val, sizeof(val));
            encodeIndex = 6 + sizeof(val);

            bInitData = false;
        }
        else
        {
            memcpy(&txBuffer[encodeIndex], &val, sizeof(val));
            encodeIndex = encodeIndex + sizeof(val);
        }

        // Data Size
        uint16_t tempSize = sizeof(val);
        dataSize += tempSize;
    }

    void sendPacket();

private:
    QTcpSocket *tcpClient;

    int encodeIndex;
    unsigned char txBuffer[TX_BUFFER_SIZE];
    uint16_t header;

    uint16_t dataSize;
    bool bInitData;
};

#endif // TCP_PACKET_H
