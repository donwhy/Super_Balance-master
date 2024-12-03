
#include "app.h"

PID_Structure_t SpeedPID;	// С�������ٶȻ���PID�ṹ��
PID_Structure TurnPID;		// С����ת���Ƶ�PID�ṹ��
PID_Structure UprightPID;	// С��ֱ�����Ƶ�PID�ṹ��
PID_Structure GyroAnglePID; // �����ǽǶȵ�PID�ṹ��

PID_Structure M1SpeedPID; // M1����ٶȻ���PID�ṹ��
PID_Structure M2SpeedPID; // M2����ٶȻ���PID�ṹ��

PID_Structure M1CurrentIdPID; // M1���d���������PID�ṹ��
PID_Structure M1CurrentIqPID; // M1���q���������PID�ṹ��

PID_Structure M2CurrentIdPID; // M2���d���������PID�ṹ��
PID_Structure M2CurrentIqPID; // M2���q���������PID�ṹ��

Encoder_t M1_Encoder; // M1����ı������ṹ��
Encoder_t M2_Encoder; // M2����ı������ṹ��

Car_t Car; // С�����ۺ�״̬�ṹ��

volatile union sFaultBitTypeDef FaultBit; // ���ϱ�־λ�����ڴ洢���ֹ���״̬

#define CurrentLimit 8.0f	 // ��������Ϊ8A����������͵�Դ
#define IqId 1.0f			 // q���d��������ֵ������Ϊ1.0
#define UprightLimit 6000.0f // ֱ������������ƣ���λ����/�룩
#define TurnLimit 3000.0f	 // ��ת����������ƣ���λ����/�룩
#define SpeedLimit 5000.0f	 // �ٶȻ�����������ƣ���λ����/�룩

