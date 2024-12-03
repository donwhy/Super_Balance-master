#include "app.h"


Print_data_t Print_data;
u8 debug_buff[72];

u8 Tips_buff[1024];
u16 Tips_cnt = 0;

#pragma import(__use_no_semihosting)             
               
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
  
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{ 	
	Tips_buff[Tips_cnt] = (u8) ch; 
	Tips_cnt++;
	return ch;
}

uint8_t Hardware_Status = 0;

void Start_Auto_Cali(void)
{
	printf("\r\n����У׼,�����ĵȴ�..........................\r\n");
	if(usb_core_dev.conn_state == 0x03) //�ж�USB���⴮���Ƿ�����
	{
		usb_vcp_send_data(&usb_core_dev, Tips_buff, Tips_cnt); // ͨ��USB���⴮�ڷ��ؽ��
	}
	UART3_SendDataDMA(Tips_buff,Tips_cnt); // ͨ�����ڷ��ؽ��
	Tips_cnt = 0;
}

void Auto_Cali_End(void)
{
	if(FaultBit.bit.M1_Encoder == 1 || FaultBit.bit.M1_Current == 1)
	{
		printf("\r\nFuck��Motor.AУ׼ʧ��,�����һ��Ӳ���Լ�,�����ٴγ���У׼��\r\n"),Hardware_Status = 1;
	}
	
	if(FaultBit.bit.M1_Encoder == 1 || FaultBit.bit.M1_Current == 1)
	{
		printf("\r\nFuck��Motor.BУ׼ʧ��,�����һ��Ӳ���Լ�,�����ٴγ���У׼��\r\n"),Hardware_Status = 1;
	}
	
	if(FaultBit.bit.flash == 1)
	{
		printf("\r\nFuck��W25Qxx����,�����һ��Ӳ���Լ죡\r\n"),     Hardware_Status = 1;
	}
	
	if(FaultBit.bit.IMU == 1)
	{
		printf("\r\nFuck��MPU6500����,�����һ��Ӳ���Լ죡\r\n"),    Hardware_Status = 1;
	}
	
	if(Hardware_Status == 0)printf("\r\n��ϲ�㣡У׼��ɣ���ػ�����,Ȼ��С����ˮƽ��+-5��������ס2-3�뼴�ɽ���ƽ�⣡\r\n");
	if(usb_core_dev.conn_state == 0x03) //�ж�USB���⴮���Ƿ�����
	{
		usb_vcp_send_data(&usb_core_dev, Tips_buff, Tips_cnt); // ͨ��USB���⴮�ڷ��ؽ��
	}
	UART3_SendDataDMA(Tips_buff,Tips_cnt); // ͨ�����ڷ��ؽ��
	Tips_cnt = 0;
}

void Start_Check_Hardware(void)
{
	printf("\r\n����Ӳ���Լ�,�����ĵȴ�..........................\r\n");
	if(usb_core_dev.conn_state == 0x03) //�ж�USB���⴮���Ƿ�����
	{
		usb_vcp_send_data(&usb_core_dev, Tips_buff, Tips_cnt); // ͨ��USB���⴮�ڷ��ؽ��
	}
	UART3_SendDataDMA(Tips_buff,Tips_cnt); // ͨ�����ڷ��ؽ��
	Tips_cnt = 0;
}

