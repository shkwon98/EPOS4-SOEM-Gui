#ifndef MACRO_H
#define MACRO_H

#include <QString>


#define TCP_PORT 2000
#define UDP_PORT 3000

#define GUI_PC_IP "192.168.0.9"
#define RASPBERRY_PI_IP "192.168.0.10"

#define TX_BUFFER_SIZE 1024
#define RX_BUFFER_SIZE 1024
#define PACKET_BUFFER_SIZE 1024

typedef struct
{
    int amplitude;
    int frequency;
}SINUSOIDAL;

typedef struct {
    unsigned int timeStamp;

    int velocity_actual_value;
    int16_t torque_actual_value;

}LOG_DATA;

#endif // MACRO_H
