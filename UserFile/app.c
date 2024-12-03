
#include "app.h"


//extern u8 seconds; // ���ʱ���������ڼ�¼������������

//extern u16 RGB_Cnt = 0;    // RGB���Ƶļ�������
//extern u8 RGB_Flag = 0;    // RGBģʽ��־��
//extern u8 LowBattery = 0;  // �͵�ѹ��־��
//extern u8 BatteryFlag = 0; // ��ر�����־��
//extern u8 LD1, LD2, LD3;   // RGB�Ƶ����Ȳ�����
//extern u32 RGB_LD;         // ���ڴ洢RGB�Ƶ���ɫ���ݡ�
//extern u8 txdata[4];       // �洢��ESP32���͵ĵ�ѹ���ݡ�
//extern u16 Volt16;         // ��ѹ���ݵ�16λ�洢������
//extern uint8_t task100ms;  // ÿ100msִ��һ������ļ�������

void Peripherals_Init(void)
{
  LED_Init();
  W25Qxx_Init();    // ��ʼ��W25Qxx�洢оƬ��
  Read_Cali_Data(); // ��ȡУ׼���ݡ�
  Gpio_config();
  Key_Init();
  Check_Encoder(); // ��������ģ�顣
  TIM10_Init();
  TIM7_Init();
  usb_init();          // ��ʼ��USBģ�顣
  USART1_init(921600); // ESP32ͨ��
  USART2_init(100000); // SUBUSͨ��
  USART3_init(230400); // ���ڴ�ӡ
  USART4_init(115200); // ���ڴ�ӡ
  IMU_Init();          // ��ʼ��IMU
  delay_ms(10);        // delay_ms(500); delay_ms(500); delay_ms(500);
  TIM1_Init();
  TIM8_Init();
  ADC_Config();

  parameters_Init();  	// ��ʼ�����ֲ�����

  delay_ms(10);
  WS2812_init(); 		 	  // ��ʼ��WS2812 RGB LEDģ�顣
	
	freertos_demo();      // ����FreeRTOS����
	
	
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
//          Hardware_Tips(); // Ӳ�������ʾ��
//        }

//        if (usb_core_dev.conn_state == 0x03)
//        {
//          usb_ReadWrite(); // 100ms��ȡһ��USB����
//        }
//        Volt16 = (u16)(Car.BatVin_filter * 100.0f); // ��ȡ��ص�ѹֵ��ת��Ϊ������
//        txdata[0] = Volt16 & 0xff;                  // ��ѹ��8λ��
//        txdata[1] = Volt16 >> 8;                    // ��ѹ��8λ��
//        UART1_SendDataDMA(txdata, 4);               // ÿ100msͨ��DMA���͵�ѹ���ݵ�ESP32��

//        if (Car.BatVin_filter > 5.5f && Car.BatVin_filter < 7.0f) // ��ѹ����6.8V
//        {
//          LowBattery = 1; // ������־�����ڱ���
//        }
//        else
//        {
//          if (BatteryFlag == 1) // ����رշ�����
//          {
//            TMR10->c1dt = 0;
//            LowBattery = 0; // ���õ͵�ѹ��־��
//          }
//        }
//      }

//      if (Auto_Cali_Flag == 1)
//      {
//        Auto_Cali(); // ִ���Զ�У׼��
//      }

//      Car_Cali();   // ����У׼��
//      RcData();     // ��ȡң�������ݡ�
//      IMU_handle(); // ����IMU���ݡ�

//      if (mpu6500.Cali_Flag == 1)
//      {
//        if (++Car.M1_M2_Cnt >= 300) // �ȴ������ʼ��
//        {
//          Car.M1_M2_Cnt = 300;
//          if (Car.Cali_flag == 1)
//          {
//            G_LED(4); // ������ɫLED��
//            if (Car.Sensitivity > 25.0f)
//            {
//              if (Car.Rc_Mode == 1)
//                B_LED(2); // ����ģʽ��ʾ��ɫLED��
//              else
//                B_LED(3);
//            }
//            else
//              B_LED(0);     // �ر���ɫLED��
//            MotorControl(); // ���Ƶ����
//          }
//        }
//      }
//      if (Car.Print_flag == 1)
//        UsartPrint();  // ���ڴ�ӡ��Ϣ��
//      Key_Scan();      // ����ɨ�衣
//      Get_InputVolt(); // ��ȡ�����ѹ��
//    }
//  }
	
}


