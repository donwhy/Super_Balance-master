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
 * ʵ��ƽ̨: 
 *     
 *
 * �޸�˵��:
 *     
**/

/* ����ͷ�ļ� -------------------------------------------------------*/
#include "FreeRTOS_demo.h"
#include "app.h"
/*FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"

/* ˽�����Ͷ��� -----------------------------------------------------*/
/* None. */

/* ˽�к궨�� -------------------------------------------------------*/
/*FreeRTOS����*/
/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */


/* ��ع��� ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define BATTERY_TASK_PRIO      				2                  		 /* �������ȼ� */
#define BATTERY_TASK_STK_SIZE  				256               		 /* �����ջ��С */
TaskHandle_t            							BatteryTask_Handler;   /* ������ */
void battery_task(void *pvParameters);            					 /* ������ */

/* У׼ ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define CALIBRATION_TASK_PRIO      		3                 	   /* �������ȼ� */
#define CALIBRATION_TASK_STK_SIZE  		256               	   /* �����ջ��С */
TaskHandle_t           						 		CalibrationTask_Handler;  /* ������ */
void calibration_task(void *pvParameters);           				  /* ������ */

/* ���� ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define CONTROL_TASK_PRIO      				4                  		 /* �������ȼ� */
#define CONTROL_TASK_STK_SIZE  				256               		 /* �����ջ��С */
TaskHandle_t            							ControlTask_Handler;   /* ������ */
void control_task(void *pvParameters);            					 /* ������ */

/* ͨ�� ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define COMMUNICATION_TASK_PRIO      	5                  		 /* �������ȼ� */
#define COMMUNICATION_TASK_STK_SIZE 	256                		 /* �����ջ��С */
TaskHandle_t           							 	CommunicationTask_Handler;  /* ������ */
void communication_task(void *pvParameters);            		 /* ������ */

/* LED ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO      	6                  					 		 /* �������ȼ� */
#define LED_TASK_STK_SIZE 	128                						   /* �����ջ��С */
TaskHandle_t           							 	LEDTask_Handler; 	 		 /* ������ */
void LED_task(void *pvParameters);            		 					 /* ������ */



/******************************************************************************************************/

/* ˽�б��� ---------------------------------------------------------*/
/* None. */
u8 seconds; // ���ʱ���������ڼ�¼������������

u16 RGB_Cnt = 0;    // RGB���Ƶļ�������
u8 RGB_Flag = 0;    // RGBģʽ��־��
u8 LowBattery = 0;  // �͵�ѹ��־��
u8 BatteryFlag = 0; // ��ر�����־��
u8 LD1, LD2, LD3;   // RGB�Ƶ����Ȳ�����
u32 RGB_LD;         // ���ڴ洢RGB�Ƶ���ɫ���ݡ�
u8 txdata[4];       // �洢��ESP32���͵ĵ�ѹ���ݡ�
u16 Volt16;         // ��ѹ���ݵ�16λ�洢������
//uint8_t task100ms;  // ÿ100msִ��һ������ļ�������

/* ��չ���� ---------------------------------------------------------*/
/* None. */

/* ˽�к���ԭ�� -----------------------------------------------------*/
/* None. */

/* ������ -----------------------------------------------------------*/
/*
 * ����: FreeRTOS������ں���
 * ����: None
 * ����: None
 * ˵��: None
 */
void freertos_demo(void)
{ 
    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ������������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */
    vTaskStartScheduler();
}

/*
 * ����: start_task
 * ����: pvParameters : �������(δ�õ�)
 * ����: None
 * ˵��: None
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* �����ٽ��� */
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
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}


// ��ع�������
void battery_task(void *pvParameters)//100msһ��
{
    while (1)
    {
        if (M1_Check_flag == 1 && M2_Check_flag == 1)
        {
          M1_Check_flag = M2_Check_flag = 0;
          Hardware_Tips(); // Ӳ�������ʾ��
        }

        if (usb_core_dev.conn_state == 0x03)
        {
          usb_ReadWrite(); // 100ms��ȡһ��USB����
        }
        Volt16 = (u16)(Car.BatVin_filter * 100.0f); // ��ȡ��ص�ѹֵ��ת��Ϊ������
        txdata[0] = Volt16 & 0xff;                  // ��ѹ��8λ��
        txdata[1] = Volt16 >> 8;                    // ��ѹ��8λ��
        UART1_SendDataDMA(txdata, 4);               // ÿ100msͨ��DMA���͵�ѹ���ݵ�ESP32��

        if (Car.BatVin_filter > 5.5f && Car.BatVin_filter < 7.0f) // ��ѹ����6.8V
        {
          LowBattery = 1; // ������־�����ڱ���
        }
        else
        {
          if (BatteryFlag == 1) // ����رշ�����
          {
            TMR10->c1dt = 0;
            LowBattery = 0; // ���õ͵�ѹ��־��
          }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // ��ʱ100ms
    }
}

// У׼����
void calibration_task(void *pvParameters)//1msһ��
{
    while (1)
    {
        if (Auto_Cali_Flag == 1)
        {
            Auto_Cali(); // �Զ�У׼
        }
        Car_Cali(); // ����У׼
				RcData();      // ��ȡң��������
        IMU_handle();  // ����IMU����
        vTaskDelay(100 / portTICK_PERIOD_MS); // ��ʱ100ms

    }
}

// ��������
void control_task(void *pvParameters)//1msһ��
{
    while (1)
    {
//        RcData();      // ��ȡң��������
//        IMU_handle();  // ����IMU����
        if (mpu6500.Cali_Flag == 1 && ++Car.M1_M2_Cnt >= 300)
        {
            Car.M1_M2_Cnt = 300;
            if (Car.Cali_flag == 1)
            {
                G_LED(4); // �̵�
                if (Car.Sensitivity > 25.0f)
                {
                    if (Car.Rc_Mode == 1)
                        B_LED(2);
                    else
                        B_LED(3);
                }
                else
                    B_LED(0);
                MotorControl(); // �������
            }
        }
        vTaskDelay(50 / portTICK_PERIOD_MS); // ��ʱ50ms
    }
}

// ͨ������
void communication_task(void *pvParameters)
{
    while (1)
    {
        if (Car.Print_flag == 1)
        {
            UsartPrint(); // ���ڴ�ӡ
        }
        Key_Scan();       // ����ɨ��
        Get_InputVolt();  // ��ȡ�����ѹ
        vTaskDelay(100 / portTICK_PERIOD_MS); // ��ʱ100ms
    }
}

// LED����
void LED_task(void *pvParameters)
{
    while (1)
    {
			
			
			
        vTaskDelay(100 / portTICK_PERIOD_MS); // ��ʱ100ms
    }
}
/******************** (C) Copyright nbufe ( End Of File ) ********************/
