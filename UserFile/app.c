
#include "app.h"


//extern u8 seconds; // 秒计时变量，用于记录经过的秒数。

//extern u16 RGB_Cnt = 0;    // RGB控制的计数器。
//extern u8 RGB_Flag = 0;    // RGB模式标志。
//extern u8 LowBattery = 0;  // 低电压标志。
//extern u8 BatteryFlag = 0; // 电池报警标志。
//extern u8 LD1, LD2, LD3;   // RGB灯的亮度参数。
//extern u32 RGB_LD;         // 用于存储RGB灯的颜色数据。
//extern u8 txdata[4];       // 存储向ESP32发送的电压数据。
//extern u16 Volt16;         // 电压数据的16位存储变量。
//extern uint8_t task100ms;  // 每100ms执行一次任务的计数器。

void Peripherals_Init(void)
{
  LED_Init();
  W25Qxx_Init();    // 初始化W25Qxx存储芯片。
  Read_Cali_Data(); // 读取校准数据。
  Gpio_config();
  Key_Init();
  Check_Encoder(); // 检查编码器模块。
  TIM10_Init();
  TIM7_Init();
  usb_init();          // 初始化USB模块。
  USART1_init(921600); // ESP32通信
  USART2_init(100000); // SUBUS通信
  USART3_init(230400); // 串口打印
  USART4_init(115200); // 串口打印
  IMU_Init();          // 初始化IMU
  delay_ms(10);        // delay_ms(500); delay_ms(500); delay_ms(500);
  TIM1_Init();
  TIM8_Init();
  ADC_Config();

  parameters_Init();  	// 初始化各种参数。

  delay_ms(10);
  WS2812_init(); 		 	  // 初始化WS2812 RGB LED模块。
	
	freertos_demo();      // 运行FreeRTOS例程
	
	
//  while (1)
//  {

//    if (TIM_Flag == 1)
//    {
//      TIM_Flag = 0;

//      if (++task100ms >= 100)
//      {
//        task100ms = 0;

//        if (M1_Check_flag == 1 && M2_Check_flag == 1)
//        {
//          M1_Check_flag = M2_Check_flag = 0;
//          Hardware_Tips(); // 硬件检查提示。
//        }

//        if (usb_core_dev.conn_state == 0x03)
//        {
//          usb_ReadWrite(); // 100ms读取一次USB数据
//        }
//        Volt16 = (u16)(Car.BatVin_filter * 100.0f); // 获取电池电压值并转换为整数。
//        txdata[0] = Volt16 & 0xff;                  // 电压低8位。
//        txdata[1] = Volt16 >> 8;                    // 电压高8位。
//        UART1_SendDataDMA(txdata, 4);               // 每100ms通过DMA发送电压数据到ESP32。

//        if (Car.BatVin_filter > 5.5f && Car.BatVin_filter < 7.0f) // 电压低于6.8V
//        {
//          LowBattery = 1; // 产生标志，用于报警
//        }
//        else
//        {
//          if (BatteryFlag == 1) // 否则关闭蜂鸣器
//          {
//            TMR10->c1dt = 0;
//            LowBattery = 0; // 重置低电压标志。
//          }
//        }
//      }

//      if (Auto_Cali_Flag == 1)
//      {
//        Auto_Cali(); // 执行自动校准。
//      }

//      Car_Cali();   // 车辆校准。
//      RcData();     // 获取遥控器数据。
//      IMU_handle(); // 处理IMU数据。

//      if (mpu6500.Cali_Flag == 1)
//      {
//        if (++Car.M1_M2_Cnt >= 300) // 等待电机初始化
//        {
//          Car.M1_M2_Cnt = 300;
//          if (Car.Cali_flag == 1)
//          {
//            G_LED(4); // 点亮绿色LED。
//            if (Car.Sensitivity > 25.0f)
//            {
//              if (Car.Rc_Mode == 1)
//                B_LED(2); // 根据模式显示蓝色LED。
//              else
//                B_LED(3);
//            }
//            else
//              B_LED(0);     // 关闭蓝色LED。
//            MotorControl(); // 控制电机。
//          }
//        }
//      }
//      if (Car.Print_flag == 1)
//        UsartPrint();  // 串口打印信息。
//      Key_Scan();      // 按键扫描。
//      Get_InputVolt(); // 获取输入电压。
//    }
//  }
	
}


