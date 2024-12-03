#include "app.h"

CaliData_t CaliData;

uint8_t u8_Cali_Data[256]; 
uint8_t Protection = 0;

// ���س����ʹ�ô������ְ���˳��1-5����У׼���ɣ���ɷ��渳ֵ����У׼
// ���С����2��У׼�ģ�������������ݺ������ٽ���У׼��

// 0xA0 0xAA 0x01 0xB1 0x66 У׼������ƫ�����뱣֤С�����ܵ��κ���(���С����2��У׼�ģ�������������ݺ������ٽ���У׼)
// 0xA0 0xAA 0x02 0xB2 0x66 У׼����,���뱣֤������˳������ת�������и���,���ڷ���ָ������ᰴ���趨�ٶ�ת����Ϊ����(��תΪ�쳣)
// 0xA0 0xAA 0x03 0xB3 0x66 У׼�ҵ��,���뱣֤������˳������ת�������и���,���ڷ���ָ������ᰴ���趨�ٶ�ת����Ϊ����(��תΪ�쳣)
// 0xA0 0xAA 0x04 0xB4 0x66 �رյ��
// 0xA0 0xAA 0x05 0xB5 0x66 ��������,��������ǰ���ȹرյ��
// 0xA0 0xAA 0x06 0xB6 0x66 �������
// 0xA0 0xAA 0x07 0xB7 0x66 �������ڴ�ӡ

// 0xA0 0xAA 0x0B 0xBB 0x66 Ӳ���Լ�
// ((�������ع���ʹ�ÿ��ص�Դ6V��ѹ0.3A��������)�Լ�ǰ���������ñ�ȷ������3.3V��5V��12V��ѹ�Ƿ������������������ȷ����������װ���������װ��������,���������Ҳ������װ,������ߺͱ����������ӵ����ذ弴��,�������ع���ʹ�ÿ��ص�Դ8V��ѹ1.6A��������,��ֹӲ������ð��)

// 0xA0 0xAA 0x66 0x16 0x66 һ��У׼(���С����2��У׼�ģ�������������ݺ������ٽ���У׼)
// (С������ʹ�õ�ػ��߿��ص�Դ(�������Ľ��鹩��ʹ�ÿ��ص�Դ8V��ѹ1.6A��������),��С������ˮƽ����,���뱣֤С�����ܵ��κ���,У׼��ɻ��Զ���������)