void parameters_Init(void) //
{
	/*************************************************************/
	// ��ʼ���ٶȻ�PID����
	SpeedPID.Kp = 1.25f;						 // ����ϵ��
	SpeedPID.Ki = 0.002f;						 // ����ϵ��
	SpeedPID.Kd = 0.0f;							 // ΢��ϵ��
	
	SpeedPID.Ki_Out = 0.0f;						 // �������ʼ��
	
	SpeedPID.Kp_Min = -SpeedLimit;				 // ���������Сֵ
	SpeedPID.Kp_Max = SpeedLimit;				 // ����������ֵ
	
	SpeedPID.Kd_Min = -SpeedLimit;				 // ΢�������Сֵ
	SpeedPID.Kd_Max = SpeedLimit;				 // ΢��������ֵ
	
	SpeedPID.Ki_Min = -SpeedLimit / SpeedPID.Ki; // ���������Сֵ
	SpeedPID.Ki_Max = SpeedLimit / SpeedPID.Ki;	 // ����������ֵ
	
	SpeedPID.outMin = -SpeedLimit / SpeedPID.Ki; // PID�������Сֵ
	SpeedPID.outMax = SpeedLimit / SpeedPID.Ki;	 // PID��������ֵ
	
	SpeedPID.PID_Out = 0.0f;					 // PID�����ʼ��
	SpeedPID.OutLimit = SpeedLimit;				 // PID�������ֵ

	/*************************************************************/

	/*************************************************************/
	// ��ʼ����ת��PID����
	TurnPID.Kp = 1.0f;
	TurnPID.Ki = 0.0f;
	TurnPID.Kd = 5.0f;
	
	TurnPID.Ki_Out = 0.0f;
	
	TurnPID.Kp_Min = -TurnLimit;
	TurnPID.Kp_Max = TurnLimit;
	
	TurnPID.Kd_Min = -TurnLimit;
	TurnPID.Kd_Max = TurnLimit;
	
	TurnPID.Ki_Min = -TurnLimit;
	TurnPID.Ki_Max = TurnLimit;
	
	TurnPID.outMin = -TurnLimit;
	TurnPID.outMax = TurnLimit;
	
	TurnPID.PID_Out = 0.0f;
	/*************************************************************/

	/*************************************************************/
	// ��ʼ��ֱ����PID����
	UprightPID.Kp = 80.0f; // �����������ýϴ����ڿ���У�����
	UprightPID.Ki = 0.0f;  // ��ʹ�û������ֹ���ֱ���
	UprightPID.Kd = 15.0f; // ΢�����棬����������б�仯��
	
	UprightPID.Ki_Out = 0.0f;
	
	UprightPID.Kp_Min = -UprightLimit;
	UprightPID.Kp_Max = UprightLimit;
	
	UprightPID.Kd_Min = -UprightLimit;
	UprightPID.Kd_Max = UprightLimit;
	
	UprightPID.Ki_Min = -UprightLimit;
	UprightPID.Ki_Max = UprightLimit;
	
	UprightPID.outMin = -UprightLimit;
	UprightPID.outMax = UprightLimit;
	
	UprightPID.PID_Out = 0.0f;
	/*************************************************************/

	/*************************************************************/
	// ��ʼ��M1�ٶȻ�PID����
	M1SpeedPID.Kp = 0.005f;
	M1SpeedPID.Ki = M1SpeedPID.Kp / 50.0f; // ���ݱ���ϵ���������ϵ��
	M1SpeedPID.Kd = 0.0f;
	
	M1SpeedPID.Ki_Out = 0.0f;
	
	M1SpeedPID.Kp_Min = -CurrentLimit;
	M1SpeedPID.Kp_Max = CurrentLimit;
	
	M1SpeedPID.Kd_Min = -CurrentLimit;
	M1SpeedPID.Kd_Max = CurrentLimit;
	
	M1SpeedPID.Ki_Min = -CurrentLimit;
	M1SpeedPID.Ki_Max = CurrentLimit;
	
	M1SpeedPID.outMin = -CurrentLimit;
	M1SpeedPID.outMax = CurrentLimit;
	
	M1SpeedPID.PID_Out = 0.0f;
	/*************************************************************/
	M2SpeedPID.Kp = 0.005f;
	M2SpeedPID.Ki = M2SpeedPID.Kp / 50.0f;
	M2SpeedPID.Kd = 0.0f;

	M2SpeedPID.Ki_Out = 0.0f;

	M2SpeedPID.Kp_Min = -CurrentLimit;
	M2SpeedPID.Kp_Max = CurrentLimit;

	M2SpeedPID.Kd_Min = -CurrentLimit;
	M2SpeedPID.Kd_Max = CurrentLimit;

	M2SpeedPID.Ki_Min = -CurrentLimit;
	M2SpeedPID.Ki_Max = CurrentLimit;

	M2SpeedPID.outMin = -CurrentLimit;
	M2SpeedPID.outMax = CurrentLimit;

	M2SpeedPID.PID_Out = 0.0f;
	/*************************************************************/

	/*************************************************************/
	M1CurrentIdPID.Kp = 0.005f;
	M1CurrentIdPID.Ki = M1CurrentIdPID.Kp / 10.0f;
	M1CurrentIdPID.Kd = 0.0f;

	M1CurrentIdPID.Ki_Out = 0.0f;

	M1CurrentIdPID.Kp_Min = -IqId / 2.0f;
	M1CurrentIdPID.Kp_Max = IqId / 2.0f;

	M1CurrentIdPID.Kd_Min = -IqId / 2.0f;
	M1CurrentIdPID.Kd_Max = IqId / 2.0f;

	M1CurrentIdPID.Ki_Min = -IqId / 2.0f;
	M1CurrentIdPID.Ki_Max = IqId / 2.0f;

	M1CurrentIdPID.outMin = -IqId / 2.0f;
	M1CurrentIdPID.outMax = IqId / 2.0f;

	M1CurrentIdPID.PID_Out = 0.0f;
	/*************************************************************/
	M1CurrentIqPID.Kp = 0.01f;
	M1CurrentIqPID.Ki = M1CurrentIqPID.Kp / 10.0f;
	M1CurrentIqPID.Kd = 0.0f;

	M1CurrentIqPID.Ki_Out = 0.0f;

	M1CurrentIqPID.Kp_Min = -IqId;
	M1CurrentIqPID.Kp_Max = IqId;

	M1CurrentIqPID.Kd_Min = -IqId;
	M1CurrentIqPID.Kd_Max = IqId;

	M1CurrentIqPID.Ki_Min = -IqId;
	M1CurrentIqPID.Ki_Max = IqId;

	M1CurrentIqPID.outMin = -IqId;
	M1CurrentIqPID.outMax = IqId;

	M1CurrentIqPID.PID_Out = 0.0f;
	/*************************************************************/

	/*************************************************************/
	M2CurrentIdPID.Kp = 0.005f;
	M2CurrentIdPID.Ki = M2CurrentIdPID.Kp / 10.0f;
	M2CurrentIdPID.Kd = 0.0f;

	M2CurrentIdPID.Ki_Out = 0.0f;

	M2CurrentIdPID.Kp_Min = -IqId / 2.0f;
	M2CurrentIdPID.Kp_Max = IqId / 2.0f;

	M2CurrentIdPID.Kd_Min = -IqId / 2.0f;
	M2CurrentIdPID.Kd_Max = IqId / 2.0f;

	M2CurrentIdPID.Ki_Min = -IqId / 2.0f;
	M2CurrentIdPID.Ki_Max = IqId / 2.0f;

	M2CurrentIdPID.outMin = -IqId / 2.0f;
	M2CurrentIdPID.outMax = IqId / 2.0f;

	M2CurrentIdPID.PID_Out = 0.0f;
	/*************************************************************/
	M2CurrentIqPID.Kp = 0.01f;
	M2CurrentIqPID.Ki = M2CurrentIqPID.Kp / 10.0f;
	M2CurrentIqPID.Kd = 0.0f;

	M2CurrentIqPID.Ki_Out = 0.0f;

	M2CurrentIqPID.Kp_Min = -IqId;
	M2CurrentIqPID.Kp_Max = IqId;

	M2CurrentIqPID.Kd_Min = -IqId;
	M2CurrentIqPID.Kd_Max = IqId;

	M2CurrentIqPID.Ki_Min = -IqId;
	M2CurrentIqPID.Ki_Max = IqId;

	M2CurrentIqPID.outMin = -IqId;
	M2CurrentIqPID.outMax = IqId;

	M2CurrentIqPID.PID_Out = 0.0f;
	/*************************************************************/

	Car.Sensitivity = 20.0f; // Ĭ��ң���ָ�
	Car.Protect_flag = 1;
	Car.BatVin_filter = 8.2f;
}

