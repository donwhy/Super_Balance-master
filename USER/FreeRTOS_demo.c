/*-----------------------------------------------------------------------------
 *      (C) Copyright 2023-2032, nbufe. All Rights Reserved 
 *-----------------------------------------------------------------------------
 * File name   : freertos.c
 * Description :
 * Author      : nbufe
 * Date        : 2023-09-14
 *-----------------------------------------------------------------------------
 * Attention
 *
 * 实验平台: 
 *     
 *
 * 修改说明:
 *     
**/

/* 包含头文件 -------------------------------------------------------*/
#include "FreeRTOS_demo.h"
#include "app.h"
/*FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"

/* 私有类型定义 -----------------------------------------------------*/
/* None. */

/* 私有宏定义 -------------------------------------------------------*/
/*FreeRTOS配置*/
/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */


/* 电池管理 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define BATTERY_TASK_PRIO      				2                  		 /* 任务优先级 */
#define BATTERY_TASK_STK_SIZE  				256               		 /* 任务堆栈大小 */
TaskHandle_t            							BatteryTask_Handler;   /* 任务句柄 */
void battery_task(void *pvParameters);            					 /* 任务函数 */

/* 校准 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define CALIBRATION_TASK_PRIO      		3                 	   /* 任务优先级 */
#define CALIBRATION_TASK_STK_SIZE  		256               	   /* 任务堆栈大小 */
TaskHandle_t           						 		CalibrationTask_Handler;  /* 任务句柄 */
void calibration_task(void *pvParameters);           				  /* 任务函数 */

/* 控制 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define CONTROL_TASK_PRIO      				4                  		 /* 任务优先级 */
#define CONTROL_TASK_STK_SIZE  				256               		 /* 任务堆栈大小 */
TaskHandle_t            							ControlTask_Handler;   /* 任务句柄 */
void control_task(void *pvParameters);            					 /* 任务函数 */

/* 通信 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define COMMUNICATION_TASK_PRIO      	5                  		 /* 任务优先级 */
#define COMMUNICATION_TASK_STK_SIZE 	256                		 /* 任务堆栈大小 */
TaskHandle_t           							 	CommunicationTask_Handler;  /* 任务句柄 */
void communication_task(void *pvParameters);            		 /* 任务函数 */

/* LED 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO      	6                  					 		 /* 任务优先级 */
#define LED_TASK_STK_SIZE 	128                						   /* 任务堆栈大小 */
TaskHandle_t           							 	LEDTask_Handler; 	 		 /* 任务句柄 */
void LED_task(void *pvParameters);            		 					 /* 任务函数 */



/******************************************************************************************************/

/* 私有变量 ---------------------------------------------------------*/
/* None. */
u8 seconds; // 秒计时变量，用于记录经过的秒数。

u16 RGB_Cnt = 0;    // RGB控制的计数器。
u8 RGB_Flag = 0;    // RGB模式标志。
u8 LowBattery = 0;  // 低电压标志。
u8 BatteryFlag = 0; // 电池报警标志。
u8 LD1, LD2, LD3;   // RGB灯的亮度参数。
u32 RGB_LD;         // 用于存储RGB灯的颜色数据。
u8 txdata[4];       // 存储向ESP32发送的电压数据。
u16 Volt16;         // 电压数据的16位存储变量。
//uint8_t task100ms;  // 每100ms执行一次任务的计数器。

/* 扩展变量 ---------------------------------------------------------*/
/* None. */

/* 私有函数原形 -----------------------------------------------------*/
/* None. */

/* 函数体 -----------------------------------------------------------*/
/*
 * 功能: FreeRTOS例程入口函数
 * 参数: None
 * 返回: None
 * 说明: None
 */
void freertos_demo(void)
{ 
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();
}

