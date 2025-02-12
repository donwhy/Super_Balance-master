#ifndef __app_H
#define __app_H

#include "at32f403a_407_clock.h"

#include "at32f403a_407.h"

#include <stdlib.h>
#include "string.h"
#include "stdio.h"
#include "string.h"
#include "arm_math.h"

#include "delay.h"
#include "timer.h"
#include "MPU6500.h"
#include "usart.h"
#include "Debug.h"
#include "AHRS.h"
#include "led.h"
#include "adc.h"
#include "AT32F403A_FOC_LIB.h"
#include "Control.h"
#include "Gpio_Config.h"
#include "key.h"
#include "Encoder.h"
#include "Calibration.h"

#include "WS2812.h"

#include "W25Q64.h"
#include "RcData.h"
//#include "hmc5883l.h"
//#include "myiic.h"

#include "usb_std.h"
#include "usbd_sdr.h"
#include "usbd_core.h"
#include "cdc_desc.h"
#include "cdc_class.h"
#include "usbd_int.h"

#include "usb.h"

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_demo.h"

void Peripherals_Init(void);

#endif
