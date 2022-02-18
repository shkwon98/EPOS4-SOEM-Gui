#ifndef CTCP_PACKET_H
#define CTCP_PACKET_H

#include <QtNetwork>
#include <QByteArray>

#include <vector>

#include <qdebug.h>


#define RASPBERRY_PI_TCPIP_PORT 2000

#define GUI_PC_IP "192.168.0.9"
#define RASPBERRY_PI_IP "192.168.0.10"

#define RX_BUFFER_SIZE 1024
#define TX_BUFFER_SIZE 1024

typedef struct
{
    int amplitude;
    int frequency;
}SINUSOIDAL;

class CTCP_Packet
{
public:
    CTCP_Packet();
    ~CTCP_Packet();

    bool connect();

    void setCommandHeader(uint16_t header);

    void encode(std::vector<char> &vec);
    void encode(std::vector<uchar> &vec);
    void encode(std::vector<char16_t> &vec);
    void encode(std::vector<bool> &vec);
    void encode(std::vector<int8_t> &vec);
    void encode(std::vector<int16_t> &vec);
    void encode(std::vector<uint16_t> &vec);
    void encode(std::vector<int> &vec);
    void encode(std::vector<uint32_t> &vec);
    void encode(std::vector<int64_t> &vec);
    void encode(std::vector<uint64_t> &vec);
    void encode(std::vector<float> &vec);
    void encode(std::vector<double> &vec);

    template <class T>
    void encode(T &val)
    {
        memcpy(&txBuffer[encodeIndex], &val, sizeof(val));
        encodeIndex = encodeIndex + sizeof(val);
    }

    template <class T>
    void encode_size(T &val, int size)
    {
        memcpy(&txBuffer[encodeIndex], &val, size);
        encodeIndex = encodeIndex + size;
    }

    void sendPacket();

private:
    QTcpSocket *tcpClient;
    unsigned char txBuffer[TX_BUFFER_SIZE];
    int encodeIndex;
    uint16_t header;
};

#endif // CTCP_PACKET_H