void Car_Cali(void)
{
	uint8_t crc = 0;
	
	if(U3_IDLE_Flag == 1)
	{
		U3_IDLE_Flag = 0;
		if(Get_usart3_data[0] == 0xA0 && Get_usart3_data[1] == 0xAA && Get_usart3_data[4] == 0x66)
		{
			crc = Get_usart3_data[0] + Get_usart3_data[1] + Get_usart3_data[2] + Get_usart3_data[4];
			if(Get_usart3_data[3] == crc)
			{
				Get_usart3_data[4] = 0x99; // �ı��β						
				Car.Cali_mode = Get_usart3_data[2];
				if(Car.Cali_mode != 11 && Car.Cali_mode != 102)
				{
					UART3_SendDataDMA(Get_usart3_data,5); // ������ͬ����
					if(usb_core_dev.conn_state == 0x03) //�ж�USB���⴮���Ƿ�����
					{
						usb_vcp_send_data(&usb_core_dev, Get_usart3_data, 5);
					}
				}
			}
			memset(Get_usart3_data,0,sizeof(char)*5);
		}
	}
		
	switch (Car.Cali_mode)	
	{
		case 1:
			Gyro_Cali();
			Car.Cali_mode = 0;
			Protection = 0;
			break;
		case 2:	
			M1_Foc.Cali_flag = 2;
			Car.Cali_mode = 0;
			break;
		case 3:		
			M2_Foc.Cali_flag = 2;
			Car.Cali_mode = 0;
			break;
		case 4:		
			M1_Foc.Cali_flag = 0;
			M2_Foc.Cali_flag = 0;
			Car.Cali_mode = 0;
			break;
		case 5:		
			if(Protection == 0)
			{
				Protection = 1;
				Car.Cali_mode = 0;
				CaliData.M1_offset = M1_Foc.ThetaOffset;
				CaliData.M2_offset = M2_Foc.ThetaOffset;
				
				CaliData.M1_Flag = M1_Foc.Cali_Status;
				CaliData.M2_Flag = M2_Foc.Cali_Status;
				
				CaliData.x[0] = mpu6500.gyro.offset.x;
				CaliData.y[0] = mpu6500.gyro.offset.y;
				CaliData.z[0] = mpu6500.gyro.offset.z;
				
				CaliData.IMU_Flag = mpu6500.Cali_Flag;
				if(FaultBit.bit.flash == 0 && FaultBit.bit.IMU)Write_Cali_Data(); // ���Flash��MPU6500�����ͱ�������
			}
			break;
		case 6:		
			if(Protection == 0)
			{
				Protection = 1;
				Car.Cali_mode = 0;
				CaliData.M1_offset = 0;
				CaliData.M2_offset = 0;
				
				CaliData.M1_Flag = 0;
				CaliData.M2_Flag = 0;
				
				CaliData.x[0] = 0;
				CaliData.y[0] = 0;
				CaliData.z[0] = 0;
				
				CaliData.IMU_Flag = 0;
				
				M1_Foc.Cali_flag = 0;
				M2_Foc.Cali_flag = 0;
				Write_Cali_Data();
			}
			break;
		case 7:		
			Car.Print_flag = 1;
			Car.Cali_mode = 0;
			break;
		case 9:		
			M1_Foc.Cali_flag = 3;
			Car.Cali_mode = 0;
			break;
		case 10:		
			M2_Foc.Cali_flag = 3;
			Car.Cali_mode = 0;
			break;
		case 11:	
			M1_Foc.Cali_flag = 0;
			M2_Foc.Cali_flag = 0;
			M1_Check_cnt = 0;
			M2_Check_cnt = 0;
			Car.Cali_mode = 12;
		  Hardware_Status = 0;
		  Start_Check_Hardware();
			break;
		case 12:	
			M1_Foc.Cali_flag = 5;
			M2_Foc.Cali_flag = 5;
			M1_Check_cnt = 0;
			M2_Check_cnt = 0;
			Car.Cali_mode = 0;
			break;
		case 102:	
			Auto_Cali_Flag = 1; //�Զ�У׼��־
			Car.Cali_flag = 0;
			Car.M1_M2_Cnt = 0;
		  Set_Speed1 = 0.0f;
			Set_Speed2 = 0.0f;
			M1_Check_cnt = 0;
			M2_Check_cnt = 0;
			Car.Cali_mode = 0;
			mpu6500.Cali_Flag = 0;
			M1_Foc.Cali_Status = 0;
			M2_Foc.Cali_Status = 0;
			M1_Foc.Cali_flag = 0;
			M2_Foc.Cali_flag = 0;
		  FaultBit.bit.M1_Encoder = 0;
			FaultBit.bit.M2_Encoder = 0;
			FaultBit.bit.M1_Current = 0;
			FaultBit.bit.M2_Current = 0;
			Auto_Cali_count = 0;
		  Start_Auto_Cali();
			break;
	}
}

uint16_t Auto_Cali_cnt = 0;
uint16_t Auto_Cali_count = 0;
uint8_t Auto_Cali_Flag = 0;

void Auto_Cali(void)
{
	G_LED(2);
	B_LED(2);
	if(++Auto_Cali_count >= 3000)
	{
		Auto_Cali_count = 3000;
		if(mpu6500.Cali_Flag == 0)
		{
			Gyro_Cali();
			Hardware_Status = 0;
		}
		else if(mpu6500.Cali_Flag == 1) //�ȴ�MPU6500У׼���
		{
			if(M1_Foc.Cali_Status == 0)M1_Foc.Cali_flag = 2;         //��ʼУ׼M1���
			
			if(M1_Foc.Cali_Status == 1)   //�ȴ�M1���У׼���
			{
				if(M2_Foc.Cali_Status == 0)M2_Foc.Cali_flag = 2;       //��ʼУ׼M2���
			}
			if(M2_Foc.Cali_Status == 1)   //�ȴ�M2���У׼���
			{
				if(++Auto_Cali_cnt >= 5000) //�ȴ�5�뱣֤�ٶȵ���
				{
					if(M1_Encoder.Speed_filter > 300.0f && M2_Encoder.Speed_filter > 300.0f)
					{
						M1_Foc.Cali_flag = 0;
						M2_Foc.Cali_flag = 0;
						
						if(Protection == 0) // ��������
						{
							Protection = 1;
							Car.Cali_mode = 0;
							CaliData.M1_offset = M1_Foc.ThetaOffset;
							CaliData.M2_offset = M2_Foc.ThetaOffset;
							
							CaliData.M1_Flag = M1_Foc.Cali_Status;
							CaliData.M2_Flag = M2_Foc.Cali_Status;
							
							CaliData.x[0] = mpu6500.gyro.offset.x;
							CaliData.y[0] = mpu6500.gyro.offset.y;
							CaliData.z[0] = mpu6500.gyro.offset.z;
							
							CaliData.IMU_Flag = mpu6500.Cali_Flag;
							if(FaultBit.bit.flash == 0 && FaultBit.bit.IMU == 0)Write_Cali_Data(); // ���Flash��MPU6500�����ͱ�������
						}
					}
					else 
					{
						if(M1_Encoder.Speed_filter < 300.0f)FaultBit.bit.M1_Encoder = 1;
						if(M2_Encoder.Speed_filter < 300.0f)FaultBit.bit.M2_Encoder = 1;
						Car.Cali_error = 1;
					}
					
					if(M1_Foc.Id > 1.0f || M1_Foc.Iq > 1.0f) // 
					{
						FaultBit.bit.M1_Current = 1;
					}

					if(M2_Foc.Id > 1.0f || M2_Foc.Iq > 1.0f)
					{
						FaultBit.bit.M2_Current = 1;
					}
					
					Auto_Cali_cnt = 0;
					Auto_Cali_Flag = 0;
					M1_Foc.Cali_Status = 0;
					M2_Foc.Cali_Status = 0;
					G_LED(1);
	        B_LED(1);
					Auto_Cali_End();
				}
			}
		}
	}
}