float ControlOut_L = 0.0f, ControlOut_R = 0.0f;

u8 CarSpeedCnt = 0;

float testfloat = 45.0f;

void MotorControl(void)
{
	// �ж��Ƿ���ƽ�����״̬
	if (fabs(mpu6500.gyroAngle.y) < testfloat)
	{
		if (Car.AutoStandUp == 1) // ������Զ�վ��ģʽ
		{
			Car.AutoStandUp = 0;
			testfloat = 2.0f;	  // ������������
			Car.Protect_flag = 0; // ȡ������ģʽ
		}

		Car.Protect = 1; // ���С������ƽ��״̬
		// ����Ƿ���Ҫ�˳�����ģʽ
		if (Car.Protect_flag == 1) // ����Ǳ���ģʽ
		{
			if (fabsf(mpu6500.gyro.filter.x) < 1.5f && fabsf(mpu6500.gyro.filter.y) < 1.5f && fabsf(mpu6500.gyro.filter.z) < 5.0f) // ������ݽ��ٶȺ�С����̬
			{
				if (++Car.ProtectCnt >= 100) // �ȴ�100��
				{
					Car.ProtectCnt = 0;
					Car.Protect_flag = 0;	// ����ƽ��// �˳�����ģʽ���ָ�ƽ��
					M1_Foc.Cali_Status = 0; // ������Ϊ�����ջ�״̬
					M2_Foc.Cali_Status = 0;
				}
			}
			else
				Car.ProtectCnt = 0; // ���ü�����
		}
	}
	// ��������ڱ���ģʽ�ҵ�Դ����
	if (Car.Protect_flag == 0 && Car.PowerFlag == 1)
	{
		if (++CarSpeedCnt >= 2) // 2ms�ջ�һ��
		{
			CarSpeedCnt = 0;
			if (Car.Protect == 1)
			{
				// �����ٶȺ���ת������
				Car.Speed = (M1_Encoder.Speed_filterA - M2_Encoder.Speed_filterA); // ��С���ٶ�

				Car.ControlY = Rc.x * Car.Sensitivity; // ǰ�������
				Car.ControlZ = Rc.z * Car.Sensitivity; // ��ת������

				if (fabsf(M1_Encoder.Speed_filterA) > 3600.0f || fabsf(M2_Encoder.Speed_filterA) > 3600.0f) // С������ת�ٴﵽ3600��/��
				{
					if (Rc.x_flag == 0 && Rc.z_flag == 0) // ���ң����û�п���
					{
						if (mpu6500.acc.filter.z > 1.025f)
							Car.Protect_flag = 1; // �����⵽С������������뱣��ģʽ
					}
				}

				PID_Adjust_S(&SpeedPID, 0.0f, Car.Speed, Car.ControlY);				  // С���ٶȻ�
				PID_Adjust_T(&TurnPID, Car.ControlZ, 0.0f, mpu6500.gyro.Primitive.z); // С����ת��
			}
		}
		// ֱ�����ƻ�
		PID_Adjust_T(&UprightPID, 0.0f, -mpu6500.gyroAngle.y, mpu6500.gyro.Primitive.y); // С��ֱ����+���ݽǶ�
		// �������ҵ�����
		ControlOut_L = UprightPID.PID_Out + SpeedPID.PID_Out + TurnPID.PID_Out; // �������ٶȿ�����
		ControlOut_R = UprightPID.PID_Out + SpeedPID.PID_Out - TurnPID.PID_Out; // �������ٶȿ�����
	}
	else if (Car.Protect_flag == 1) // ����ģʽ���ͣת
	{
		if (Car.AutoStandUp == 1) // ֱ�ӿ��Ƶ��ת�ٽ���������ջ�����Ҫע�������̥�����Ħ����һ��Ҫ�㹻��Ҫ��Ȼ��򻬲��������벻�������
		{
			if (mpu6500.gyroAngle.y > 0.0f && mpu6500.gyroAngle.y < 175.0f)
			{
				ControlOut_L = 3600.0f;
				ControlOut_R = 3600.0f;
			}
			else if (mpu6500.gyroAngle.y < 0.0f && mpu6500.gyroAngle.y > -175.0f)
			{
				ControlOut_L = -3600.0f;
				ControlOut_R = -3600.0f;
			}
			else // ���С���ǵ���ˮƽ���ã���Ӵ����ٶ���߷�Ť����֤С����˳������
			{
				ControlOut_L = 7200.0f;
				ControlOut_R = 7200.0f;
			}
			M1_Foc.Cali_Status = 0;
			M2_Foc.Cali_Status = 0;
		}
		else if (Car.AutoStandUp == 0)
		{
			ControlOut_L = 0.0f;
			ControlOut_R = 0.0f;

			if (fabsf(M1_Encoder.Speed_filter) < 5.0f && fabsf(M2_Encoder.Speed_filter) < 5.0f) // ת��С��5
			{
				M1_Foc.Cali_Status = 2; // �������ֹͣ
				M2_Foc.Cali_Status = 2; // �������ֹͣ
			}

			Car.Speed = 0.0f;
			Car.Protect = 0;

			testfloat = 10.0f;

			M1_Encoder.Spd_Offset_Flag = 0;
			M2_Encoder.Spd_Offset_Flag = 0;

			UprightPID.Ki_Out = 0.0f;
			UprightPID.PID_Out = 0.0f;

			SpeedPID.Ki_Out = 0.0f;
			SpeedPID.PID_Out = 0.0f;

			TurnPID.Ki_Out = 0.0f;
			TurnPID.PID_Out = 0.0f;

			M1_Encoder.Speed_filterA = 0.0f;
			M2_Encoder.Speed_filterA = 0.0f;
		}
	}

	if (FaultBit.bit.IMU == 1 || FaultBit.bit.M1_Current == 1 || FaultBit.bit.M2_Current == 1) // IMU����
	{
		ControlOut_L = 0.0f;
		ControlOut_R = 0.0f;
		M1_Foc.Cali_Status = 2; // �������ֹͣ
		M2_Foc.Cali_Status = 2; // �������ֹͣ
	}
}

