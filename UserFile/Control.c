
#include "app.h"

PID_Structure_t SpeedPID;	// 小车整体速度环的PID结构体
PID_Structure TurnPID;		// 小车旋转控制的PID结构体
PID_Structure UprightPID;	// 小车直立控制的PID结构体
PID_Structure GyroAnglePID; // 陀螺仪角度的PID结构体

PID_Structure M1SpeedPID; // M1电机速度环的PID结构体
PID_Structure M2SpeedPID; // M2电机速度环的PID结构体

PID_Structure M1CurrentIdPID; // M1电机d轴电流环的PID结构体
PID_Structure M1CurrentIqPID; // M1电机q轴电流环的PID结构体

PID_Structure M2CurrentIdPID; // M2电机d轴电流环的PID结构体
PID_Structure M2CurrentIqPID; // M2电机q轴电流环的PID结构体

Encoder_t M1_Encoder; // M1电机的编码器结构体
Encoder_t M2_Encoder; // M2电机的编码器结构体

Car_t Car; // 小车的综合状态结构体

volatile union sFaultBitTypeDef FaultBit; // 故障标志位，用于存储各种故障状态

#define CurrentLimit 8.0f	 // 电流限制为8A，保护电机和电源
#define IqId 1.0f			 // q轴和d轴电流最大值，定义为1.0
#define UprightLimit 6000.0f // 直立控制输出限制（单位：度/秒）
#define TurnLimit 3000.0f	 // 旋转控制输出限制（单位：度/秒）
#define SpeedLimit 5000.0f	 // 速度环控制输出限制（单位：度/秒）

void parameters_Init(void) //
{
	/*************************************************************/
	// 初始化速度环PID参数
	SpeedPID.Kp = 1.25f;						 // 比例系数
	SpeedPID.Ki = 0.002f;						 // 积分系数
	SpeedPID.Kd = 0.0f;							 // 微分系数
	
	SpeedPID.Ki_Out = 0.0f;						 // 积分项初始化
	
	SpeedPID.Kp_Min = -SpeedLimit;				 // 比例输出最小值
	SpeedPID.Kp_Max = SpeedLimit;				 // 比例输出最大值
	
	SpeedPID.Kd_Min = -SpeedLimit;				 // 微分输出最小值
	SpeedPID.Kd_Max = SpeedLimit;				 // 微分输出最大值
	
	SpeedPID.Ki_Min = -SpeedLimit / SpeedPID.Ki; // 积分输出最小值
	SpeedPID.Ki_Max = SpeedLimit / SpeedPID.Ki;	 // 积分输出最大值
	
	SpeedPID.outMin = -SpeedLimit / SpeedPID.Ki; // PID总输出最小值
	SpeedPID.outMax = SpeedLimit / SpeedPID.Ki;	 // PID总输出最大值
	
	SpeedPID.PID_Out = 0.0f;					 // PID输出初始化
	SpeedPID.OutLimit = SpeedLimit;				 // PID输出限制值

	/*************************************************************/

	/*************************************************************/
	// 初始化旋转环PID参数
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
	// 初始化直立环PID参数
	UprightPID.Kp = 80.0f; // 比例增益设置较大，用于快速校正倾角
	UprightPID.Ki = 0.0f;  // 不使用积分项，防止积分饱和
	UprightPID.Kd = 15.0f; // 微分增益，用于抑制倾斜变化率
	
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
	// 初始化M1速度环PID参数
	M1SpeedPID.Kp = 0.005f;
	M1SpeedPID.Ki = M1SpeedPID.Kp / 50.0f; // 根据比例系数计算积分系数
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

	Car.Sensitivity = 20.0f; // 默认遥控手感
	Car.Protect_flag = 1;
	Car.BatVin_filter = 8.2f;
}

float ControlOut_L = 0.0f, ControlOut_R = 0.0f;

u8 CarSpeedCnt = 0;

float testfloat = 45.0f;