void Hardware_Tips(void)
{
	uint16_t W25QxxID,MPU6500_ID;
	
  W25QxxID = ReadW25Qxx_ID()&0x00ff;
	
	if(W25QxxID < 0x14 || W25QxxID > 0x17) //�ж������Ƿ��� W25Q16~W25Q128 ��Χ��������ڷ�Χ�ڻ����쳣���ƿ�����ʾ����
	{
		FaultBit.bit.flash = 1;
	}
	
	MPU6500_ID = IMU_Read_Reg(MPU6500_WHO_AM_I); // ��ȡMPU6500��ַ
	
	if(MPU6500_ID != 0x70)
	{
		FaultBit.bit.IMU = 1; // IMU����
	}
	
	printf("\r\nӲ���Լ����,�������:\r\n");
	
	if(Encoder_Type == 0)printf("\r\n������ʶ���ͺ�ΪMA732(��ʶ����������ԭ��ͼ����Ӧ���š������ߡ�VCC��ѹ�������Ƿ���ں�������麸)��\r\n");
	if(Encoder_Type == 1)printf("\r\n������ʶ���ͺ�ΪTLE5012(��ʶ����������ԭ��ͼ����Ӧ���š������ߡ�VCC��ѹ�������Ƿ���ں�������麸)��\r\n");
	if(Encoder_Type == 2)printf("\r\n������ʶ���ͺ�ΪMT6701(��ʶ����������ԭ��ͼ����Ӧ���š������ߡ�VCC��ѹ�������Ƿ���ں�������麸)��\r\n");
	
	if(FaultBit.bit.flash   == 1)printf("\r\nFuck��W25Q64��ȡʧ��(�����ԭ��ͼ����Ӧ���š����ӡ�VCC��ѹ���������W25QXX��ַ)��\r\n"),                  Hardware_Status = 1;
	if(FaultBit.bit.IMU     == 1)printf("\r\nFuck��MP6500��ȡʧ��(�����ԭ��ͼ����Ӧ���š����ӡ�VCC��ѹ)��\r\n"),																				 Hardware_Status = 1;
	if(FaultBit.bit.voltage == 1)printf("\r\nFuck����ص�ѹ�쳣(�����ԭ��ͼ����Ӧ���š���ѹ�����Ƿ񺸴�����麸)��\r\n"),                               Hardware_Status = 1;
	if(FaultBit.bit.Encoder == 1)printf("\r\nFuck��������ʶ���쳣(��ʶ����������ԭ��ͼ����Ӧ���š������ߡ�VCC��ѹ�������Ƿ���ں�������麸)��\r\n"),  Hardware_Status = 1;
	
	if(FaultBit.bit.M1_Encoder == 1)printf("\r\nFuck��M1�����������쳣(�����ԭ��ͼ����Ӧ���š������ߡ�VCC��ѹ�������Ƿ���ں�������麸,���ƨ���Ƿ�װ��װ�������)��\r\n"),       Hardware_Status = 1;
	if(FaultBit.bit.M2_Encoder == 1)printf("\r\nFuck��M2�����������쳣(�����ԭ��ͼ����Ӧ���š������ߡ�VCC��ѹ�������Ƿ���ں�������麸,���ƨ���Ƿ�������װ�������)��\r\n"),       Hardware_Status = 1;
	
	if(FaultBit.bit.M1_Current == 1)printf("\r\nFuck��Motor.A�����������쳣(�����ԭ��ͼ����Ӧ���š�VCC��ѹ���˷ŵ�·�����Ƿ���ں�������麸,������Ƿ����������������)��\r\n"),Hardware_Status = 1;
	if(FaultBit.bit.M2_Current == 1)printf("\r\nFuck��Motor.B�����������쳣(�����ԭ��ͼ����Ӧ���š�VCC��ѹ���˷ŵ�·�����Ƿ���ں�������麸,������Ƿ����������������)��\r\n"),Hardware_Status = 1;
	
	if(Hardware_Status == 0)printf("\r\n��ϲ�㣡��ʱ�޷����쳣,���Գ��Խ���У׼��\r\n");
	if(usb_core_dev.conn_state == 0x03) //�ж�USB���⴮���Ƿ�����
	{
		usb_vcp_send_data(&usb_core_dev, Tips_buff, Tips_cnt); // ͨ��USB���⴮�ڷ��ؽ��
	}
	UART3_SendDataDMA(Tips_buff,Tips_cnt); // ͨ�����ڷ��ؽ��
	Tips_cnt = 0;
	
	FaultBit.bit.flash = 0;
	FaultBit.bit.IMU = 0;
	FaultBit.bit.voltage = 0;
	FaultBit.bit.M1_Encoder = 0;
	FaultBit.bit.M2_Encoder = 0;
	FaultBit.bit.M1_Current = 0;
	FaultBit.bit.M2_Current = 0;
	Hardware_Status = 0;
}

u16 test_cnt = 0;

void UsartPrint(void) // ֧��ni������V1.0.1��λ����ӡ���ݣ�һ��16ͨ�����������޸�������λ��
{
	uint8_t i;
	
	if(Tx3_Flag == 0)
	{
		Tx3_Flag = 1;

		Print_data.data0  = 0; // �ѡ�0���滻������Ҫ�鿴�ı���
		Print_data.data1  = 0;
		Print_data.data2  = 0;
		Print_data.data3  = 0;
		Print_data.data4  = 0;
		Print_data.data5  = 0;
		Print_data.data6  = 0;
		Print_data.data7  = 0;
		Print_data.data8  = 0;
		Print_data.data9  = 0;
		Print_data.data10 = 0;
		Print_data.data11 = 0;
		Print_data.data12 = 0;
		Print_data.data13 = 0;
		Print_data.data14 = 0;
		Print_data.data15 = test_cnt++;	

	  debug_buff[0] = 0xAB;
		debug_buff[1] = 0;
		debug_buff[2] = 0;
		debug_buff[3] = 0xFA;
		debug_buff[4] = 64; 
		debug_buff[5] = 0;
		
		memcpy(debug_buff+6,&Print_data,sizeof(Print_data));
		
		debug_buff[70]=0;
		debug_buff[71]=0;
		
		for(i=0;i<70;i++)
		{	  				    
			debug_buff[70] += debug_buff[i];
			debug_buff[71] += debug_buff[70];
		}	
		UART3_SendDataDMA(debug_buff,72);
	}
}
