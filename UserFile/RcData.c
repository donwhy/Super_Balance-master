
#include "app.h"

u16 Sbus_values[12];
u8 Sbus_Data[32];

s16 CH0 = 1024;//352 - 1024 - 1696
s16 CH1 = 1024;//352 - 1024 - 1696 
s16 CH2 = 0;
s16 CH3 = 0;

RC_t Rc;

u16 BLE_Cnt = 0;
uint8_t Rc_flag = 0;

void RcData(void)
{

//	if(U4_IDLE_Flag == 1) // 接收到串口4数据
//  {
//		U4_IDLE_Flag = 0;
//		UART4_SendDataDMA(Get_usart4_data,8); // 把收到的前8个数据发出去
//	}
	
	if(U1_IDLE_Flag == 1 || U2_IDLE_Flag == 1)
	{
		if(U2_IDLE_Flag == 1)
		{
			if(Sbus_Data[0] == 0x0f && Sbus_Data[25] == 0x00) // FUTABA航模SBUS接收机通道解析
			{
				Sbus_values[0]=Sbus_Data[1]|((Sbus_Data[2]&0x07)<<8);
				Sbus_values[1]=((Sbus_Data[3]&0x3f)<<5)|((Sbus_Data[2]&0xf8)>>3);
				Sbus_values[2]=((Sbus_Data[5]&0x01)<<10)|Sbus_Data[4]<<2|((Sbus_Data[3]&0xc0)>>6);
				Sbus_values[3]=((Sbus_Data[6]&0x0f)<<7)|((Sbus_Data[5]&0xfe)>>1);
				Sbus_values[4]=((Sbus_Data[7]&0x7f)<<4)|((Sbus_Data[6]&0xf0)>>4);
				Sbus_values[5]=((Sbus_Data[9]&0x03)<<9)|Sbus_Data[8]<<1|((Sbus_Data[7]&0x80)>>7);
				Sbus_values[6]=((Sbus_Data[10]&0x1f)<<6)|((Sbus_Data[9]&0xfc)>>2);	
				Sbus_values[7]=(Sbus_Data[11]<<3)|((Sbus_Data[10]&0xe0)>>5);	
				Sbus_values[8]=Sbus_Data[12]|((Sbus_Data[13]&0x07)<<8);
				Sbus_values[9]=((Sbus_Data[14]&0x3f)<<5)|((Sbus_Data[13]&0xf8)>>3);
				Sbus_values[10]=((Sbus_Data[16]&0x01)<<10)|Sbus_Data[15]<<2|((Sbus_Data[14]&0xc0)>>6);
				Sbus_values[11]=((Sbus_Data[17]&0x0f)<<7)|((Sbus_Data[16]&0xfe)>>1);
				Sbus_Data[0] = 0;
				
				Rc.ch0 = -(Sbus_values[1] - 1023) * 0.5f; // 前后
				Rc.ch1 = -(Sbus_values[0] - 1023) * 0.5f; // 旋转
				if(Sbus_values[4] > 1300)Rc.ch2 = 1; else Rc.ch2 = 0; //第5通道3段开关，最大值开启灯光，最小值关闭电机动力中间无作用
				
				if(Sbus_values[4] < 700)
				{
					Car.Protect_flag = 1;
					Car.AutoStandUp = 0;// 终止一键起立
				}
				
				if(abs(Rc.ch0) < 15)
				{
					Rc.ch0 = 0; 
					Rc.x += (Rc.ch0 - Rc.x) * 0.05f;
					if(fabs(Rc.x) < 0.0001f)
					{
						Rc.x = 0.0f;
						Rc.x_flag = 0;
					}
				}
				else if(Rc.ch0 > 15)
				{
					Rc.ch0 -= 15;
					Rc.x += (Rc.ch0 - Rc.x) * 0.05f;
					Rc.x_flag = 1;
				}
				else if(Rc.ch0 < -15)
				{
					Rc.ch0 += 15;
					Rc.x += (Rc.ch0 - Rc.x) * 0.05f;
					Rc.x_flag = 1;
				}
				
				if(abs(Rc.ch1) < 15)
				{
					Rc.ch1 = 0; 
					Rc.z += (Rc.ch1 - Rc.z) * 0.05f;
					if(fabs(Rc.z) < 0.0001f)
					{
						Rc.z = 0.0f;
						Rc.z_flag = 0;
					}
				}
				else if(Rc.ch1 > 15)
				{
					Rc.ch1 -= 15;
					Rc.z += (Rc.ch1 - Rc.z) * 0.05f;
					Rc.z_flag = 1;
				}
				else if(Rc.ch1 < -15)
				{
					Rc.ch1 += 15;
					Rc.z += (Rc.ch1 - Rc.z) * 0.05f;
					Rc.z_flag = 1;
				}
			}
			U2_IDLE_Flag = 0;
		}
		if(U1_IDLE_Flag == 1)
		{
			U1_IDLE_Flag = 0;
			BLE_Cnt = 0;
			if(Get_usart1_data[0] == 0x20 && Get_usart1_data[1] == 0x0f && Get_usart1_data[14] == 0x02) // 游戏手柄控制数据
			{
				if(Get_usart1_data[10] > 250)
				{
					Car.Protect_flag = 1;
					Car.AutoStandUp = 0;// 终止一键起立
				}
				
				if(Get_usart1_data[11] > 250 && Rc_flag == 0)
				{
					Rc_flag = 1;
					if(Car.Rc_Mode == 0)
					{
						Car.Rc_Mode = 1; 
					}
					else if(Car.Rc_Mode == 1)
					{
						Car.Rc_Mode = 0;
					}
				}
				else if(Get_usart1_data[11] < 10)Rc_flag = 0;
					
				Rc.ch0 = (Get_usart1_data[4] - 128); // = +- 128 -前 +后
				
				if(Car.Rc_Mode == 1)Rc.ch1 = (Get_usart1_data[3] - 128); 
				else Rc.ch1 = (Get_usart1_data[5] - 128);
				
				if(Get_usart1_data[8] == 2 && CH2 == 0)
				{
					CH2 = 3;
				}
				else if(Get_usart1_data[8] != 2 && CH2 == 3)
				{
					CH2 = 0;
					if(Rc.ch2 == 0)Rc.ch2 = 1; else if(Rc.ch2 == 1)Rc.ch2 = 0;
				}
				else if(Get_usart1_data[8] == 4 && CH3 == 0)  // 左 0x04 右 0x08
				{
					CH3 = 3;
				}
				else if(Get_usart1_data[8] != 4 && CH3 == 3)
				{
					CH3 = 0;
					if(Car.Sensitivity < 25.0f) 
					{
						Car.Sensitivity = 40.0f; 
	//					B_LED_Status(1);
					}
					else if(Car.Sensitivity > 25.0f)
					{
						Car.Sensitivity = 20.0f;
	//					B_LED_Status(0);
					}
				}
				else if(Get_usart1_data[8] == 1)
				{
					TMR10->c1dt = 200;
					if(Car.AutoStandUp == 0)Car.AutoStandUp = 1;
					
	//				if(Car.Protect_flag > 0)
	//				{
	//					Car.Protect_flag = 0;
	//					M1_Foc.Cali_Status = 0;
	//					M2_Foc.Cali_Status = 0;
	//				}
				}
				else TMR10->c1dt = 0;
				
				if(abs(Rc.ch0) < 8)
				{
					Rc.ch0 = 0; 
					Rc.x += (Rc.ch0 - Rc.x) * 0.5f;
					if(fabs(Rc.x) < 0.0001f)
					{
						Rc.x = 0.0f;
						Rc.x_flag = 0;
					}
				}
				else if(Rc.ch0 > 8)
				{
					Rc.ch0 -= 8;
					Rc.x += (Rc.ch0 - Rc.x) * 0.25f;
					Rc.x_flag = 1;
				}
				else if(Rc.ch0 < -8)
				{
					Rc.ch0 += 8;
					Rc.x += (Rc.ch0 - Rc.x) * 0.25f;
					Rc.x_flag = 1;
				}
				
				if(abs(Rc.ch1) < 8)
				{
					Rc.ch1 = 0; 
					Rc.z += (Rc.ch1 - Rc.z) * 0.8f;
					if(fabs(Rc.z) < 0.0001f)
					{
						Rc.z = 0.0f;
						Rc.z_flag = 0;
					}
				}
				else if(Rc.ch1 > 8)
				{
					Rc.ch1 -= 8;
					Rc.z += (Rc.ch1 - Rc.z) * 0.1f;
					Rc.z_flag = 1;
				}
				else if(Rc.ch1 < -8)
				{
					Rc.ch1 += 8;
					Rc.z += (Rc.ch1 - Rc.z) * 0.1f;
					Rc.z_flag = 1;
				}
			}
			else if(Get_usart1_data[0] == 0x20 && Get_usart1_data[1] == 0x0f && Get_usart1_data[14] == 0x66) // 小程序控制数据
			{
				Rc.ch0 = Get_usart1_data[5]; // = +- 100 
				Rc.ch1 = Get_usart1_data[4];
				Rc.ch2 = Get_usart1_data[2];
				
				if(Get_usart1_data[8] == 2 && CH2 == 0)
				{
					CH2 = 3;
				}
				else if(Get_usart1_data[8] != 2 && CH2 == 3)
				{
					CH2 = 0;
				}
				else if(Get_usart1_data[8] == 4 && CH3 == 0)
				{
					CH3 = 3;
				}
				else if(Get_usart1_data[8] != 4 && CH3 == 3)
				{
					CH3 = 0;
					if(Car.Sensitivity < 10.0f) 
					{
						Car.Sensitivity = 12.0f; 
						B_LED(1);
					}
					else if(Car.Sensitivity > 10.0f)
					{
						Car.Sensitivity = 8.0f;
						B_LED(0);
					}
				}
				else if(Get_usart1_data[8] == 1)
				{
					TMR10->c1dt = 200;
					if(Car.Protect_flag > 0)
					{
						Car.Protect_flag = 0;
						M1_Foc.Cali_Status = 0;
						M2_Foc.Cali_Status = 0;
					}
				}
				else TMR10->c1dt = 0;
				
				if(abs(Rc.ch0) < 5)
				{
					Rc.ch0 = 0; 
					Rc.x += (Rc.ch0 - Rc.x) * 0.1f;
					if(fabs(Rc.x) < 0.001f)
					{
						Rc.x = 0.0f;
						Rc.x_flag = 0;
					}
				}
				else if(Rc.ch0 > 5)
				{
					Rc.ch0 -= 5;
					Rc.x += (Rc.ch0 - Rc.x) * 0.5f;
					Rc.x_flag = 1;
				}
				else if(Rc.ch0 < -5)
				{
					Rc.ch0 += 5;
					Rc.x += (Rc.ch0 - Rc.x) * 0.5f;
					Rc.x_flag = 1;
				}
				
				if(abs(Rc.ch1) < 25)
				{
					Rc.ch1 = 0; 
					Rc.z += (Rc.ch1 - Rc.z) * 0.1f;
					if(fabs(Rc.z) < 0.001f)
					{
						Rc.z = 0.0f;
						Rc.z_flag = 0;
					}
				}
				else if(Rc.ch1 > 25)
				{
					Rc.ch1 -= 25;
					Rc.z += (Rc.ch1 - Rc.z) * 0.5f;
					Rc.z_flag = 1;
				}
				else if(Rc.ch1 < -25)
				{
					Rc.ch1 += 25;
					Rc.z += (Rc.ch1 - Rc.z) * 0.5f;
					Rc.z_flag = 1;
				}
			}
			Get_usart1_data[0] = Get_usart1_data[1] = 0;
		}
	}
	else
	{
		if(++BLE_Cnt >= 150)
		{
			BLE_Cnt = 500;
			if(Rc.ch0 > 0)
			{
				Rc.ch0--;
			  if(Rc.ch0 < 0)Rc.ch0 = 0; 
			}
			else if(Rc.ch0 < 0)
			{
				Rc.ch0++;
			  if(Rc.ch0 > 0)Rc.ch0 = 0; 
			}
			
			Rc.x += (Rc.ch0 - Rc.x) * 0.01f;
			if(fabs(Rc.x) < 0.0001f)
			{
				Rc.x = 0.0f;
				Rc.x_flag = 0;
			}
			
			if(Rc.ch1 > 0)
			{
				Rc.ch1--;
			  if(Rc.ch0 < 0)Rc.ch1 = 0; 
			}
			else if(Rc.ch1 < 0)
			{
				Rc.ch1++;
			  if(Rc.ch1 > 0)Rc.ch1 = 0; 
			}
			Rc.z += (Rc.ch1 - Rc.z) * 0.1f;
			if(fabs(Rc.z) < 0.0001f)
			{
				Rc.z = 0.0f;
				Rc.z_flag = 0;
			}
		}
	}
}

