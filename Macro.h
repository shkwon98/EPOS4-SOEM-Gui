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

    int32_t velocity_actual_value;
    int16_t torque_actual_value;
    int32_t position_actual_value;

}LOG_DATA;


typedef struct {

    float disp;
    float period;
    int repeat;

    int taskType;
    int taskMotion;

    unsigned int numRun;

    bool footSwitch;
    bool dataApply;

}TASK_PARAM;


typedef enum {
    RUN_TASK_NONE,
    RUN_TASK_MOTOR_CONTROL
}RUN_TASK_TYPE;

#define LOG_DATA_SIZE sizeof(LOG_DATA)

// EPOS4 Gear Ratio
#define GEAR_RATIO  17

////////////////////////////////////////////////////
///////////////// GUI PC ----> RPI /////////////////
////////////////////////////////////////////////////

#define COMMAND_MODE_STOP_MOTOR                 0x0000
#define COMMAND_MODE_CSV                        0x0001 // Cyclic Synchronous Velocity Mode
#define COMMAND_MODE_CST                        0x0002 // Cyclic Synchronous Torque Mode
#define COMMAND_MODE_PPM                        0x0003 // Profile Position Mode
#define COMMAND_MODE_SINUSOIDAL_VELOCITY        0x0004
#define COMMAND_MODE_BACK_AND_FORTH_VELOCITY    0x0005
#define COMMAND_MODE_SET_ZERO                   0x0006
#define COMMAND_MODE_CSP                        0x0007


#define COMMAND_CODE_SET_TASK_PARAM             0X00F1
#define COMMAND_CODE_RUN_TASK                   0X00F2

////////////////////////////////////////////////////
///////////////// RPI ----> GUI PC /////////////////
////////////////////////////////////////////////////

 #define STREAM_MODE            0x0001
// #define


#endif // MACRO_H