//void Peripherals_Init(void)
//{
//  LED_Init();
//  W25Qxx_Init();    // ��ʼ��W25Qxx�洢оƬ��
//  Read_Cali_Data(); // ��ȡУ׼���ݡ�
//  Gpio_config();
//  Key_Init();
//  Check_Encoder(); // ��������ģ�顣
//  TIM10_Init();
//  TIM7_Init();
//  usb_init();          // ��ʼ��USBģ�顣
//  USART1_init(921600); // ESP32ͨ��
//  USART2_init(100000); // SUBUSͨ��
//  USART3_init(230400); // ���ڴ�ӡ
//  USART4_init(115200); // ���ڴ�ӡ
//  IMU_Init();          // ��ʼ��IMU
//  delay_ms(10);        // delay_ms(500); delay_ms(500); delay_ms(500);
//  TIM1_Init();
//  TIM8_Init();
//  ADC_Config();

//  parameters_Init();  	// ��ʼ�����ֲ�����

//  delay_ms(10);
//  WS2812_init(); 		 	  // ��ʼ��WS2812 RGB LEDģ�顣
//	
//	freertos_demo();      // ����FreeRTOS����
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
//          Hardware_Tips(); // Ӳ�������ʾ��
//        }

//        if (usb_core_dev.conn_state == 0x03)
//        {
//          usb_ReadWrite(); // 100ms��ȡһ��USB����
//        }
//        Volt16 = (u16)(Car.BatVin_filter * 100.0f); // ��ȡ��ص�ѹֵ��ת��Ϊ������
//        txdata[0] = Volt16 & 0xff;                  // ��ѹ��8λ��
//        txdata[1] = Volt16 >> 8;                    // ��ѹ��8λ��
//        UART1_SendDataDMA(txdata, 4);               // ÿ100msͨ��DMA���͵�ѹ���ݵ�ESP32��

//        if (Car.BatVin_filter > 5.5f && Car.BatVin_filter < 7.0f) // ��ѹ����6.8V
//        {
//          LowBattery = 1; // ������־�����ڱ���
//        }
//        else
//        {
//          if (BatteryFlag == 1) // ����رշ�����
//          {
//            TMR10->c1dt = 0;
//            LowBattery = 0; // ���õ͵�ѹ��־��
//          }
//        }
//      }

//      if (Auto_Cali_Flag == 1)
//      {
//        Auto_Cali(); // ִ���Զ�У׼��
//      }

//      Car_Cali();   // ����У׼��
//      RcData();     // ��ȡң�������ݡ�
//      IMU_handle(); // ����IMU���ݡ�

//      if (mpu6500.Cali_Flag == 1)
//      {
//        if (++Car.M1_M2_Cnt >= 300) // �ȴ������ʼ��
//        {
//          Car.M1_M2_Cnt = 300;
//          if (Car.Cali_flag == 1)
//          {
//            G_LED(4); // ������ɫLED��
//            if (Car.Sensitivity > 25.0f)
//            {
//              if (Car.Rc_Mode == 1)
//                B_LED(2); // ����ģʽ��ʾ��ɫLED��
//              else
//                B_LED(3);
//            }
//            else
//              B_LED(0);     // �ر���ɫLED��
//            MotorControl(); // ���Ƶ����
//          }
//        }
//      }
//      if (Car.Print_flag == 1)
//        UsartPrint();  // ���ڴ�ӡ��Ϣ��
//      Key_Scan();      // ����ɨ�衣
//      Get_InputVolt(); // ��ȡ�����ѹ��

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