/*
 * 功能: start_task
 * 参数: pvParameters : 传入参数(未用到)
 * 返回: None
 * 说明: None
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
		xTaskCreate((TaskFunction_t )battery_task,
                (const char*    )"battery_task",
                (uint16_t       )BATTERY_TASK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )BATTERY_TASK_PRIO,
                (TaskHandle_t*  )&BatteryTask_Handler);

    xTaskCreate((TaskFunction_t )calibration_task,
                (const char*    )"calibration_task",
                (uint16_t       )CALIBRATION_TASK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CALIBRATION_TASK_PRIO,
                (TaskHandle_t*  )&CalibrationTask_Handler);

    xTaskCreate((TaskFunction_t )control_task,
                (const char*    )"control_task",
                (uint16_t       )CONTROL_TASK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CONTROL_TASK_PRIO,
                (TaskHandle_t*  )&ControlTask_Handler);

    xTaskCreate((TaskFunction_t )communication_task,
                (const char*    )"communication_task",
                (uint16_t       )COMMUNICATION_TASK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )COMMUNICATION_TASK_PRIO,
                (TaskHandle_t*  )&CommunicationTask_Handler);
								
		xTaskCreate((TaskFunction_t )LED_task,
                (const char*    )"LED_task",
                (uint16_t       )LED_TASK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}


// 电池管理任务
void battery_task(void *pvParameters)//100ms一次
{
    while (1)
    {
        if (M1_Check_flag == 1 && M2_Check_flag == 1)
        {
          M1_Check_flag = M2_Check_flag = 0;
          Hardware_Tips(); // 硬件检查提示。
        }

        if (usb_core_dev.conn_state == 0x03)
        {
          usb_ReadWrite(); // 100ms读取一次USB数据
        }
        Volt16 = (u16)(Car.BatVin_filter * 100.0f); // 获取电池电压值并转换为整数。
        txdata[0] = Volt16 & 0xff;                  // 电压低8位。
        txdata[1] = Volt16 >> 8;                    // 电压高8位。
        UART1_SendDataDMA(txdata, 4);               // 每100ms通过DMA发送电压数据到ESP32。

        if (Car.BatVin_filter > 5.5f && Car.BatVin_filter < 7.0f) // 电压低于6.8V
        {
          LowBattery = 1; // 产生标志，用于报警
        }
        else
        {
          if (BatteryFlag == 1) // 否则关闭蜂鸣器
          {
            TMR10->c1dt = 0;
            LowBattery = 0; // 重置低电压标志。
          }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // 延时100ms
    }
}

// 校准任务
void calibration_task(void *pvParameters)//1ms一次
{
    while (1)
    {
        if (Auto_Cali_Flag == 1)
        {
            Auto_Cali(); // 自动校准
        }
        Car_Cali(); // 车辆校准
				RcData();      // 获取遥控器数据
        IMU_handle();  // 处理IMU数据
        vTaskDelay(100 / portTICK_PERIOD_MS); // 延时100ms

    }
}

// 控制任务
void control_task(void *pvParameters)//1ms一次
{
    while (1)
    {
//        RcData();      // 获取遥控器数据
//        IMU_handle();  // 处理IMU数据
        if (mpu6500.Cali_Flag == 1 && ++Car.M1_M2_Cnt >= 300)
        {
            Car.M1_M2_Cnt = 300;
            if (Car.Cali_flag == 1)
            {
                G_LED(4); // 绿灯
                if (Car.Sensitivity > 25.0f)
                {
                    if (Car.Rc_Mode == 1)
                        B_LED(2);
                    else
                        B_LED(3);
                }
                else
                    B_LED(0);
                MotorControl(); // 电机控制
            }
        }
        vTaskDelay(50 / portTICK_PERIOD_MS); // 延时50ms
    }
}

// 通信任务
void communication_task(void *pvParameters)
{
    while (1)
    {
        if (Car.Print_flag == 1)
        {
            UsartPrint(); // 串口打印
        }
        Key_Scan();       // 按键扫描
        Get_InputVolt();  // 获取输入电压
        vTaskDelay(100 / portTICK_PERIOD_MS); // 延时100ms
    }
}

// LED任务
void LED_task(void *pvParameters)
{
    while (1)
    {
			
			
			
        vTaskDelay(100 / portTICK_PERIOD_MS); // 延时100ms
    }
}
/******************** (C) Copyright nbufe ( End Of File ) ********************/
