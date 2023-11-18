#ifndef __MCU_HAL_USER_H_
#define __MCU_HAL_USER_H_
#include "mcu_hal.h"


typedef enum
{ 
	MCU_IOIN_ONE = 0,
	MCU_IOIN_TWO,
	MCU_IOIN_THREE,
	MCU_IOIN_NUM,
}MCU_IOIN_ID;
void mcu_ioin_user_init(void);





typedef enum
{
	MCU_IOOUT_ONE = 0,
	MCU_IOOUT_TWO,
	MCU_IOOUT_NUM
}MCU_IOOUT_ID;
void mcu_ioout_user_init(void);





typedef enum
{
	MCU_DIO_ONE = 0,
	MCU_DIO_TWO,
	MCU_DIO_THREE,
	MCU_DIO_NUM
}MCU_DIO_ID;
void mcu_dio_user_init(void);





typedef enum
{
	MCU_ADIN_ONE = 0,
	MCU_ADIN_TWO,
	MCU_ADIN_THREE,
	MCU_ADIN_NUM
}MCU_ADIN_ID;
void mcu_adc_user_init(void);





typedef enum
{
	MCU_PWMO_ONE = 0,
	MCU_PWMO_NUM
}MCU_PWMO_ID;
void mcu_pwmo_user_init(void);





typedef enum
{
	MCU_PWMI_ONE = 0,
	MCU_PWMI_NUM
}MCU_PWMI_ID;
void mcu_pwmi_user_init(void);





typedef enum
{
	MCU_CAN_ONE = 0,
	//MCU_CAN_TWO,
	MCU_CAN_NUM
}MCU_CAN_ID;
void mcu_can_user_init(void);





typedef enum
{
	MCU_MSPI_ONE = 0,
	MCU_MSPI_TWO,
	MCU_MSPI_NUM
}MCU_MSPI_ID;
void mcu_mspi_user_init(void);





typedef enum
{
	MCU_SSPI_ONE = 0,
	MCU_SSPI_NUM = 0,
}MCU_SSPI_ID;
void mcu_sspi_user_init(void);





typedef enum
{
	MCU_UART_ONE = 0,
	MCU_UART_TWO,
	MCU_UART_NUM
}MCU_UART_ID;
void mcu_uart_user_init(void);





void mcu_user_init(void);
void mcu_user_main(void);







#endif //__MCU_HAL_USER_H_