/***********************************************************************************************************
*ԭ��: void Gyro_Cali(void)
*���ܣ���������ƫУ׼
*���ã�
*���룺
*�����
************************************************************************************************************/
void Gyro_Cali(void)
{
	mpu6500.gyro.offset.x = mpu6500.gyro.filter.x;
	mpu6500.gyro.offset.y = mpu6500.gyro.filter.y;
	mpu6500.gyro.offset.z = mpu6500.gyro.filter.z;
	mpu6500.Cali_Flag = 1;
}

/***********************************************************************************************************
*ԭ��: void Write_Cali_Data(void)
*���ܣ��������ݵ�Flash
*���ã�
*���룺
*�����
************************************************************************************************************/
void Write_Cali_Data(void)
{
	uint16_t i,len = 0;
	uint8_t *CaliDataT = (uint8_t *)&CaliData;
	
	CaliData.sumcheck = 0;
	CaliData.addcheck = 0;
	len = sizeof(CaliData)-2;
	for(i=0;i<len;i++)
	{
		CaliData.sumcheck += CaliDataT[i];
		CaliData.addcheck += CaliData.sumcheck;
	}	
	
	memcpy(u8_Cali_Data,&CaliData,sizeof(CaliData));
	W25Qxx_ErasePage(0);
	W25Qxx_WritePage(u8_Cali_Data,0,sizeof(CaliData));
}

/***********************************************************************************************************
*ԭ��: void Read_Cali_Data(void)
*���ܣ���ȡFlash����
*���ã�
*���룺
*�����
************************************************************************************************************/
void Read_Cali_Data(void)
{
	uint16_t i,len = 0;
	uint8_t sumcheck = 0,addcheck = 0;
	
	memset(&M1_Foc,0,sizeof(M1_Foc));
	memset(&M2_Foc,0,sizeof(M2_Foc));
	memset(&mpu6500,0,sizeof(mpu6500));
	
	W25Qxx_Read(u8_Cali_Data,0,sizeof(CaliData));
	
	memset(&CaliData,0,sizeof(CaliData));
	
	len = sizeof(CaliData)-2;
	for(i=0;i<len;i++)
	{
		sumcheck += u8_Cali_Data[i];
		addcheck += sumcheck;
	}	
	memcpy(&CaliData,u8_Cali_Data,sizeof(CaliData));
//	sumcheck = 0;
	if(CaliData.sumcheck == sumcheck && CaliData.addcheck == addcheck)
	{
		if(CaliData.M1_Flag == 1 && CaliData.M2_Flag == 1 && CaliData.IMU_Flag == 1)
		{
			Car.Cali_flag = 1;
			Cubli_Cali_Status = 1;
			
			M1_Foc.Cali_flag = 1;
			M2_Foc.Cali_flag = 1;
			
			if(CaliData.M1_Flag == 1)
			{
				M1_Foc.ThetaOffset = CaliData.M1_offset;
			}
			
			if(CaliData.M2_Flag == 1)
			{
				M2_Foc.ThetaOffset = CaliData.M2_offset;
			}
			
			if(CaliData.IMU_Flag == 1)
			{
				mpu6500.Cali_Flag = CaliData.IMU_Flag;
				mpu6500.gyro.offset.x = CaliData.x[0];
				mpu6500.gyro.offset.y = CaliData.y[0];
				mpu6500.gyro.offset.z = CaliData.z[0];
			}
		}
	}
	else 
	{
		memset(&CaliData,0,sizeof(CaliData));
		Car.Cali_flag = 0;
	}
}