u8 M1SpeedCnt = 0;
u8 M2SpeedCnt = 0;

float Set_Speed1 = 0.0f;
float Set_Speed2 = 0.0f;

uint8_t M1_Id_cnt, M2_Id_cnt;

void M1_Control(void)
{
	if (M1_Foc.Cali_Status == 0) // �����ջ�
	{
		M1SpeedControl(-ControlOut_L);
		M1Current_ClosedLoop(M1SpeedPID.PID_Out);
		if (Car.Cali_flag == 1)
		{
			if (fabsf(M1_Foc.Id) > 1.0f)
			{
				if (++M1_Id_cnt >= 200)
				{
					M1_Id_cnt = 200;
					FaultBit.bit.M1_Current = 1; // �������Id�ӽ�0
				}
			}
			else
				M1_Id_cnt = 0;
		}
	}
	else if (M1_Foc.Cali_Status == 2) // ���ֹͣ
	{
		M1_Foc.Vd = 0.0f;
		M1_Foc.Vq = 0.0f;
	}
	else if (M1_Foc.Cali_Status == 1) // ���У׼��ת��
	{
		if (Set_Speed1 < 600.0f)
		{
			Set_Speed1 += 0.1f;
		}
		M1SpeedControl(Set_Speed1);
		M1Current_ClosedLoop(M1SpeedPID.PID_Out);
	}
	else if (M1_Foc.Cali_Status == 3) // �����ٶ�
	{
		if (++M1SpeedCnt >= 2)
		{
			M1SpeedCnt = 0;
			SpeedCompute(&M1_Encoder, M1_Foc.Encoder_data);
		}
	}
}

