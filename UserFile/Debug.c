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
	printf("\r\n正在校准,请耐心等待..........................\r\n");
	if(usb_core_dev.conn_state == 0x03) //判断USB虚拟串口是否在线
	{
		usb_vcp_send_data(&usb_core_dev, Tips_buff, Tips_cnt); // 通过USB虚拟串口返回结果
	}
	UART3_SendDataDMA(Tips_buff,Tips_cnt); // 通过串口返回结果
	Tips_cnt = 0;
}

void Auto_Cali_End(void)
{
	if(FaultBit.bit.M1_Encoder == 1 || FaultBit.bit.M1_Current == 1)
	{
		printf("\r\nFuck！Motor.A校准失败,请进行一次硬件自检,或者再次尝试校准！\r\n"),Hardware_Status = 1;
	}
	
	if(FaultBit.bit.M1_Encoder == 1 || FaultBit.bit.M1_Current == 1)
	{
		printf("\r\nFuck！Motor.B校准失败,请进行一次硬件自检,或者再次尝试校准！\r\n"),Hardware_Status = 1;
	}
	
	if(FaultBit.bit.flash == 1)
	{
		printf("\r\nFuck！W25Qxx错误,请进行一次硬件自检！\r\n"),     Hardware_Status = 1;
	}
	
	if(FaultBit.bit.IMU == 1)
	{
		printf("\r\nFuck！MPU6500错误,请进行一次硬件自检！\r\n"),    Hardware_Status = 1;
	}
	
	if(Hardware_Status == 0)printf("\r\n恭喜你！校准完成！请关机重启,然后将小车抚水平在+-5度以内稳住2-3秒即可进入平衡！\r\n");
	if(usb_core_dev.conn_state == 0x03) //判断USB虚拟串口是否在线
	{
		usb_vcp_send_data(&usb_core_dev, Tips_buff, Tips_cnt); // 通过USB虚拟串口返回结果
	}
	UART3_SendDataDMA(Tips_buff,Tips_cnt); // 通过串口返回结果
	Tips_cnt = 0;
}

void Start_Check_Hardware(void)
{
	printf("\r\n正在硬件自检,请耐心等待..........................\r\n");
	if(usb_core_dev.conn_state == 0x03) //判断USB虚拟串口是否在线
	{
		usb_vcp_send_data(&usb_core_dev, Tips_buff, Tips_cnt); // 通过USB虚拟串口返回结果
	}
	UART3_SendDataDMA(Tips_buff,Tips_cnt); // 通过串口返回结果
	Tips_cnt = 0;
}

void Hardware_Tips(void)
{
	uint16_t W25QxxID,MPU6500_ID;
	
  W25QxxID = ReadW25Qxx_ID()&0x00ff;
	
	if(W25QxxID < 0x14 || W25QxxID > 0x17) //判断容量是否在 W25Q16~W25Q128 范围，如果不在范围内或者异常则红灯快闪提示错误
	{
		FaultBit.bit.flash = 1;
	}
	
	MPU6500_ID = IMU_Read_Reg(MPU6500_WHO_AM_I); // 读取MPU6500地址
	
	if(MPU6500_ID != 0x70)
	{
		FaultBit.bit.IMU = 1; // IMU错误
	}
	
	printf("\r\n硬件自检完毕,结果如下:\r\n");
	
	if(Encoder_Type == 0)printf("\r\n编码器识别型号为MA732(如识别错误请根据原理图检查对应引脚、连接线、VCC电压、焊接是否存在焊错或者虚焊)！\r\n");
	if(Encoder_Type == 1)printf("\r\n编码器识别型号为TLE5012(如识别错误请根据原理图检查对应引脚、连接线、VCC电压、焊接是否存在焊错或者虚焊)！\r\n");
	if(Encoder_Type == 2)printf("\r\n编码器识别型号为MT6701(如识别错误请根据原理图检查对应引脚、连接线、VCC电压、焊接是否存在焊错或者虚焊)！\r\n");
	
	if(FaultBit.bit.flash   == 1)printf("\r\nFuck！W25Q64读取失败(请根据原理图检查对应引脚、焊接、VCC电压、软件则检查W25QXX地址)！\r\n"),                  Hardware_Status = 1;
	if(FaultBit.bit.IMU     == 1)printf("\r\nFuck！MP6500读取失败(请根据原理图检查对应引脚、焊接、VCC电压)！\r\n"),																				 Hardware_Status = 1;
	if(FaultBit.bit.voltage == 1)printf("\r\nFuck！电池电压异常(请根据原理图检查对应引脚、分压电阻是否焊错或者虚焊)！\r\n"),                               Hardware_Status = 1;
	if(FaultBit.bit.Encoder == 1)printf("\r\nFuck！编码器识别异常(如识别错误请根据原理图检查对应引脚、连接线、VCC电压、焊接是否存在焊错或者虚焊)！\r\n"),  Hardware_Status = 1;
	
	if(FaultBit.bit.M1_Encoder == 1)printf("\r\nFuck！M1编码器数据异常(请根据原理图检查对应引脚、连接线、VCC电压、焊接是否存在焊错或者虚焊,电机屁股是否安装安装径向磁铁)！\r\n"),       Hardware_Status = 1;
	if(FaultBit.bit.M2_Encoder == 1)printf("\r\nFuck！M2编码器数据异常(请根据原理图检查对应引脚、连接线、VCC电压、焊接是否存在焊错或者虚焊,电机屁股是否正常安装径向磁铁)！\r\n"),       Hardware_Status = 1;
	
	if(FaultBit.bit.M1_Current == 1)printf("\r\nFuck！Motor.A电机电流检测异常(请根据原理图检查对应引脚、VCC电压、运放电路焊接是否存在焊错或者虚焊,电机线是否与主板进行了连接)！\r\n"),Hardware_Status = 1;
	if(FaultBit.bit.M2_Current == 1)printf("\r\nFuck！Motor.B电机电流检测异常(请根据原理图检查对应引脚、VCC电压、运放电路焊接是否存在焊错或者虚焊,电机线是否与主板进行了连接)！\r\n"),Hardware_Status = 1;
	
	if(Hardware_Status == 0)printf("\r\n恭喜你！暂时无发现异常,可以尝试进行校准！\r\n");
	if(usb_core_dev.conn_state == 0x03) //判断USB虚拟串口是否在线
	{
		usb_vcp_send_data(&usb_core_dev, Tips_buff, Tips_cnt); // 通过USB虚拟串口返回结果
	}
	UART3_SendDataDMA(Tips_buff,Tips_cnt); // 通过串口返回结果
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

void UsartPrint(void) // 支持ni名助手V1.0.1上位机打印数据，一共16通道，可自行修改其它上位机
{
	uint8_t i;
	
	if(Tx3_Flag == 0)
	{
		Tx3_Flag = 1;

		Print_data.data0  = 0; // 把“0”替换成所需要查看的变量
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
