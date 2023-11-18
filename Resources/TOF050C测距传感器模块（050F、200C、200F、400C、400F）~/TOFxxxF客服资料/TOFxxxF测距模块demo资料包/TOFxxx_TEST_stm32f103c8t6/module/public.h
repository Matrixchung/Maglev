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
#ifndef __PUBLIC_H_
#define __PUBLIC_H_
#include "type.h"


#define MATH_MIN(A,B)                   (((A) > (B)) ? (B) : (A))
#define MATH_MAX(A,B)                   (((A) < (B)) ? (B) : (A))
#define MATH_ABS(A,B)                   (((A) > (B)) ? ((A) - (B)) : ((B) - (A)))
#define MATH_ABS_VAR(A)                 (((A) > 0)   ? (A) : (-(A)))
#define MATH_DIV_ROUND(A,B)             (((A)>=0) ? ((((A)+((B)/2))/(B))) : (((A)-((B)/2))/(B)))		// 四舍五入整除算法，除数必须大于0

//get memory address bit n
#define GMA_BYTE_BIT(Addr, Bit)		((((uint8 *)(Addr))[(Bit) / 8] >> ((Bit) % 8)) & 0x01)
//set memory address bit n
#define SMA_BYTE_BIT(Addr, Bit)		(((uint8 *)(Addr))[(Bit) / 8] |= (0x01 << ((Bit) % 8)))
//clear memory address bit n
#define CMA_BYTE_BIT(Addr, Bit)		(((uint8 *)(Addr))[(Bit) / 8] &= ((uint8)(~(0x01 << ((Bit) % 8)))))



uint8 memchk(void *p_src, uint16 len, uint8 str);
uint8 ascii_to_int(uint8 c);						//通用化
void hex_to_ascii(uint8 num, uint8 *p_data);		//通用化
void proces_hex_to_ascii(uint8 *p_data, uint16 len, uint8 *p_ascii);//通用化
uint8 ascii_to_hex(uint8 *p_data);					//通用化
void proces_ascii_to_hex(uint8 *p_data, uint16 len);//通用化
uint8 calc_sum_ascii(uint8 *p_data, uint16 len);	//通用化
uint8 calc_sum_hex(uint8 *p_data, uint16 len);		//通用化
//uint8 check_sum_ascii(uint8 *p_data, uint16 len);
//uint8 check_sum_num(uint8 *p_data, uint16 len);
uint8 hex_check_sum_ascii(uint8 *p_data, uint16 len);
uint8 mot_check_sum_ascii(uint8 *p_data, uint16 len);
uint8 check_xor(uint8 *p_data, uint16 len);
uint16 assmbleInt2ByteS(void *p_data);
void disassmbleInt2ByteS(uint16 data, void *p_data);
uint16 assmbleInt2ByteB(void *p_data);
void disassmbleInt2ByteB(uint16 data, void *p_data);
uint32 assmbleInt4ByteS(void *p_data);
void disassmbleInt4ByteS(uint32 data, void *p_data);
uint32 assmbleInt4ByteB(void *p_data);
void disassmbleInt4ByteB(uint32 data, void *p_data);
float assmbleFloat4ByteS(void *p_data);
void disassmbleFloat4ByteS(float data, void *p_data);
float assmbleFloat4ByteB(void *p_data);
void disassmbleFloat4ByteB(float data, void *p_data);
double assmbleDouble8ByteS(void *p_data);
void disassmbleDouble8ByteS(double data, void *p_data);
double assmbleDouble8ByteB(void *p_data);
void disassmbleDouble8ByteB(double data, void *p_data);
void crc32_reinit(void);
void crc32_calculate(uint8 *p_buff, uint32 len);
uint32 crc32_get_crc(void);
uint32 crc32_calculate1(uint8 *p_buff, uint32 len);
uint16 crc16_calculate_modbus(uint8 *p_data, uint16 len);
void crc16_reinit(void);
void crc16_calculate(uint8 *p_buff, uint32 len);
uint16 crc16_get_crc(void);
uint16 crc16_calculate1(uint8 *p_buff, uint32 len);
uint8 check_timeout(uint32 start, uint32 now, uint32 invt);
uint32 calcu_interval(uint32 start, uint32 now);
void updata_roll_count8(uint8 *p_count);
void updata_roll_count16(uint16 *p_count);
void updata_roll_count32(uint32 *p_count);
uint8 data_dec_bcd(uint8 dec);
uint8 bit_lsb_num(uint32 data);
uint8 lookup_table_index_uint32(const uint32 arr[], uint16 arrLen, uint32 target, uint16 *pLowIndex, uint16 *pHighIndex);
uint32 calc_resistor_up(uint32 ResUp, uint32 ResDown, uint16 vt, uint16 Vref);
uint32 calc_resistor_down(uint32 ResUp, uint32 ResDown, uint16 vt, uint16 Vref);
uint32 calc_resistor_para(uint32 ParRes, uint32 SrcRes);
uint8 bcd_to_dec(uint8 bcd);
uint8 dec_to_bcd(uint8 dec);
typedef struct
{
	uint8 second;	//0~59:0~59
	uint8 minute;	//0~59:0~59
	uint8 hour;		//0~23:0~23
	uint8 day;		//1~31:1~31
	uint8 week;		//0~6:周日~周六
	uint8 month;	//0~11:1~12
	uint16 year;	//1970~
}LOCAL_TIME;
void unixtime_to_localtime(uint32 unixTime, LOCAL_TIME *p_localTime);
void localtime_to_unixtime(LOCAL_TIME *p_localTime, uint32 *p_unixTime);
void sort_aescend_uint32(uint32 *p_data, uint16 dataNum);
#endif//__PUBLIC_H_
