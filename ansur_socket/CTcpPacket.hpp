#ifndef CTCPPACKET_HPP_
#define CTCPPACKET_HPP_

#include <QtNetwork>
#include <qdebug.h>
#include "socketDef.hpp"


class CTcpPacket
{
public:
    CTcpPacket();
    ~CTcpPacket();

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

#endif // CTCPPACKET_HPP_
