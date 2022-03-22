#ifndef UDP_PACKET_H
#define UDP_PACKET_H

#include <QObject>
#include <QtNetwork>
#include <qdebug.h>
#include "Macro.h"

class UDP_Packet: public QObject
{
    Q_OBJECT

public:
    UDP_Packet(QObject *parent = nullptr);
    ~UDP_Packet();

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

#endif // UDP_PACKET_H