void M2_Control(void)
{
	if (M2_Foc.Cali_Status == 0) // �����ջ�
	{
		M2SpeedControl(ControlOut_R);
		M2Current_ClosedLoop(M2SpeedPID.PID_Out);
		if (Car.Cali_flag == 1)
		{
			if (fabsf(M2_Foc.Id) > 1.0f)
			{
				if (++M2_Id_cnt >= 200)
				{
					M2_Id_cnt = 200;
					FaultBit.bit.M2_Current = 1; // �������Id�ӽ�0
				}
			}
			else
				M2_Id_cnt = 0;
		}
	}
	else if (M2_Foc.Cali_Status == 2) // ���ֹͣ
	{
		M2_Foc.Vd = 0.0f;
		M2_Foc.Vq = 0.0f;
	}
	else if (M2_Foc.Cali_Status == 1) // ���У׼��ת��
	{
		if (Set_Speed2 < 600.0f)
		{
			Set_Speed2 += 0.1f;
		}
		M2SpeedControl(Set_Speed2);
		M2Current_ClosedLoop(M2SpeedPID.PID_Out);
	}
	else if (M2_Foc.Cali_Status == 3) // �����ٶ�
	{
		if (++M2SpeedCnt >= 2)
		{
			M2SpeedCnt = 0;
			SpeedCompute(&M2_Encoder, M2_Foc.Encoder_data);
		}
	}
}

void SpeedCompute(Encoder_t *Speed, uint16_t Encoder_data) // �����ٶ�
{
	Speed->data = Encoder_data;
	if (Speed->Spd_Offset_Flag == 0)
	{
		Speed->Spd_Offset_Flag = 1;
		Speed->UpData = Speed->data;
	}
	if (Speed->UpData > 32000 && Speed->data < 768)
	{
		Speed->DiffeData = (int32_t)(32768 + Speed->data) - Speed->UpData;
	}
	else if (Speed->UpData < 768 && Speed->data > 32000)
	{
		Speed->DiffeData = -((int32_t)(32768 - Speed->data) + Speed->UpData);
	}
	else
	{
		Speed->DiffeData = Speed->data - Speed->UpData;
	}
	Speed->UpData = Speed->data;

	Speed->Speed = (Speed->DiffeData * 0.010986328125f) / 0.0002f;
	Speed->Speed_filter += (Speed->Speed - Speed->Speed_filter) * 0.25f;
	Speed->Speed_filterA += (Speed->Speed_filter - Speed->Speed_filterA) * 0.025f;
}

