/*********************************************************************************
	*Copyright(C),20xx-20xx,

	*Filename:public.c

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
#include "public.h"


/***********************************************************************************************
*函数名: uint8 memchk(void *p_src, uint16 len, uint8 str)
*功能描述:判断内存是否为特定值,0:一致,
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 memchk(void *p_src, uint16 len, uint8 str)
{
	uint16 i = 0;
	uint8 *p_data = (uint8 *)p_src;
	for(i = 0; i < len; i++)
	{
		if(p_data[i] != str)
		{
			return 0;
		}
	}
	return 1;
}

/***********************************************************************************************
*函数名: uint8 ascii_to_int(uint8 c)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 ascii_to_int(uint8 c)
{
	if(c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if(c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	else if(c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else
	{
		return 0;
	}
}

/***********************************************************************************************
*函数名: void hex_to_ascii(uint8 num, uint8 *p_data)
*功能描述: 传入一个字节数值转换成两个字节ascii
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void hex_to_ascii(uint8 num, uint8 *p_data)
{
	p_data[0] = (num >> 4 & 0x0F) > 9 ? (num >> 4 & 0x0F) - 10 + 'A' : (num >> 4 & 0x0F) + '0';
	p_data[1] = (num & 0x0F) > 9 ? (num & 0x0F) - 10 + 'A' : (num & 0x0F) + '0';
}

/***********************************************************************************************
*函数名: void proces_hex_to_ascii(uint8 *p_data, uint16 len, uint8 *p_ascii)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void proces_hex_to_ascii(uint8 *p_data, uint16 len, uint8 *p_ascii)
{
	uint16 i = 0, j = 0;
	for(i = 0, j = 0; i < len; i++, j += 2)
	{
		hex_to_ascii(p_data[i], &p_ascii[j]);
	}
}

/***********************************************************************************************
*函数名: uint8 ascii_to_hex(uint8 *p_data)
*功能描述: 传入两个字节ascii转成一个字节数值
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 ascii_to_hex(uint8 *p_data)
{
	uint8 num = 0;
	num = ascii_to_int(p_data[0]);
	num <<= 4;
	num |= ascii_to_int(p_data[1]);
	return num;
}

/***********************************************************************************************
*函数名: void proces_ascii_to_hex(uint8 *p_data, uint16 len)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void proces_ascii_to_hex(uint8 *p_data, uint16 len)
{
	uint16 i = 0, j = 0;
	for(i = 0, j = 0; i < len; i++, j += 2)
	{
		p_data[i] = ascii_to_hex(&p_data[j]);
	}
}

/***********************************************************************************************
*函数名: uint8 calc_sum_ascii(uint8 *p_data, uint16 len)
*功能描述: 计算一段ascii数字的sum
*特别说明:
*函数参数: 
			len :数值长度,为ascii长度2倍
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 calc_sum_ascii(uint8 *p_data, uint16 len)
{
	uint8 sum = 0;
	while(len--)
	{
		sum += ascii_to_hex(p_data);
		p_data += 2;
	}
	return sum;
}

/***********************************************************************************************
*函数名: uint8 calc_sum_hex(uint8 *p_data, uint16 len)
*功能描述: 计算一段数值的sum
*特别说明:
*函数参数: 
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 calc_sum_hex(uint8 *p_data, uint16 len)
{
	uint8 sum = 0;
	while(len--)
	{
		sum += *p_data++;
	}
	return sum;
}

///***********************************************************************************************
//*函数名: uint8 check_sum_ascii(uint8 *p_data, uint16 len)
//*功能描述:
//*特别说明:
//*函数参数:
//*函数返回值:
//*修改记录:
//***********************************************************************************************/
//uint8 check_sum_ascii(uint8 *p_data, uint16 len)
//{
//	return 0x100 - calc_sum_ascii(p_data, len);
//}

///***********************************************************************************************
//*函数名: uint8 check_sum_hex(uint8 *p_data, uint16 len)
//*功能描述:
//*特别说明:
//*函数参数:
//*函数返回值:
//*修改记录:
//***********************************************************************************************/
//uint8 check_sum_hex(uint8 *p_data, uint16 len)
//{
//	return 0x100 - calc_sum_hex(p_data, len);
//}