void MotorControl(void)
{
	// 判断是否处于平衡控制状态
	if (fabs(mpu6500.gyroAngle.y) < testfloat)
	{
		if (Car.AutoStandUp == 1) // 如果是自动站立模式
		{
			Car.AutoStandUp = 0;
			testfloat = 2.0f;	  // 调整允许的倾角
			Car.Protect_flag = 0; // 取消保护模式
		}

		Car.Protect = 1; // 标记小车处于平衡状态
		// 检查是否需要退出保护模式
		if (Car.Protect_flag == 1) // 如果是保护模式
		{
			if (fabsf(mpu6500.gyro.filter.x) < 1.5f && fabsf(mpu6500.gyro.filter.y) < 1.5f && fabsf(mpu6500.gyro.filter.z) < 5.0f) // 检测陀螺角速度和小车姿态
			{
				if (++Car.ProtectCnt >= 100) // 等待100次
				{
					Car.ProtectCnt = 0;
					Car.Protect_flag = 0;	// 进入平衡// 退出保护模式，恢复平衡
					M1_Foc.Cali_Status = 0; // 电机标记为正常闭环状态
					M2_Foc.Cali_Status = 0;
				}
			}
			else
				Car.ProtectCnt = 0; // 重置计数器
		}
	}
	// 如果不处于保护模式且电源正常
	if (Car.Protect_flag == 0 && Car.PowerFlag == 1)
	{
		if (++CarSpeedCnt >= 2) // 2ms闭环一次
		{
			CarSpeedCnt = 0;
			if (Car.Protect == 1)
			{
				// 更新速度和旋转控制量
				Car.Speed = (M1_Encoder.Speed_filterA - M2_Encoder.Speed_filterA); // 获小车速度

				Car.ControlY = Rc.x * Car.Sensitivity; // 前后控制量
				Car.ControlZ = Rc.z * Car.Sensitivity; // 旋转控制量

				if (fabsf(M1_Encoder.Speed_filterA) > 3600.0f || fabsf(M2_Encoder.Speed_filterA) > 3600.0f) // 小车车轮转速达到3600度/秒
				{
					if (Rc.x_flag == 0 && Rc.z_flag == 0) // 如果遥控器没有控制
					{
						if (mpu6500.acc.filter.z > 1.025f)
							Car.Protect_flag = 1; // 如果检测到小车被拿起则进入保护模式
					}
				}

				PID_Adjust_S(&SpeedPID, 0.0f, Car.Speed, Car.ControlY);				  // 小车速度环
				PID_Adjust_T(&TurnPID, Car.ControlZ, 0.0f, mpu6500.gyro.Primitive.z); // 小车旋转环
			}
		}
		// 直立控制环
		PID_Adjust_T(&UprightPID, 0.0f, -mpu6500.gyroAngle.y, mpu6500.gyro.Primitive.y); // 小车直立环+陀螺角度
		// 计算左右电机输出
		ControlOut_L = UprightPID.PID_Out + SpeedPID.PID_Out + TurnPID.PID_Out; // 输出电机速度控制量
		ControlOut_R = UprightPID.PID_Out + SpeedPID.PID_Out - TurnPID.PID_Out; // 输出电机速度控制量
	}
	else if (Car.Protect_flag == 1) // 保护模式电机停转
	{
		if (Car.AutoStandUp == 1) // 直接控制电机转速进行起立后闭环，需要注意的是轮胎与地面摩擦力一定要足够大，要不然会打滑并发生意想不到的情况
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
			else // 如果小车是倒立水平放置，则加大电机速度提高反扭力保证小车能顺利起立
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

			if (fabsf(M1_Encoder.Speed_filter) < 5.0f && fabsf(M2_Encoder.Speed_filter) < 5.0f) // 转速小于5
			{
				M1_Foc.Cali_Status = 2; // 电机进入停止
				M2_Foc.Cali_Status = 2; // 电机进入停止
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

	if (FaultBit.bit.IMU == 1 || FaultBit.bit.M1_Current == 1 || FaultBit.bit.M2_Current == 1) // IMU错误
	{
		ControlOut_L = 0.0f;
		ControlOut_R = 0.0f;
		M1_Foc.Cali_Status = 2; // 电机进入停止
		M2_Foc.Cali_Status = 2; // 电机进入停止
	}
}

u8 M1SpeedCnt = 0;
u8 M2SpeedCnt = 0;

float Set_Speed1 = 0.0f;
float Set_Speed2 = 0.0f;

uint8_t M1_Id_cnt, M2_Id_cnt;

void M1_Control(void)
{
	if (M1_Foc.Cali_Status == 0) // 正常闭环
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
					FaultBit.bit.M1_Current = 1; // 正常情况Id接近0
				}
			}
			else
				M1_Id_cnt = 0;
		}
	}
	else if (M1_Foc.Cali_Status == 2) // 电机停止
	{
		M1_Foc.Vd = 0.0f;
		M1_Foc.Vq = 0.0f;
	}
	else if (M1_Foc.Cali_Status == 1) // 电机校准后转动
	{
		if (Set_Speed1 < 600.0f)
		{
			Set_Speed1 += 0.1f;
		}
		M1SpeedControl(Set_Speed1);
		M1Current_ClosedLoop(M1SpeedPID.PID_Out);
	}
	else if (M1_Foc.Cali_Status == 3) // 计算速度
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
	if (M2_Foc.Cali_Status == 0) // 正常闭环
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
					FaultBit.bit.M2_Current = 1; // 正常情况Id接近0
				}
			}
			else
				M2_Id_cnt = 0;
		}
	}
	else if (M2_Foc.Cali_Status == 2) // 电机停止
	{
		M2_Foc.Vd = 0.0f;
		M2_Foc.Vq = 0.0f;
	}
	else if (M2_Foc.Cali_Status == 1) // 电机校准后转动
	{
		if (Set_Speed2 < 600.0f)
		{
			Set_Speed2 += 0.1f;
		}
		M2SpeedControl(Set_Speed2);
		M2Current_ClosedLoop(M2SpeedPID.PID_Out);
	}
	else if (M2_Foc.Cali_Status == 3) // 计算速度
	{
		if (++M2SpeedCnt >= 2)
		{
			M2SpeedCnt = 0;
			SpeedCompute(&M2_Encoder, M2_Foc.Encoder_data);
		}
	}
}

