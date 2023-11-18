/*********************************************************************************
	*Copyright(C),20xx-20xx,

	*Author:
			wwyyy
			q1046685883

	*Version:
			V1.0

	*Data:
			2019/11/16

	*Description:


	*Other:


	*Function List:
		1.
		2.
		3.
		4.
	*History modify:
		1.Data:
		Author:
		Log:

**********************************************************************************/
#ifndef __TYPE_H_
#define __TYPE_H_

//定义类型
typedef unsigned char		bool;
typedef signed char			int8;
typedef unsigned char		uint8;
typedef signed short		int16;
typedef unsigned short		uint16;
typedef signed long			int32;
typedef unsigned long		uint32;
typedef unsigned long long	uint64;
typedef signed long long	int64;

#ifndef TRUE	//true/false
#define TRUE	1
#endif
#ifndef FALS
#define FALS	0
#endif

#ifndef E_OK	//ok/error
#define E_OK	0
#endif
#ifndef E_ER
#define E_ER	1
#endif

#ifndef HIH		//high/low
#define HIH		1
#endif
#ifndef LOW
#define LOW		0
#endif

#ifndef FLIP	//flip/noflip
#define FLIP	1
#endif
#ifndef NFLP
#define NFLP	0
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef CAN_IDE_STANDARD		//标准帧/扩展帧
#define CAN_IDE_STANDARD		0
#endif
#ifndef CAN_IDE_EXTENDED
#define CAN_IDE_EXTENDED		1
#endif

#ifndef CAN_FRAM_RTR_DATA		//数据帧/远程帧
#define CAN_FRAM_RTR_DATA		0
#endif
#ifndef CAN_FRAM_RTR_REMO
#define CAN_FRAM_RTR_REMO		1
#endif

typedef struct
{
	uint32 id;
	uint8 ide;
	uint8 rtr;
	uint8 dlc;
	uint8 a_data[8];
}CAN_MSG;

typedef struct
{
	uint32		freq;//0.1hz
	uint16		duty;//0.1%
}PWM_STATUS;

#endif//__TYPE_H_