//void Peripherals_Init(void)
//{
//  LED_Init();
//  W25Qxx_Init();    // 初始化W25Qxx存储芯片。
//  Read_Cali_Data(); // 读取校准数据。
//  Gpio_config();
//  Key_Init();
//  Check_Encoder(); // 检查编码器模块。
//  TIM10_Init();
//  TIM7_Init();
//  usb_init();          // 初始化USB模块。
//  USART1_init(921600); // ESP32通信
//  USART2_init(100000); // SUBUS通信
//  USART3_init(230400); // 串口打印
//  USART4_init(115200); // 串口打印
//  IMU_Init();          // 初始化IMU
//  delay_ms(10);        // delay_ms(500); delay_ms(500); delay_ms(500);
//  TIM1_Init();
//  TIM8_Init();
//  ADC_Config();

//  parameters_Init();  	// 初始化各种参数。

//  delay_ms(10);
//  WS2812_init(); 		 	  // 初始化WS2812 RGB LED模块。
//	
//	freertos_demo();      // 运行FreeRTOS例程
//	
//	
//  while (1)
//  {

//    if (TIM_Flag == 1)
//    {
//      TIM_Flag = 0;

//      if (++task100ms >= 100)
//      {
//        task100ms = 0;

//        if (M1_Check_flag == 1 && M2_Check_flag == 1)
//        {
//          M1_Check_flag = M2_Check_flag = 0;
//          Hardware_Tips(); // 硬件检查提示。
//        }

//        if (usb_core_dev.conn_state == 0x03)
//        {
//          usb_ReadWrite(); // 100ms读取一次USB数据
//        }
//        Volt16 = (u16)(Car.BatVin_filter * 100.0f); // 获取电池电压值并转换为整数。
//        txdata[0] = Volt16 & 0xff;                  // 电压低8位。
//        txdata[1] = Volt16 >> 8;                    // 电压高8位。
//        UART1_SendDataDMA(txdata, 4);               // 每100ms通过DMA发送电压数据到ESP32。

//        if (Car.BatVin_filter > 5.5f && Car.BatVin_filter < 7.0f) // 电压低于6.8V
//        {
//          LowBattery = 1; // 产生标志，用于报警
//        }
//        else
//        {
//          if (BatteryFlag == 1) // 否则关闭蜂鸣器
//          {
//            TMR10->c1dt = 0;
//            LowBattery = 0; // 重置低电压标志。
//          }
//        }
//      }

//      if (Auto_Cali_Flag == 1)
//      {
//        Auto_Cali(); // 执行自动校准。
//      }

//      Car_Cali();   // 车辆校准。
//      RcData();     // 获取遥控器数据。
//      IMU_handle(); // 处理IMU数据。

//      if (mpu6500.Cali_Flag == 1)
//      {
//        if (++Car.M1_M2_Cnt >= 300) // 等待电机初始化
//        {
//          Car.M1_M2_Cnt = 300;
//          if (Car.Cali_flag == 1)
//          {
//            G_LED(4); // 点亮绿色LED。
//            if (Car.Sensitivity > 25.0f)
//            {
//              if (Car.Rc_Mode == 1)
//                B_LED(2); // 根据模式显示蓝色LED。
//              else
//                B_LED(3);
//            }
//            else
//              B_LED(0);     // 关闭蓝色LED。
//            MotorControl(); // 控制电机。
//          }
//        }
//      }
//      if (Car.Print_flag == 1)
//        UsartPrint();  // 串口打印信息。
//      Key_Scan();      // 按键扫描。
//      Get_InputVolt(); // 获取输入电压。

//      if (++seconds >= 5)
//      {
//        seconds = 0;

//        if (Rc.ch2 == 1)
//        {
//          if (LD1 < 200)
//          {
//            LD1++;
//            LD3 = LD2 = LD1;
//          }
//        }
//        else if (Rc.ch2 == 0)
//        {
//          if (LD1 > 0)
//          {
//            LD1--;
//            LD3 = LD2 = LD1;
//          }
//        }
//      }

//      if (WS2812_SendFlag == 0)
//      {
//        WS2812_SendFlag = 1;
//        if (Rc.ch2 == 1 && LowBattery == 0)
//        {
//          RGB_LD = (LD3 << 16) | (LD2 << 8) | LD1;

//          RGB_Data[0] = 0x161600;
//          RGB_Data[1] = RGB_LD;
//          RGB_Data[2] = RGB_LD;
//          RGB_Data[3] = RGB_LD;
//          RGB_Data[4] = RGB_LD;
//          RGB_Data[5] = 0x161600;

//          //					RGB_Data[6] = 0x160000;
//          //					RGB_Data[7] = 0x001600;

//          RGB_Data[8] = 0x323232;
//          //					RGB_Data[9] = RGB_LD;
//          RGB_Data[10] = 0x323232;
//          RGB_Data[11] = 0x323232;
//          //					RGB_Data[12] = RGB_LD;
//          RGB_Data[13] = 0x323232;