void SpeedCompute(Encoder_t *Speed, uint16_t Encoder_data) // 计算速度
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

void M1SpeedControl(float speed) // 速度环 200us
{
	if (++M1SpeedCnt >= 2)
	{
		M1SpeedCnt = 0;
		SpeedCompute(&M1_Encoder, M1_Foc.Encoder_data);
		PID_Adjust(&M1SpeedPID, speed, M1_Encoder.Speed_filter);
	}
}

void M1Current_ClosedLoop(float Current) // 电流环 100us
{
	PID_Adjust(&M1CurrentIdPID, 0.0f, M1_Foc.Id);
	M1_Foc.Vd = M1CurrentIdPID.PID_Out;

	PID_Adjust(&M1CurrentIqPID, Current, M1_Foc.Iq);
	M1_Foc.Vq = M1CurrentIqPID.PID_Out;
}

void M2SpeedControl(float speed) // 速度环 200us
{
	if (++M2SpeedCnt >= 2)
	{
		M2SpeedCnt = 0;
		SpeedCompute(&M2_Encoder, M2_Foc.Encoder_data);
		PID_Adjust(&M2SpeedPID, speed, M2_Encoder.Speed_filter);
	}
}

void M2Current_ClosedLoop(float Current) // 电流环 100us
{
	PID_Adjust(&M2CurrentIdPID, 0.0f, M2_Foc.Id);
	M2_Foc.Vd = M2CurrentIdPID.PID_Out;

	PID_Adjust(&M2CurrentIqPID, Current, M2_Foc.Iq);
	M2_Foc.Vq = M2CurrentIqPID.PID_Out;
}

uint8_t M1_Check_cnt = 0;
uint8_t M1_Check_flag = 0;
uint16_t M1_Check_Timeout = 0;

void M1_Check(void) // 电机开环转自检会调用的判断函数
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

void M2_Check(void) // 电机开环转自检会调用的判断函数
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

// PID调整函数
float PID_Adjust_T(PID_Structure *handle, float Given, float Feedback, float Gyro)
{
    float Error_value; // 误差值
    float P_Out;       // 比例输出
    float D_Out;       // 微分输出

    // 1. 计算误差
    Error_value = Given - Feedback;

    // 2. 计算比例输出 (P项)
    P_Out = Error_value * handle->Kp;

    // 3. 累积积分输出 (I项)
    handle->Ki_Out += Error_value * handle->Ki;

    // 4. 计算微分输出 (D项)
    D_Out = Gyro * handle->Kd;

    // 5. 限制比例输出
    if (P_Out < handle->Kp_Min)
        P_Out = handle->Kp_Min;
    else if (P_Out > handle->Kp_Max)
        P_Out = handle->Kp_Max;

    // 6. 限制积分输出
    if (handle->Ki_Out < handle->Ki_Min)
        handle->Ki_Out = handle->Ki_Min;
    else if (handle->Ki_Out > handle->Ki_Max)
        handle->Ki_Out = handle->Ki_Max;

    // 7. 限制微分输出
    if (D_Out < handle->Kd_Min)
        D_Out = handle->Kd_Min;
    else if (D_Out > handle->Kd_Max)
        D_Out = handle->Kd_Max;

    // 8. 计算PID总输出
    handle->PID_Out = P_Out + handle->Ki_Out + D_Out;

    // 9. 限制总输出范围
    if (handle->PID_Out > handle->outMax)
    {
        handle->PID_Out = handle->outMax;
    }
    else if (handle->PID_Out < handle->outMin)
    {
        handle->PID_Out = handle->outMin;
    }

    // 10. 返回PID输出值
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