/***********************************************************************************************
*函数名: uint8 hex_check_sum_ascii(uint8 *p_data, uint16 len)
*功能描述:
*特别说明:升级程序hex文件专用校验
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 hex_check_sum_ascii(uint8 *p_data, uint16 len)
{
	uint16 i = 0;
	uint8 tmp = 0, sum = 0;
	if(len%2)
	{
		return 1;
	}
	for(i = 0; i < len; i += 2)
	{
		tmp = (ascii_to_int(p_data[i]) << 4) | ascii_to_int(p_data[i + 1]);
		sum+=tmp;
	}
	if(sum != 0x00)
	{
		return 1;
	}
	return 0;
}

/***********************************************************************************************
*函数名: uint8 mot_check_sum_ascii(uint8 *p_data, uint16 len)
*功能描述:
*特别说明:升级程序摩托罗拉文件专用校验
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mot_check_sum_ascii(uint8 *p_data, uint16 len)
{
	uint16 i = 0;
	uint8 tmp = 0, sum = 0;
	if(len%2)
	{
		return 1;
	}
	for(i = 0; i < len; i += 2)
	{
		tmp = (ascii_to_int(p_data[i]) << 4) | ascii_to_int(p_data[i + 1]);
		sum+=tmp;
	}
	if(sum != 0xFF)
	{
		return 1;
	}
	return 0;
}

/***********************************************************************************************
*函数名: uint8 check_xor(uint8 *p_data, uint16 len)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 check_xor(uint8 *p_data, uint16 len)
{
	uint16 i = 0;
	uint8 tmp = 0;
	for(i = 0; i < len; i++)
	{
		tmp ^= p_data[i];
	}
	return tmp;
}

/***********************************************************************************************
*函数名: uint16 assmbleInt2ByteS(void *p_data)
*功能描述: 把内存中字节按小段方式组合成16位数据,即小地址低字节
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint16 assmbleInt2ByteS(void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	uint16 temp = 0;
	temp = p[1];
	temp <<= 8;
	temp |= p[0];
	return temp;
}

/***********************************************************************************************
*函数名: void disassmbleInt2ByteS(uint16 data, void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void disassmbleInt2ByteS(uint16 data, void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	if(NULL != p_data)
	{
		p[0] = (uint8)data;
		data >>= 8;
		p[1] = (uint8)data;
	}
}

/***********************************************************************************************
*函数名: uint16 assmbleInt2ByteB(void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint16 assmbleInt2ByteB(void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	uint16 temp = 0;
	temp = p[0];
	temp <<= 8;
	temp |= p[1];
	return temp;
}

/***********************************************************************************************
*函数名: void disassmbleInt2ByteB(uint16 data, void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void disassmbleInt2ByteB(uint16 data, void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	if(NULL != p_data)
	{
		p[1] = (uint8)data;
		data >>= 8;
		p[0] = (uint8)data;
	}
}

/***********************************************************************************************
*函数名: uint32 assmbleInt4ByteS(void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint32 assmbleInt4ByteS(void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	uint32 temp = 0;
	temp = p[3];
	temp <<= 8;
	temp |= p[2];
	temp <<= 8;
	temp |= p[1];
	temp <<= 8;
	temp |= p[0];
	return temp;
}

/***********************************************************************************************
*函数名: void disassmbleInt4ByteS(uint32 data, void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void disassmbleInt4ByteS(uint32 data, void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	if(NULL != p_data)
	{
		p[0] = (uint8)data;
		data >>= 8;
		p[1] = (uint8)data;
		data >>= 8;
		p[2] = (uint8)data;
		data >>= 8;
		p[3] = (uint8)data;
	}
}

/***********************************************************************************************
*函数名: uint32 assmbleInt4ByteB(void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint32 assmbleInt4ByteB(void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	uint32 temp = 0;
	temp = p[0];
	temp <<= 8;
	temp |= p[1];
	temp <<= 8;
	temp |= p[2];
	temp <<= 8;
	temp |= p[3];
	return temp;
}

/***********************************************************************************************
*函数名: void disassmbleInt4ByteB(uint32 data, void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void disassmbleInt4ByteB(uint32 data, void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	if(NULL != p_data)
	{
		p[3] = (uint8)data;
		data >>= 8;
		p[2] = (uint8)data;
		data >>= 8;
		p[1] = (uint8)data;
		data >>= 8;
		p[0] = (uint8)data;
	}
}

/***********************************************************************************************
*函数名: float assmbleFloat4ByteS(void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
float assmbleFloat4ByteS(void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	float temp = 0;
	*(((uint8 *)(&temp))+0) = p[0];
	*(((uint8 *)(&temp))+1) = p[1];
	*(((uint8 *)(&temp))+2) = p[2];
	*(((uint8 *)(&temp))+3) = p[3];
	return temp;
}

/***********************************************************************************************
*函数名: void disassmbleFloat4ByteS(float data, void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void disassmbleFloat4ByteS(float data, void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	p[0] = *(((uint8 *)(&data))+0);
	p[1] = *(((uint8 *)(&data))+1);
	p[2] = *(((uint8 *)(&data))+2);
	p[3] = *(((uint8 *)(&data))+3);
}

/***********************************************************************************************
*函数名: float assmbleFloat4ByteB(void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
float assmbleFloat4ByteB(void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	float temp = 0;
	*(((uint8 *)(&temp))+0) = p[3];
	*(((uint8 *)(&temp))+1) = p[2];
	*(((uint8 *)(&temp))+2) = p[1];
	*(((uint8 *)(&temp))+3) = p[0];
	return temp;
}

/***********************************************************************************************
*函数名: void disassmbleFloat4ByteB(float data, void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void disassmbleFloat4ByteB(float data, void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	p[0] = *(((uint8 *)(&data))+3);
	p[1] = *(((uint8 *)(&data))+2);
	p[2] = *(((uint8 *)(&data))+1);
	p[3] = *(((uint8 *)(&data))+0);
}

/***********************************************************************************************
*函数名: double assmbleDouble8ByteS(void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
double assmbleDouble8ByteS(void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	double temp = 0;
	*(((uint8 *)(&temp))+0) = p[0];
	*(((uint8 *)(&temp))+1) = p[1];
	*(((uint8 *)(&temp))+2) = p[2];
	*(((uint8 *)(&temp))+3) = p[3];
	*(((uint8 *)(&temp))+4) = p[4];
	*(((uint8 *)(&temp))+5) = p[5];
	*(((uint8 *)(&temp))+6) = p[6];
	*(((uint8 *)(&temp))+7) = p[7];
	return temp;
}

/***********************************************************************************************
*函数名: void disassmbleDouble8ByteS(double data, void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void disassmbleDouble8ByteS(double data, void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	p[0] = *(((uint8 *)(&data))+0);
	p[1] = *(((uint8 *)(&data))+1);
	p[2] = *(((uint8 *)(&data))+2);
	p[3] = *(((uint8 *)(&data))+3);
	p[4] = *(((uint8 *)(&data))+4);
	p[5] = *(((uint8 *)(&data))+5);
	p[6] = *(((uint8 *)(&data))+6);
	p[7] = *(((uint8 *)(&data))+7);
}

/***********************************************************************************************
*函数名: double assmbleDouble8ByteB(void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
double assmbleDouble8ByteB(void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	double temp = 0;
	*(((uint8 *)(&temp))+0) = p[7];
	*(((uint8 *)(&temp))+1) = p[6];
	*(((uint8 *)(&temp))+2) = p[5];
	*(((uint8 *)(&temp))+3) = p[4];
	*(((uint8 *)(&temp))+4) = p[3];
	*(((uint8 *)(&temp))+5) = p[2];
	*(((uint8 *)(&temp))+6) = p[1];
	*(((uint8 *)(&temp))+7) = p[0];
	return temp;
}

/***********************************************************************************************
*函数名: void disassmbleDouble8ByteB(double data, void *p_data)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void disassmbleDouble8ByteB(double data, void *p_data)
{
	uint8 * p = (uint8 *)p_data;
	p[0] = *(((uint8 *)(&data))+7);
	p[1] = *(((uint8 *)(&data))+6);
	p[2] = *(((uint8 *)(&data))+5);
	p[3] = *(((uint8 *)(&data))+4);
	p[4] = *(((uint8 *)(&data))+3);
	p[5] = *(((uint8 *)(&data))+2);
	p[6] = *(((uint8 *)(&data))+1);
	p[7] = *(((uint8 *)(&data))+0);
}

/***********************************************************************************************
*函数名: 
*功能描述: 
*特别说明:以太网标准多项式crc32校验
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
static const uint32 a256_crc32Table[256] =
{
	0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B,
	0x1A864DB2, 0x1E475005, 0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
	0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD, 0x4C11DB70, 0x48D0C6C7,
	0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
	0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3,
	0x709F7B7A, 0x745E66CD, 0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039,
	0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5, 0xBE2B5B58, 0xBAEA46EF,
	0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
	0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB,
	0xCEB42022, 0xCA753D95, 0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1,
	0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D, 0x34867077, 0x30476DC0,
	0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
	0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4,
	0x0808D07D, 0x0CC9CDCA, 0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE,
	0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02, 0x5E9F46BF, 0x5A5E5B08,
	0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
	0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC,
	0xB6238B25, 0xB2E29692, 0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6,
	0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A, 0xE0B41DE7, 0xE4750050,
	0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
	0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34,
	0xDC3ABDED, 0xD8FBA05A, 0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637,
	0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB, 0x4F040D56, 0x4BC510E1,
	0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
	0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5,
	0x3F9B762C, 0x3B5A6B9B, 0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF,
	0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623, 0xF12F560E, 0xF5EE4BB9,
	0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
	0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD,
	0xCDA1F604, 0xC960EBB3, 0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7,
	0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B, 0x9B3660C6, 0x9FF77D71,
	0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
	0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2,
	0x470CDD2B, 0x43CDC09C, 0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8,
	0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24, 0x119B4BE9, 0x155A565E,
	0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
	0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A,
	0x2D15EBE3, 0x29D4F654, 0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0,
	0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C, 0xE3A1CBC1, 0xE760D676,
	0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
	0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662,
	0x933EB0BB, 0x97FFAD0C, 0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668,
	0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};
static uint32 crc32Seed = 0;
void crc32_reinit(void)
{
	crc32Seed = 0xFFFFFFFF;
}
void crc32_calculate(uint8 *p_buff, uint32 len)
{
	uint32 index = 0;
	uint32 nTemp = 0; 
	uint8 i = 0;
	for(index = 0; index < len; index++)
	{
		crc32Seed ^= (uint32)p_buff[index];
		for (i = 0; i < 4; i++)
		{
			nTemp = a256_crc32Table[(crc32Seed >> 24) & 0xff];
			crc32Seed <<= 8;
			crc32Seed ^= nTemp;
		}
	}
}
uint32 crc32_get_crc(void)
{
	return crc32Seed;
}
uint32 crc32_calculate1(uint8 *p_buff, uint32 len)
{
	uint32 index = 0;
	uint32 nTemp = 0;
	uint32 temp = 0xFFFFFFFF;
	uint8 i = 0;
	for(index = 0; index < len; index++)
	{
		temp ^= (uint32)p_buff[index];
		for(i = 0; i < 4; i++)
		{
			nTemp = a256_crc32Table[(temp >> 24) & 0xff];
			temp <<= 8;
			temp ^= nTemp;
		}
	}
	return temp;
}

/***********************************************************************************************
*函数名: uint16 crc16_calculate_modbus(uint8 *ptr, uint16 len)
*功能描述: modbus crc16校验
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
static const uint8 a_crc16mTableHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40
};
static const uint8 a_crc16mTableLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
	0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
	0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
	0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
	0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
	0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
	0x40
};
uint16 crc16_calculate_modbus(uint8 *p_data, uint16 len)
{
	uint8 crchi = 0xff;
	uint8 crclo = 0xff;
	uint16 index;
	while (len--)
	{
		index = crclo ^ *p_data++;
		crclo = crchi ^ a_crc16mTableHi[index];
		crchi = a_crc16mTableLo[index];
	}
	return (crchi << 8 | crclo);
}

/***********************************************************************************************
*函数名: 
*功能描述: 
*特别说明:Table of CRC constants - implements x^16+x^12+x^5+1
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
static const uint16 a_crc16Table[] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};
static uint16 crc16Seed = 0;
void crc16_reinit(void)
{
	crc16Seed = 0;
}
void crc16_calculate(uint8 *p_buff, uint32 len)
{
	while(len--)
	{
		crc16Seed = a_crc16Table[((crc16Seed >> 8) ^ *p_buff++) & 0xFF] ^ (crc16Seed << 8);
	}
}
uint16 crc16_get_crc(void)
{
	return crc16Seed;
}
uint16 crc16_calculate1(uint8 *p_buff, uint32 len)
{
	uint16 cksum = 0;
	while(len--)
	{
		cksum = a_crc16Table[((cksum >> 8) ^ *p_buff++) & 0xFF] ^ (cksum << 8);
	}
	return cksum;
}

/***********************************************************************************************
*函数名: uint8 check_timeout(uint32 start, uint32 now, uint32 invt)
*功能描述: 
*特别说明:用于计算三个数字之间的间隔,一般用于定时器计算超时时间
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 check_timeout(uint32 start, uint32 now, uint32 invt)
{
	if((uint32)(start + invt) >= start)//未溢出
	{
		if((now >= (uint32)(start + invt)) || (now < start))
		{
			return 1;
		}
	}
	else//溢出
	{
		if((now < start) && (now >= (uint32)(start + invt)))
		{
			return 1;
		}
	}
	return 0;
}

/***********************************************************************************************
*函数名: uint32 calcu_interval(uint32 start, uint32 now)
*功能描述: 
*特别说明:用于计算两个数字之间的间隔,一般用于定时器计算间隔时间
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint32 calcu_interval(uint32 start, uint32 now)
{ 
	if(now >= start)
	{
		return (now - start);
	}
	else
	{
		return (uint32)(0xFFFFFFFF) - start + now + 1;
	}
}

/***********************************************************************************************
*函数名: void updata_roll_count8(uint8 *p_count)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void updata_roll_count8(uint8 *p_count)
{ 
	if(0xFF == p_count[0])
	{
		p_count[0] = 1;
	}
	else
	{
		p_count[0]++;
	}
}

/***********************************************************************************************
*函数名: void updata_roll_count16(uint16 *p_count)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void updata_roll_count16(uint16 *p_count)
{ 
	if(0xFFFF == p_count[0])
	{
		p_count[0] = 1;
	}
	else
	{
		p_count[0]++;
	}
}

/***********************************************************************************************
*函数名: void updata_roll_count32(uint32 *p_count)
*功能描述: 
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void updata_roll_count32(uint32 *p_count)
{ 
	if(0xFFFFFFFF == p_count[0])
	{
		p_count[0] = 1;
	}
	else
	{
		p_count[0]++;
	}
}

/***********************************************************************************************
*函数名: uint8 data_dec_bcd(uint8 dec)
*功能描述: 十进制转BCD码
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 data_dec_bcd(uint8 dec)
{
	return (((dec / 10 ) << 4) | (dec % 10));
}

/***********************************************************************************************
*函数名: uint8 bit_lsb_num(uint32 data)
*功能描述: 计算最低位1序号
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 bit_lsb_num(uint32 data)
{
	uint8 i = 0;
	for(i = 0; i < 32; i++)
	{
		if(((uint32)1 << i) == data)
		{
			break;
		}
	}
	return i; 
}

/***********************************************************************************************
*函数名: uint8 lookup_table_index_uint32(const uint32 arr[], uint16 arrLen, uint32 target, uint16 *pLowIndex, uint16 *pHighIndex)
*功能描述: 在数组中查找指定目标下标
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 lookup_table_index_uint32(const uint32 arr[], uint16 arrLen, uint32 target, uint16 *pLowIndex, uint16 *pHighIndex)
{
	uint16 mid, start, end;
	// 判断输入参数有效性
	if(arr == NULL || arrLen == 0 || pLowIndex == NULL || pHighIndex == NULL){
		return 0;
	}
	start = 0;
	end = arrLen - 1;
	// 判断目标在界外
	if(target < arr[start] || target > arr[end]){
		return 0;
	}
	if(target == arr[start]){
		*pLowIndex = start;
		*pHighIndex = start;
	}else if(target == arr[end]){
		*pLowIndex = end;
		*pHighIndex = end;
	}else{
		while(end - start > 1){
			mid = ((uint32)start + end) / 2;
			if(arr[mid] < target){
				start = mid;
			}else if(arr[mid] > target){
				end = mid;
			}else{
				*pLowIndex = mid;
				*pHighIndex = mid;
				return 1;
			}
		}
		*pLowIndex = start;
		*pHighIndex = end;
	}
	return 1;
}

/***********************************************************************************************
*函数名: uint32 calc_resistor_up(uint32 ResUp, uint32 ResDown, uint16 vt, uint16 Vref)
*功能描述: 计算测量点上部分阻值
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint32 calc_resistor_up(uint32 ResUp, uint32 ResDown, uint16 vt, uint16 Vref)
{
	uint32	RVal;
	// 防止公式计算溢出
	if(vt >= Vref || !vt){
		return 0;
	}
	//计算电阻值
	RVal = (uint32)(Vref - vt) * ResDown;
	RVal /= vt;

	if(RVal > ResUp){
		RVal -= ResUp;
	}else{
		RVal = 0;
	}
	return RVal;
}

/***********************************************************************************************
*函数名: uint32 calc_resistor_down(uint32 ResUp, uint32 ResDown, uint16 vt, uint16 Vref)
*功能描述: 计算测量点下部分阻值
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint32 calc_resistor_down(uint32 ResUp, uint32 ResDown, uint16 vt, uint16 Vref)
{
	uint32	RVal, div1, div2;
	//防止公式计算溢出
	if(vt >= Vref){
		vt = Vref - 1;
	}
	//计算电阻值
	div1 = ResUp * ((uint32)vt);
	div2 = Vref - vt;
	RVal = (div1+(div2>>1)) / div2;
	//减去下拉保护电阻阻值
	if(RVal >= ResDown){
		RVal = RVal - ResDown;
	}else{
		RVal = 0;
	}
	return RVal;
}

/***********************************************************************************************
*函数名: uint32 calc_resistor_para(uint32 ParRes, uint32 SrcRes)
*功能描述: 根据并联后阻值，计算并联电阻阻值
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint32 calc_resistor_para(uint32 ParRes, uint32 SrcRes)
{
	uint64 DstRes;

	if(SrcRes <= ParRes){
		return (uint32)-1;
	}else{
		DstRes = ((uint64)ParRes*SrcRes+(SrcRes - ParRes)/2)/(SrcRes - ParRes);
		if(DstRes >= (uint32)-1){
			return (uint32)-1;
		}else{
			return (uint32)DstRes;
		}
	}
}

/***********************************************************************************************
*函数名: uint8 bcd_to_dec(uint8 bcd)
*功能描述: bcd码转十进制
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 bcd_to_dec(uint8 bcd)
{
	return (uint8)(((bcd >> 4) * 10) + (bcd & 0x0F));

}

/***********************************************************************************************
*函数名: uint8 dec_to_bcd(uint8 dec)
*功能描述: 十进制转bcd码
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 dec_to_bcd(uint8 dec)
{
	return (uint8)(((dec / 10 ) << 4) | (dec % 10));
}

static const uint8 a_dayPerMon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const uint8 a_ldayPerMon[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/***********************************************************************************************
*函数名: void unixtime_to_localtime(uint32 unixTime, LOCAL_TIME *p_localTime)
*功能描述: unix时间->本地时间(未加时区偏移)
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void unixtime_to_localtime(uint32 unixTime, LOCAL_TIME *p_localTime)
{
	uint32 temp = 0;
	uint16 year = 0;
	uint8 month = 0;
	temp = unixTime % (60 * 60 * 24);//不足一天部分
	p_localTime->hour = temp / (60 * 60);
	p_localTime->minute = (temp % (60 * 60)) / 60;
	p_localTime->second = temp % 60;
	temp = unixTime / (60 * 60 * 24);//整天数
	p_localTime->week = (temp + 4) % 7;//1970/1/1 0:0:0 周4
	for(year = 1970; temp > 0; year++)
	{
		if((year % 4 == 0 && year % 100 != 0)
			|| (year % 400 == 0 && year % 3200 != 0)
			|| (year % 172800 == 0))
		{//闰年
			if (temp >= 366)
			{
				temp -= 366;
			}
			else
			{
				break;
			}
		}
		else
		{
			if(temp >= 365)
			{
				temp -= 365;
			}
			else
			{
				break;
			}
		}
	}
	p_localTime->year = year;
	month = 0;
	if((year % 4 == 0 && year % 100 != 0)
		|| (year % 400 == 0 && year % 3200 != 0)
		|| (year % 172800 == 0))
	{
		while(temp >= a_ldayPerMon[month])
		{
			temp -= a_ldayPerMon[month++];
		}
	}
	else
	{
		while(temp >= a_dayPerMon[month])
		{
			temp -= a_dayPerMon[month++];
		}
	}
	p_localTime->month = month;
	p_localTime->day = temp + 1;
}

/***********************************************************************************************
*函数名: void localtime_to_unixtime(LOCAL_TIME *p_localTime, uint32 *p_unixTime)
*功能描述: 本地时间->unix时间(未加时区偏移)
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void localtime_to_unixtime(LOCAL_TIME *p_localTime, uint32 *p_unixTime)
{
	uint32 temp = 0;
	uint16 year = 0;
	uint8 month = 0;

	if(p_localTime->year < 1970)
	{
		*p_unixTime = 0;
		return;
	}
	temp = 0;
	for(year = 1970; year < p_localTime->year; year++)
	{
		if((year % 4 == 0 && year % 100 != 0)
			|| (year % 400 == 0 && year % 3200 != 0)
			|| (year % 172800 == 0))
		{
			temp += 366;
		}
		else
		{
			temp += 365;
		}
	}
	if((p_localTime->year % 4 == 0 && p_localTime->year % 100 != 0)
		|| (p_localTime->year % 400 == 0 && p_localTime->year % 3200 != 0)
		|| (p_localTime->year % 172800 == 0))
	{
		for(month = 0; month < p_localTime->month; month++)
		{
			temp += a_ldayPerMon[month];
		}
	}
	else
	{
		for(month = 0; month < p_localTime->month; month++)
		{
			temp += a_dayPerMon[month];
		}
	}
	temp += p_localTime->day - 1;
	temp = temp * 24 * 60 * 60;
	temp += p_localTime->hour * 60 * 60;
	temp += p_localTime->minute * 60;
	temp += p_localTime->second;
	*p_unixTime = temp;
}

/***********************************************************************************************
*函数名: void sort_aescend_uint32(uint32 *p_data, uint16 dataNum)
*功能描述: 升序排列(由小到大)
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void sort_aescend_uint32(uint32 *p_data, uint16 dataNum)
{
	uint16 i = 0, j = 0;
	uint16 sortIndex = 0, sortLen = dataNum - 1;
	uint32 temp = 0;
	for(i = 0; i < dataNum-1; i++)
	{
		for(j = 0; j < sortLen; j++)
		{
			if(p_data[j] > p_data[j+1])
			{
				temp = p_data[j];
				p_data[j] = p_data[j+1];
				p_data[j+1] = temp;
				sortIndex = j;
			}
		}
		sortLen = sortIndex;
		if(0 == sortLen)
		{
			break;
		}
	}
}

