/**
  ******************************************************************************
  * @file    bsp_oled_iic.h
  * @version V1.0
  * @author  fengweiguo
  * @brief   0.96'OLED IIC��������ͷ�ļ�
  ******************************************************************************
*/

#ifndef __BSP_OLED_IIC_H
#define __BSP_OLED_IIC_H			  	


#include "sys.h"  	
    						  
//*******************************IICģ������***************************************
// IIC���� IO��������
#define SDA_IN()  {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=(u32)8<<28;}  //1000 ���� PA15
#define SDA_OUT() {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=(u32)3<<28;}  //0010 ������� PA15
//IO��������	 
#define IIC_SCL    PAout(12) //SCL��ģ��ģ�Ҳ�����������ߣ���Ҫע��Ӳ�����ӣ�
#define IIC_SDA    PAout(15) //SDA	 
#define READ_SDA   PAin(15)  //����SDA 
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
//*******************************************************************************

//*******************************OLEDģ������***************************************
#define ADDRESS_WRITE          0x78
#define ADDRESS_READ           0x79
#define WRITE_CMD              0x00   //�����ֽ� д��������
#define WRITE_DATA             0x40  //�����ֽ� д��������
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//OLED�����ú���
void OLED_Refresh_Gram(void);  		  //�����Դ浽LCD	
void OLED_WR_Byte(u8 dat,u8 cmd);	  //��SSD1306д��һ���ֽڡ� dat:Ҫд�������/����  cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_Display_On(void);//����OLED��ʾ   
void OLED_Display_Off(void);//�ر�OLED��ʾ    
void OLED_Clear(void);//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Init(void);//��ʼ��SSD1306		

void OLED_DrawPoint(u8 x,u8 y,u8 t);//����  x:0~127   y:0~63   t:1 ��� 0,���

void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);  //x1,y1,x2,y2 �������ĶԽ�����   
																									//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63   dot:0,���;1,���	  
 
 


void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);//��ָ��λ����ʾһ���ַ�,���������ַ�
																											//x:0~127
																											//y:0~63
																											//mode:0,������ʾ;1,������ʾ				 
																											//size:ѡ������ 12/16/24
																											
																											
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//��ʾ2������
																										//x,y :�������	 
																										//len :���ֵ�λ��
																										//size:�����С
																										//mode:ģʽ	0,���ģʽ;1,����ģʽ
																										//num:��ֵ(0~4294967295);

void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);	//��ʾ�ַ���
																										//x,y:�������  
																										//size:�����С 
																										//*p:�ַ�����ʼ��ַ 
#endif /*__BSP_OLED_IIC_H*/
	 

