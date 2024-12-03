#include "app.h"

usbd_core_type usb_core_dev;
uint8_t usb_rx_buffer[256];
uint8_t usb_tx_buffer[256];

/**
  * @brief  usb 48M clock select
  * @param  clk_s:USB_CLK_HICK, USB_CLK_HEXT
  * @retval none
  */
void usb_clock48m_select(void)
{
	crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);

	/* enable the acc calibration ready interrupt */
	crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

	/* update the c1\c2\c3 value */
	acc_write_c1(7980);
	acc_write_c2(8000);
	acc_write_c3(8020);

	/* open acc calibration */
	acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
}

void usb_init(void)
{
  /* select usb 48m clcok source */
  usb_clock48m_select();

  /* enable usb clock */
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);

  /* enable usb interrupt */
  nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 0, 0);

  /* usb core init */
  usbd_core_init(&usb_core_dev, USB, &cdc_class_handler, &cdc_desc_handler, 0);

  /* enable usb pull-up */
  usbd_connect(&usb_core_dev);
}

void usb_ReadWrite(void)
{
	uint16_t data_len = 0;
//	/* get usb vcp receive data */
  data_len = usb_vcp_get_rxdata(&usb_core_dev, Get_usart3_data);
	
	if(data_len > 0)
	{
		U3_IDLE_Flag = 1;
//		usb_vcp_send_data(&usb_core_dev, usb_rx_buffer, data_len);
	}
}

///**
//  * @brief  this function handles usb interrupt.
//  * @param  none
//  * @retval none
//  */
//void USBFS_L_CAN1_RX0_IRQHandler(void)
//{
//  usbd_irq_handler(&usb_core_dev);
//}

void USBFS_L_CAN1_RX0_IRQHandler(void)
{
	usbd_irq_handler(&usb_core_dev);
}
