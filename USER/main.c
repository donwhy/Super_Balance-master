#include "app.h"

int main(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	system_clock_config();
	delay_init();
	Peripherals_Init();
	while(1)
	{
		
	}
}
