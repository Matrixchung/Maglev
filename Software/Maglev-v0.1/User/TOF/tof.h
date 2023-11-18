#ifndef _TOF_H
#define _TOF_H

#include "main.h"
#include "uart_handler.h"
#include "delay.h"

#define TOF050F_COMM_TIMEOUT       100000 // us

#define TOF050F_ADDRESS            0x01

#define TOF050F_REG_SPECIAL        0x0001
#define TOF050F_CMD_RESET_DEFAULT  0xAA55
#define TOF050F_CMD_REBOOT         0x1000

#define TOF050F_REG_DEV_ADDR       0x0002

#define TOF050F_REG_BAUDRATE       0x0003
#define TOF050F_CMD_BAUD_9600      0x0002
#define TOF050F_CMD_BAUD_38400     0x0001
#define TOF050F_CMD_BAUD_115200    0x0000

#define TOF050F_REG_SCALE          0x0004
#define TOF050F_CMD_SCALE_20CM     0x0001
#define TOF050F_CMD_SCALE_40CM     0x0002
#define TOF050F_CMD_SCALE_50CM     0x0003

#define TOF050F_REG_OUTPUT_CONTROL 0x0005

#define TOF050F_REG_LOAD_CALIBRATE 0x0006
#define TOF050F_CMD_LOAD_CLB       0x0001
#define TOF050F_CMD_UNLOAD_CLB     0x0000

#define TOF050F_REG_OFFSET         0x0007

#define TOF050F_REG_XTALK_OFFSET   0x0008

#define TOF050F_REG_I2C_ENABLE     0x0009
#define TOF050F_CMD_I2C_OFF        0x0001
#define TOF050F_CMD_I2C_ON         0x0000

#define TOF050F_REG_RESULT         0x0010

extern uint16_t tof_distance;
extern uint8_t tof_sensor_state;

void TOF_Sensor_Init(void);
void TOF_ReadRegister(uint16_t reg, uint16_t* dst, uint16_t* len);
void TOF_WriteRegister(uint16_t reg, uint16_t data);
void TOF_RX_Callback(void);

#endif