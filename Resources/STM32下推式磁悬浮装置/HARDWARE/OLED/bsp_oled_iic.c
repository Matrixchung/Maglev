/**
  ******************************************************************************
  * @file    bsp_oled_iic.c
  * @version V1.0
  * @author  fengweiguo
  * @brief   0.96'OLED IIC驱动函数源文件
  ******************************************************************************
*/

#include "bsp_oled_iic.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
 

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[8][128];	  //8*8行 128列
           //Y轴 X轴
//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE );	//使能GPIOA时钟
	
	//使能GPIOABC外设时钟使能	 ps:重映射必须使能AFIO时钟  
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //PA15 关闭jtag 开启sw
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_12|GPIO_Pin_15); 	//PA12,PA15 输出高
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线为输出方向 
	IIC_SDA=1;	  	  
	IIC_SCL=1;  //SDA SCL都拉高 为初始状态
	 
 	IIC_SDA=0;    //起始信号：SCL 为高电平时，SDA 由高电平向低电平跳变，
	  
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 ,防止产生结束信号
						//  ps:SCL为高电平时，SDA 由低电平向高电平跳变，结束传送数据
}	  

//产生IIC停止信号
void IIC_Stop(void)
{	
//结束信号：SCL 为高电平时，SDA 由低电平向高电平跳变（上升沿）
		IIC_SDA=0;
		IIC_SCL=1;
		 
		IIC_SDA=1;
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1; 	  //   相当于对对应ODR置1  即上拉输入
	IIC_SCL=1; 	 //时钟 拉高
	//（上述是释放总线）
	
	while(READ_SDA)   //读SDA 应答为低电平
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();//接收应答失败  产生IIC停止信号
			return 1;
		}
	}  //接收到应答信号后继续执行，否则接收应答失败
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

//产生ACK应答 
void IIC_Ack(void)
{
	IIC_SCL=0;   //低电平SDA数据可变
	
	SDA_OUT();   //设置为推挽输出 PB7
	IIC_SDA=0;   //向发送数据的 IC(24c02) 发出 应答信号
	 
	IIC_SCL=1;
	 //SCL拉高 即发送数据（时钟信号高电平期间 SDA要稳定，即数据不能变）
	
	IIC_SCL=0;  //恢复低电平
}

//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;//低电平SDA数据可变
	SDA_OUT();//设置为推挽输出 PB7
	IIC_SDA=1;//向发送数据的 IC(24c02) 发出非应答信号
	
	IIC_SCL=1; //SCL拉高 即发送数据（时钟信号高电平期间 SDA要稳定，即数据不能变）
	
	IIC_SCL=0;
}			


//IIC发送一个字节  单片机为主机  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;//先发送最高位
        txd<<=1; 	  
		   	IIC_SCL=1;
				IIC_SCL=0;		//SCL拉高 即发送数据（时钟信号高电平期间 SDA要稳定，即数据不能变）
    }	 
} 

//读1个字节，
//参数ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
		{
					IIC_SCL=0; 
			 
					IIC_SCL=1;
					receive<<=1;
					if(READ_SDA)receive++;   //先读取的为高位
			 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;	    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址  全部128列  列地址自增
		for(n=0;n<128;n++)
				OLED_WR_Byte( OLED_GRAM[i][n],OLED_DATA );  
	}   
}
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	  IIC_Start();  //起始信号
	 
		IIC_Send_Byte(ADDRESS_WRITE);   //发送写命令（设备地址）   
		IIC_Wait_Ack();	//等待应答信号到来  
	  
	  if(cmd==0)
		{
		    IIC_Send_Byte(WRITE_CMD);   //发送写'命令'命令 
				IIC_Wait_Ack();	//等待应答信号到来  
		}
		else{
				IIC_Send_Byte(WRITE_DATA);   //发送写'数据'命令 
				IIC_Wait_Ack();	//等待应答信号到来  	
		}
		
		IIC_Send_Byte(dat);   //发送数据/命令
		IIC_Wait_Ack();	//等待应答信号到来  
	
    IIC_Stop();//产生一个停止条件  
} 

	
	  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
		OLED_GRAM[i][n]=0X00;  
	OLED_Refresh_Gram();//更新显示
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;  //计算出页的位置
	bx=y%8;     //该页中的具体位置
	temp=1<<(7-bx); 
	if(t)OLED_GRAM[pos][x]|=temp;    //一个字节八个点
	else OLED_GRAM[pos][x]&=~temp;	    
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)
			OLED_DrawPoint(x,y,dot); //调用画点函数
	}													    
	OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
			if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
			else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
			else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
			else return;								//没有的字库
				
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);    //调用画点函数
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
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
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
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);//开头为0则显示空白
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
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
//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
 	IIC_Init();
	delay_ms(200);	//		  
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0

	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
		 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
	OLED_Clear();
}  
 