void M1SpeedControl(float speed) // �ٶȻ� 200us
{
	if (++M1SpeedCnt >= 2)
	{
		M1SpeedCnt = 0;
		SpeedCompute(&M1_Encoder, M1_Foc.Encoder_data);
		PID_Adjust(&M1SpeedPID, speed, M1_Encoder.Speed_filter);
	}
}

void M1Current_ClosedLoop(float Current) // ������ 100us
{
	PID_Adjust(&M1CurrentIdPID, 0.0f, M1_Foc.Id);
	M1_Foc.Vd = M1CurrentIdPID.PID_Out;

	PID_Adjust(&M1CurrentIqPID, Current, M1_Foc.Iq);
	M1_Foc.Vq = M1CurrentIqPID.PID_Out;
}

void M2SpeedControl(float speed) // �ٶȻ� 200us
{
	if (++M2SpeedCnt >= 2)
	{
		M2SpeedCnt = 0;
		SpeedCompute(&M2_Encoder, M2_Foc.Encoder_data);
		PID_Adjust(&M2SpeedPID, speed, M2_Encoder.Speed_filter);
	}
}

void M2Current_ClosedLoop(float Current) // ������ 100us
{
	PID_Adjust(&M2CurrentIdPID, 0.0f, M2_Foc.Id);
	M2_Foc.Vd = M2CurrentIdPID.PID_Out;

	PID_Adjust(&M2CurrentIqPID, Current, M2_Foc.Iq);
	M2_Foc.Vq = M2CurrentIqPID.PID_Out;
}

uint8_t M1_Check_cnt = 0;
uint8_t M1_Check_flag = 0;
uint16_t M1_Check_Timeout = 0;

void M1_Check(void) // �������ת�Լ����õ��жϺ���
{
	if (++M1_Check_cnt >= 200)
	{
		M1_Check_cnt = 0;
		if (FaultBit.bit.M1_Current == 1)
			FaultBit.bit.M1_Current = 0;

		if (M1_Foc.Id < 2.0f || M1_Foc.Id > 3.2f)
		{
			FaultBit.bit.M1_Current = 1;
			M1_Check_flag = 1;
			M1_Foc.Cali_flag = 0;
			M1_Foc.Cali_Status = 0;
			M1_Check_Timeout = 0;
		}

		if (M1_Foc.Iq > 0.5f)
		{
			FaultBit.bit.M1_Current = 1;
			M1_Check_flag = 1;
			M1_Foc.Cali_flag = 0;
			M1_Foc.Cali_Status = 0;
			M1_Check_Timeout = 0;
		}
	}

	if (M1_Foc.theta >= 11.0f)
	{
		if (M1_Foc.Angle < 85.0f || M1_Foc.Angle > 95.0f)
			FaultBit.bit.M1_Encoder = 1;
		M1_Check_flag = 1;
		M1_Foc.Cali_flag = 0;
		M1_Foc.Cali_Status = 0;
		M1_Check_Timeout = 0;
	}

	if (++M1_Check_Timeout >= 30000)
	{
		M1_Check_Timeout = 0;
		M1_Check_flag = 1;
		M1_Foc.Cali_flag = 0;
		M1_Foc.Cali_Status = 0;
	}
}

uint8_t M2_Check_cnt = 0;
uint8_t M2_Check_flag = 0;
uint16_t M2_Check_Timeout = 0;

