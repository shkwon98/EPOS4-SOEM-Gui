#ifndef TCP_PACKET_H
#define TCP_PACKET_H

#include <QtNetwork>
#include <qdebug.h>
#include "Macro.h"


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
        memcpy(&txBuffer[encodeIndex], &val, sizeof(val));
        encodeIndex = encodeIndex + sizeof(val);
    }

    void sendPacket();

private:
    QTcpSocket *tcpClient;

    int encodeIndex;
    unsigned char txBuffer[TX_BUFFER_SIZE];
};

#endif // TCP_PACKET_H
