/**
  ******************************************************************************
  * @file    bsp_oled_iic.h
  * @version V1.0
  * @author  fengweiguo
  * @brief   0.96'OLED IIC驱动函数头文件
  ******************************************************************************
*/

#ifndef __BSP_OLED_IIC_H
#define __BSP_OLED_IIC_H			  	


#include "sys.h"  	
    						  
//*******************************IIC模块声明***************************************
// IIC引脚 IO方向设置
#define SDA_IN()  {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=(u32)8<<28;}  //1000 输入 PA15
#define SDA_OUT() {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=(u32)3<<28;}  //0010 推挽输出 PA15
//IO操作函数	 
#define IIC_SCL    PAout(12) //SCL（模拟的，也可以用其他线，但要注意硬件连接）
#define IIC_SDA    PAout(15) //SDA	 
#define READ_SDA   PAin(15)  //输入SDA 
//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
//*******************************************************************************

//*******************************OLED模块声明***************************************
#define ADDRESS_WRITE          0x78
#define ADDRESS_READ           0x79
#define WRITE_CMD              0x00   //控制字节 写命令命令
#define WRITE_DATA             0x40  //控制字节 写数据命令
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_Refresh_Gram(void);  		  //更新显存到LCD	
void OLED_WR_Byte(u8 dat,u8 cmd);	  //向SSD1306写入一个字节。 dat:要写入的数据/命令  cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_Display_On(void);//开启OLED显示   
void OLED_Display_Off(void);//关闭OLED显示    
void OLED_Clear(void);//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Init(void);//初始化SSD1306		

void OLED_DrawPoint(u8 x,u8 y,u8 t);//画点  x:0~127   y:0~63   t:1 填充 0,清空

void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);  //x1,y1,x2,y2 填充区域的对角坐标   
																									//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63   dot:0,清空;1,填充	  
 
 


void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);//在指定位置显示一个字符,包括部分字符
																											//x:0~127
																											//y:0~63
																											//mode:0,反白显示;1,正常显示				 
																											//size:选择字体 12/16/24
																											
																											
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//显示2个数字
																										//x,y :起点坐标	 
																										//len :数字的位数
																										//size:字体大小
																										//mode:模式	0,填充模式;1,叠加模式
																										//num:数值(0~4294967295);

void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);	//显示字符串
																										//x,y:起点坐标  
																										//size:字体大小 
																										//*p:字符串起始地址 
#endif /*__BSP_OLED_IIC_H*/
	 

