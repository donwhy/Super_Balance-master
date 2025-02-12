#include "app.h"

//按键初始化函数
void Key_Init(void)
{
	gpio_init_type gpio_init_struct;
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
	gpio_default_para_init(&gpio_init_struct);
	/* configure the led gpio */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_11; 
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOD, &gpio_init_struct);
	
	gpio_bits_set(GPIOD,GPIO_PINS_11); 
} 
//按键处理函数
//返回按键值

u16 Key1_Count = 0;
u8 KeyFlag = 0;
u16 KeyCnt = 200; 
uint16_t CarCaliCnt = 0;

void Key_Scan(void)
{	 
	if(Key1 == 0 && Key1_Count < KeyCnt)
	{
		Key1_Count++;
		if(Key1 == 0 && Key1_Count >= KeyCnt) // KeyCnt = 去抖时间
		{
			if(KeyFlag == 0)
			{
				KeyFlag = 1;
				KeyCnt = 20;
				Car.PowerFlag = 1;
				TMR10->c1dt = 200;
				delay_ms(50);
				TMR10->c1dt = 0;
				G_LED(1);
				gpio_bits_set(GPIOD,GPIO_PINS_10);			
			}
			else if(KeyFlag == 1)
			{
				KeyFlag = 0;
				M1_Foc.Cali_flag = 0;
				M2_Foc.Cali_flag = 0;
				TMR10->c1dt = 200;
				delay_ms(300);
				TMR10->c1dt = 0;
				R_LED(1);
				G_LED(0);
				B_LED(0);
				
				gpio_bits_reset(GPIOE,GPIO_PINS_7); // 12V电源失能
				gpio_bits_reset(GPIOD,GPIO_PINS_10);
				while(1){};
			}
		}
	}
	else if(Key1 != 0)
	{
		Key1_Count = 0;
	}
}



