void M2_Check(void) // �������ת�Լ����õ��жϺ���
{
	if (++M2_Check_cnt >= 200)
	{
		M2_Check_cnt = 0;

		if (FaultBit.bit.M2_Current == 1)
			FaultBit.bit.M2_Current = 0;
		if (M2_Foc.Id < 2.0f || M2_Foc.Id > 3.2f)
		{
			FaultBit.bit.M2_Current = 1;
			M2_Check_flag = 1;
			M2_Foc.Cali_flag = 0;
			M2_Foc.Cali_Status = 0;
			M2_Check_Timeout = 0;
		}

		if (M2_Foc.Iq > 0.5f)
		{
			FaultBit.bit.M2_Current = 1;
			M2_Check_flag = 1;
			M2_Foc.Cali_flag = 0;
			M2_Foc.Cali_Status = 0;
			M2_Check_Timeout = 0;
		}
	}

	if (M2_Foc.theta >= 11.0f)
	{
		if (M2_Foc.Angle < 85.0f || M2_Foc.Angle > 95.0f)
			FaultBit.bit.M2_Encoder = 1;
		M2_Check_flag = 1;
		M2_Foc.Cali_flag = 0;
		M2_Foc.Cali_Status = 0;
		M2_Check_Timeout = 0;
	}

	if (++M2_Check_Timeout >= 30000)
	{
		M2_Check_Timeout = 0;
		M2_Check_flag = 1;
		M2_Foc.Cali_flag = 0;
		M2_Foc.Cali_Status = 0;
	}
}

// PID��������
float PID_Adjust_T(PID_Structure *handle, float Given, float Feedback, float Gyro)
{
    float Error_value; // ���ֵ
    float P_Out;       // �������
    float D_Out;       // ΢�����

    // 1. �������
    Error_value = Given - Feedback;

    // 2. ���������� (P��)
    P_Out = Error_value * handle->Kp;

    // 3. �ۻ�������� (I��)
    handle->Ki_Out += Error_value * handle->Ki;

    // 4. ����΢����� (D��)
    D_Out = Gyro * handle->Kd;

    // 5. ���Ʊ������
    if (P_Out < handle->Kp_Min)
        P_Out = handle->Kp_Min;
    else if (P_Out > handle->Kp_Max)
        P_Out = handle->Kp_Max;

    // 6. ���ƻ������
    if (handle->Ki_Out < handle->Ki_Min)
        handle->Ki_Out = handle->Ki_Min;
    else if (handle->Ki_Out > handle->Ki_Max)
        handle->Ki_Out = handle->Ki_Max;

    // 7. ����΢�����
    if (D_Out < handle->Kd_Min)
        D_Out = handle->Kd_Min;
    else if (D_Out > handle->Kd_Max)
        D_Out = handle->Kd_Max;

    // 8. ����PID�����
    handle->PID_Out = P_Out + handle->Ki_Out + D_Out;

    // 9. �����������Χ
    if (handle->PID_Out > handle->outMax)
    {
        handle->PID_Out = handle->outMax;
    }
    else if (handle->PID_Out < handle->outMin)
    {
        handle->PID_Out = handle->outMin;
    }

    // 10. ����PID���ֵ
    return handle->PID_Out;
}

float PID_Adjust(PID_Structure *handle, float Given, float Feedback)
{
	float Error_value;

	float P_Out;

	Error_value = Given - Feedback;

	P_Out = Error_value * handle->Kp;
	handle->Ki_Out += Error_value * handle->Ki;

	if (P_Out < handle->Kp_Min)
		P_Out = handle->Kp_Min;
	else if (P_Out > handle->Kp_Max)
		P_Out = handle->Kp_Max;

	if (handle->Ki_Out < handle->Ki_Min)
		handle->Ki_Out = handle->Ki_Min;
	else if (handle->Ki_Out > handle->Ki_Max)
		handle->Ki_Out = handle->Ki_Max;

	handle->PID_Out = P_Out + handle->Ki_Out;

	if (handle->PID_Out > handle->outMax)
	{
		handle->PID_Out = handle->outMax;
	}
	else if (handle->PID_Out < handle->outMin)
	{
		handle->PID_Out = handle->outMin;
	}
	return handle->PID_Out;
}
