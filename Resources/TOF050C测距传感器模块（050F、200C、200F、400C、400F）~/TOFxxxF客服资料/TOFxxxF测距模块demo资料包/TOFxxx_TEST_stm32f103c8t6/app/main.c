#include "main.h"
#include "stdio.h"
uint8 a_testBuff[20];
//��������������
#define REG_RD_ADDR  0x0010//��������Ĵ���
void read_cmd_test_main(void)
{
	static uint32 timStamp = 0;
	if(tim_check_timeout(timStamp, tim_get_count(), 500))//500ms����һ�ζ�����
	{
		a_testBuff[0] = 0;//�㲥��ַ
		a_testBuff[1] = 3;//��������
		a_testBuff[2] = REG_RD_ADDR>>8;//regH
		a_testBuff[3] = REG_RD_ADDR;//regL
		a_testBuff[4] = 0;//numH(��֧�ֶ������Ĵ���)
		a_testBuff[5] = 1;//numL
		uart_tpm_tx_data(UART_TPM_ONE, a_testBuff, 6);
		timStamp = tim_get_count();
	}
}
#define REG_WR_ADDR  0x0005//����������ƼĴ���
#define REG_WR_DATA  1000//д������
void write_cmd_test_main(void)
{
	static uint32 timStamp = 0;
	static uint8 flag = 1;
	if(tim_check_timeout(timStamp, tim_get_count(), 500))//500ms��ֻ����һ��д����
	{
		if(flag)
		{
			flag = 0;
			a_testBuff[0] = 0;//�㲥��ַ
			a_testBuff[1] = 6;//д������
			a_testBuff[2] = REG_WR_ADDR>>8;//regH
			a_testBuff[3] = REG_WR_ADDR;//regL
			a_testBuff[4] = REG_WR_DATA>>8;//dataH
			a_testBuff[5] = (uint8)REG_WR_DATA;//dataL
			uart_tpm_tx_data(UART_TPM_ONE, a_testBuff, 6);
		}
		timStamp = tim_get_count();
	}
}
void cmd_test_main(void)
{
	//write_cmd_test_main();//����д�������
	read_cmd_test_main();//���Ͷ��������
}

//modbus���յ���ȷ����֡�ص�����
void modbus_recv_data(uint8 *p_data, uint16 len)
{
	uint16 readData = 0;
/*��������Ӧ  
  deviceAddr = p_data[0];//��Ӧ���豸��ַ
  cmd = p_data[1];      //��Ӧ������,��03
  byteNum = p_data[2];  //���ض����ݳ�*/
	readData = p_data[3]; //��������H
	readData <<= 8;
	readData |= p_data[4];//��������H
	printf("Distance:%umm\n", readData);
/*д������Ӧ  
  deviceAddr = p_data[0];//��Ӧ���豸��ַ
  cmd = p_data[1];      //��Ӧ������,д06
  regAddr = p_data[2];  //�Ĵ�����ַH
  regAddr <<= 8;
  regAddr |= p_data[3]; //�Ĵ�����ַL
  writeData = p_data[4];//д����H
  writeData <<= 8;
  writeData |= p_data[5];//д����L
*/
}


int main(void)
{
	mcu_disable_irq();
	mcu_init();
	tim_user_init();
	uart_tpm_user_init();
	mcu_enable_irq();

	while(1)
	{
		cmd_test_main();
		uart_tpm_main();
	}
}

