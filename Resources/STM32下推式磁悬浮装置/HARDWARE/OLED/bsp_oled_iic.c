/**
  ******************************************************************************
  * @file    bsp_oled_iic.c
  * @version V1.0
  * @author  fengweiguo
  * @brief   0.96'OLED IIC��������Դ�ļ�
  ******************************************************************************
*/

#include "bsp_oled_iic.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
 

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[8][128];	  //8*8�� 128��
           //Y�� X��
//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE );	//ʹ��GPIOAʱ��
	
	//ʹ��GPIOABC����ʱ��ʹ��	 ps:��ӳ�����ʹ��AFIOʱ��  
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //PA15 �ر�jtag ����sw
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_12|GPIO_Pin_15); 	//PA12,PA15 �����
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda��Ϊ������� 
	IIC_SDA=1;	  	  
	IIC_SCL=1;  //SDA SCL������ Ϊ��ʼ״̬
	 
 	IIC_SDA=0;    //��ʼ�źţ�SCL Ϊ�ߵ�ƽʱ��SDA �ɸߵ�ƽ��͵�ƽ���䣬
	  
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� ,��ֹ���������ź�
						//  ps:SCLΪ�ߵ�ƽʱ��SDA �ɵ͵�ƽ��ߵ�ƽ���䣬������������
}	  

//����IICֹͣ�ź�
void IIC_Stop(void)
{	
//�����źţ�SCL Ϊ�ߵ�ƽʱ��SDA �ɵ͵�ƽ��ߵ�ƽ���䣨�����أ�
		IIC_SDA=0;
		IIC_SCL=1;
		 
		IIC_SDA=1;
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1; 	  //   �൱�ڶԶ�ӦODR��1  ����������
	IIC_SCL=1; 	 //ʱ�� ����
	//���������ͷ����ߣ�
	
	while(READ_SDA)   //��SDA Ӧ��Ϊ�͵�ƽ
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();//����Ӧ��ʧ��  ����IICֹͣ�ź�
			return 1;
		}
	}  //���յ�Ӧ���źź����ִ�У��������Ӧ��ʧ��
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

//����ACKӦ�� 
void IIC_Ack(void)
{
	IIC_SCL=0;   //�͵�ƽSDA���ݿɱ�
	
	SDA_OUT();   //����Ϊ������� PB7
	IIC_SDA=0;   //�������ݵ� IC(24c02) ���� Ӧ���ź�
	 
	IIC_SCL=1;
	 //SCL���� ���������ݣ�ʱ���źŸߵ�ƽ�ڼ� SDAҪ�ȶ��������ݲ��ܱ䣩
	
	IIC_SCL=0;  //�ָ��͵�ƽ
}

//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;//�͵�ƽSDA���ݿɱ�
	SDA_OUT();//����Ϊ������� PB7
	IIC_SDA=1;//�������ݵ� IC(24c02) ������Ӧ���ź�
	
	IIC_SCL=1; //SCL���� ���������ݣ�ʱ���źŸߵ�ƽ�ڼ� SDAҪ�ȶ��������ݲ��ܱ䣩
	
	IIC_SCL=0;
}			


//IIC����һ���ֽ�  ��Ƭ��Ϊ����  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;//�ȷ������λ
        txd<<=1; 	  
		   	IIC_SCL=1;
				IIC_SCL=0;		//SCL���� ���������ݣ�ʱ���źŸߵ�ƽ�ڼ� SDAҪ�ȶ��������ݲ��ܱ䣩
    }	 
} 

//��1���ֽڣ�
//����ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
		{
					IIC_SCL=0; 
			 
					IIC_SCL=1;
					receive<<=1;
					if(READ_SDA)receive++;   //�ȶ�ȡ��Ϊ��λ
			 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;	    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ  ȫ��128��  �е�ַ����
		for(n=0;n<128;n++)
				OLED_WR_Byte( OLED_GRAM[i][n],OLED_DATA );  
	}   
}
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	  IIC_Start();  //��ʼ�ź�
	 
		IIC_Send_Byte(ADDRESS_WRITE);   //����д����豸��ַ��   
		IIC_Wait_Ack();	//�ȴ�Ӧ���źŵ���  
	  
	  if(cmd==0)
		{
		    IIC_Send_Byte(WRITE_CMD);   //����д'����'���� 
				IIC_Wait_Ack();	//�ȴ�Ӧ���źŵ���  
		}
		else{
				IIC_Send_Byte(WRITE_DATA);   //����д'����'���� 
				IIC_Wait_Ack();	//�ȴ�Ӧ���źŵ���  	
		}
		
		IIC_Send_Byte(dat);   //��������/����
		IIC_Wait_Ack();	//�ȴ�Ӧ���źŵ���  
	
    IIC_Stop();//����һ��ֹͣ����  
} 

	
	  	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
		OLED_GRAM[i][n]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;  //�����ҳ��λ��
	bx=y%8;     //��ҳ�еľ���λ��
	temp=1<<(7-bx); 
	if(t)OLED_GRAM[pos][x]|=temp;    //һ���ֽڰ˸���
	else OLED_GRAM[pos][x]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)
			OLED_DrawPoint(x,y,dot); //���û��㺯��
	}													    
	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
			if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
			else if(size==16)temp=asc2_1608[chr][t];	//����1608����
			else if(size==24)temp=asc2_2412[chr][t];	//����2412����
			else return;								//û�е��ֿ�
				
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);    //���û��㺯��
				temp<<=1;
				y++;
				if((y-y0)==size)
				{
					y=y0;
					x++;
					break;
				}
			}  	 
    }          
}
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);//��ͷΪ0����ʾ�հ�
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if( x>(128-(size/2)) )
				{
					x=0;
					y+=size;
				}
        if(y>(64-size))
				{
					y=x=0;
					OLED_Clear();
				}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}	   
//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
 	IIC_Init();
	delay_ms(200);	//		  
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
	OLED_Clear();
}  
 

