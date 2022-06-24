#ifndef MACRO_H
#define MACRO_H

#include <QString>


#define TCP_PORT 2000
#define UDP_PORT 3000
#define GUI_PC_IP "192.168.0.9"
#define RASPBERRY_PI_IP "192.168.0.10"

#define RX_BUFFER_SIZE 1024
#define TX_BUFFER_SIZE 1024
#define PACKET_BUFFER_SIZE 1024


typedef enum
{
    RUN_TASK_NONE,
    RUN_TASK_MOTOR_CONTROL
}RUN_TASK_TYPE;

typedef struct
{
    int   taskType;
    int   taskMotion;

    float disp;
    float period;
    int   repeat;

    // unsigned int numRun;

    // bool footSwitch;
    bool  dataApply;
}TASK_PARAM;

typedef struct
{
    int16_t    torque;
    int32_t    velocity;
    // int32_t   position;
    TASK_PARAM taskParam;
}INPUT_LIST;

typedef struct
{
    uint    timeStamp;

    int32_t actualVelocity;
    int16_t actualTorque;
    int32_t actualPosition;
}LOG_DATA;
#define LOG_DATA_SIZE sizeof(LOG_DATA)


// EPOS4 Gear Ratio
#define GEAR_RATIO  17

////////////////////////////////////////////////////
///////////////// GUI PC ----> RPI /////////////////
////////////////////////////////////////////////////

#define COMMAND_SET_TASK_PARAM             0X0000
#define COMMAND_RUN_CSP                    0X0001 // Cyclic Synchronous Position Mode

#define COMMAND_RUN_CSV                    0x0011 // Cyclic Synchronous Velocity Mode

#define COMMAND_RUN_CST                    0x0021 // Cyclic Synchronous Torque Mode

#define COMMAND_RUN_PPM                    0x0031 // Profile Position Mode

// #define COMMAND_MODE_STOP_MOTOR           0xFFFF



////////////////////////////////////////////////////
///////////////// RPI ----> GUI PC /////////////////
////////////////////////////////////////////////////

#define STREAM_MODE            0x0001
// #define


#endif // MACRO_H