//          if (RGB_Cnt < 40)
//          {
//            if (RGB_Flag == 0)
//            {
//              RGB_Data[7] = RGB_Data[9] = 0x0000EE;
//              RGB_Data[6] = RGB_Data[12] = 0x000000;
//            }
//            else if (RGB_Flag == 1)
//            {
//              RGB_Data[7] = RGB_Data[9] = 0x000000;
//              RGB_Data[6] = RGB_Data[12] = 0xEE0000;
//            }
//          }
//          else if (RGB_Cnt == 80)
//          {
//            RGB_Data[7] = RGB_Data[9] = 0x000000;
//            RGB_Data[6] = RGB_Data[12] = 0x000000;
//          }
//          else if (RGB_Cnt == 120)
//          {
//            if (RGB_Flag == 0)
//            {
//              RGB_Data[7] = RGB_Data[9] = 0x0000EE;
//              RGB_Data[6] = RGB_Data[12] = 0x000000;
//            }
//            else if (RGB_Flag == 1)
//            {
//              RGB_Data[7] = RGB_Data[9] = 0x000000;
//              RGB_Data[6] = RGB_Data[12] = 0xEE0000;
//            }
//          }
//          else if (RGB_Cnt == 160)
//          {
//            RGB_Data[7] = RGB_Data[9] = 0x000000;
//            RGB_Data[6] = RGB_Data[12] = 0x000000;
//          }
//          else if (RGB_Cnt == 100)
//          {
//            if (RGB_Flag == 0)
//            {
//              RGB_Data[7] = RGB_Data[9] = 0x0000EE;
//              RGB_Data[6] = RGB_Data[12] = 0x000000;
//            }
//            else if (RGB_Flag == 1)
//            {
//              RGB_Data[7] = RGB_Data[9] = 0x000000;
//              RGB_Data[6] = RGB_Data[12] = 0xEE0000;
//            }
//          }
//          else if (RGB_Cnt == 200)
//          {
//            RGB_Data[7] = RGB_Data[9] = 0x000000;
//            RGB_Data[6] = RGB_Data[12] = 0x000000;
//          }
//          else if (RGB_Cnt >= 500)
//          {
//            RGB_Cnt = 0;
//            if (RGB_Flag == 0)
//              RGB_Flag = 1;
//            else if (RGB_Flag == 1)
//              RGB_Flag = 0;
//          }
//          RGB_Cnt++;
//        }
//        else if (Rc.ch2 == 0 && LowBattery == 0)
//        {
//          RGB_LD = (LD3 << 16) | (LD2 << 8) | LD1;
//          RGB_Data[0] = 0;
//          RGB_Data[1] = RGB_LD;
//          RGB_Data[2] = RGB_LD;
//          RGB_Data[3] = RGB_LD;
//          RGB_Data[4] = RGB_LD;
//          RGB_Data[5] = 0;
//          RGB_Data[6] = 0;
//          RGB_Data[7] = 0;

//          RGB_Data[8] = 0;
//          RGB_Data[9] = 0;
//          RGB_Data[10] = 0;
//          RGB_Data[11] = 0;
//          RGB_Data[12] = 0;
//          RGB_Data[13] = 0;
//        }
//        else if (LowBattery == 1)
//        {
//          if (++RGB_Cnt >= 250)
//          {
//            RGB_Cnt = 0;
//            if (BatteryFlag == 0)
//            {
//              TMR10->c1dt = 200;
//              BatteryFlag = 1;
//              RGB_Data[0] = 0;
//              RGB_Data[1] = 0;
//              RGB_Data[2] = 0;
//              RGB_Data[3] = 0;
//              RGB_Data[4] = 0;
//              RGB_Data[5] = 0;
//              RGB_Data[6] = 0xEE0000;
//              RGB_Data[7] = 0xEE0000;

//              RGB_Data[8] = 0;
//              RGB_Data[9] = 0xEE0000;
//              RGB_Data[10] = 0;
//              RGB_Data[11] = 0;
//              RGB_Data[12] = 0xEE0000;
//              RGB_Data[13] = 0;
//            }
//            else if (BatteryFlag == 1)
//            {
//              TMR10->c1dt = 0;
//              BatteryFlag = 0;
//              RGB_Data[0] = 0;
//              RGB_Data[1] = 0;
//              RGB_Data[2] = 0;
//              RGB_Data[3] = 0;
//              RGB_Data[4] = 0;
//              RGB_Data[5] = 0;
//              RGB_Data[6] = 0;
//              RGB_Data[7] = 0;

//              RGB_Data[8] = 0;
//              RGB_Data[9] = 0;
//              RGB_Data[10] = 0;
//              RGB_Data[11] = 0;
//              RGB_Data[12] = 0;
//              RGB_Data[13] = 0;
//            }
//          }
//        }

//        send_Data(RGB_Data, 14);
//      }
//    }
//  }
//}
