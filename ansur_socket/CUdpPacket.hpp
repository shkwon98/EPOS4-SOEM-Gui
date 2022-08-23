#ifndef CUDPPACKET_HPP_
#define CUDPPACKET_HPP_

#include <QObject>
#include <QtNetwork>
#include <qdebug.h>
#include "socketDef.hpp"

class CUdpPacket: public QObject
{
    Q_OBJECT

public:
    CUdpPacket(QObject *parent = nullptr);
    ~CUdpPacket();

    short getHeader()
    {
        return header;
    }

    template<class T>
    void decode(T &val)
    {
        memcpy(&val, &packetBuffer[decodeIndex], sizeof(val));
        decodeIndex = decodeIndex + sizeof(val);
    }

    void readPacket();

private:
    QUdpSocket *UdpSocket;
    short header;
    int decodeIndex;
    unsigned char rxBuffer[RX_BUFFER_SIZE];
    unsigned char packetBuffer[PACKET_BUFFER_SIZE];
};

#endif // CUDPPACKET_HPP_
