#ifndef __Control_H
#define __Control_H
#include "app.h"

union sFaultBitTypeDef
{
	struct
	{
		uint8_t IMU : 1;
		uint8_t flash : 1;
		uint8_t Encoder : 1;
		uint8_t voltage : 1;

		uint8_t M1_Encoder : 1;
		uint8_t M2_Encoder : 1;
		uint8_t M1_Current : 1;
		uint8_t M2_Current : 1;

		uint8_t rev3; // Ԥ��
		uint8_t rev4; // Ԥ��
		uint8_t rev5; // Ԥ��
		uint8_t rev6; // Ԥ��

		uint8_t rev7;  // Ԥ��
		uint8_t rev8;  // Ԥ��
		uint8_t rev9;  // Ԥ��
		uint8_t rev10; // Ԥ��
	} bit;
	uint16_t all;
};

extern volatile union sFaultBitTypeDef FaultBit;

typedef struct _PIDFloat_Obj_
{
	volatile float Kp; // 比例系数，用于调节当前误差的响应强度
	volatile float Ki; // 积分系数，用于消除系统稳态误差
	volatile float Kd; // 微分系数，用于抑制误差变化率

	volatile float Ki_Out; // 当前积分项的输出，用于累积积分结果

	volatile float Kp_Min; // 比例项的最小输出限制
	volatile float Kp_Max; // 比例项的最大输出限制

	volatile float Ki_Min; // 积分项的最小输出限制
	volatile float Ki_Max; // 积分项的最大输出限制

	volatile float Kd_Min; // 微分项的最小输出限制
	volatile float Kd_Max; // 微分项的最大输出限制

	volatile float outMin; // PID控制器总输出的最小值
	volatile float outMax; // PID控制器总输出的最大值

	volatile float PID_Out; // PID控制器的总输出结果
} PID_Structure;

typedef struct
{
	uint16_t data;			 // 当前编码器的原始数据
	uint8_t Spd_Offset_Flag; // 速度偏移标志位，用于校正编码器速度
	uint8_t rev;			 // 保留位（可用于扩展）
	int32_t DiffeData;		 // 当前编码器的差分数据（增量编码器的增量）
	uint16_t UpData;		 // 上一次编码器的原始数据
	float angle;			 // 当前编码器对应的旋转角度
	float Speed;			 // 当前编码器测量的速度
	float Speed_filter;		 // 编码器速度的滤波值（去噪处理后）
	float Speed_filterA;	 // 另一种滤波处理后的速度值（可能用于特定需求）
} Encoder_t;

typedef struct
{
	float BatVin;		 // 电池电压实时测量值（单位：伏特）
	float BatVin_filter; // 滤波后的电池电压（平滑处理结果）
	float ControlY;		 // 小车前后控制量（通常与速度相关）
	float ControlZ;		 // 小车旋转控制量（通常与转向相关）
	float Speed;		 // 小车速度（可能由编码器计算得出）
	float Sensitivity;	 // 小车控制的灵敏度（影响响应速度）
	uint16_t M1_M2_Cnt;	 // M1和M2电机的计数值，用于同步或其他目的

	uint8_t Cali_flag;	  // 校准标志位（1表示需要校准，0表示校准完成）
	uint8_t Cali_mode;	  // 当前校准模式（用于选择不同的校准方式）
	uint8_t Cali_error;	  // 校准错误标志（1表示校准出错）
	uint8_t Protect_flag; // 小车保护模式标志（1表示进入保护模式）
	uint8_t Rc_Mode;	  // 遥控模式标志（选择手动控制或自动模式）
	uint8_t Protect;	  // 小车保护状态标志（例如防止倾倒）

	uint8_t ProtectCnt;	 // 保护模式计数器（用于计时保护模式的持续时间）
	uint8_t PowerFlag;	 // 电源标志（1表示电源正常，0表示断电）
	uint8_t AutoStandUp; // 自动站立模式标志（1表示开启自动站立功能）
	uint8_t Print_flag;	 // 打印状态标志（1表示开启打印调试信息）
} Car_t;

extern PID_Structure UprightPID;
extern PID_Structure GyroAnglePID;
extern PID_Structure M1SpeedPID;
extern PID_Structure M2SpeedPID;
extern PID_Structure M1CurrentIdPID;
extern PID_Structure M1CurrentIqPID;
extern PID_Structure M2CurrentIdPID;
extern PID_Structure M2CurrentIqPID;
extern Car_t Car;

extern uint8_t M1_Check_cnt, M2_Check_cnt, M1_Check_flag, M2_Check_flag;
extern Encoder_t M1_Encoder;
extern Encoder_t M2_Encoder;

extern float CarSpeed;
extern float ControlOut_L, ControlOut_R;
extern float Set_Speed1, Set_Speed2;

void parameters_Init(void);

float PID_Adjust_T(PID_Structure *handle, float Given, float Feedback, float Gyro);
float PID_Adjust(PID_Structure *handle, float Given, float Feedback);
void M1SpeedControl(float speed);
void M2SpeedControl(float speed);
void M1Current_ClosedLoop(float Current); // ������
void M2Current_ClosedLoop(float Current); // ������

void MotorControl(void);
void SpeedControl(void);
void AngleControl(void);
void SpeedCompute(Encoder_t *Speed, uint16_t Encoder_data);
#endif
