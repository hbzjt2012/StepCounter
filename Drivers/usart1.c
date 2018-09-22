#include "main.h"
#include <stdio.h>

/**
  * @brief  USART1≥ı ºªØ
**/
void USART1_Init(void)
{
	/* enable COM GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART1);

	/* connect port to USARTx_Tx */
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);

	/* connect port to USARTx_Rx */
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);

	/* configure USART Tx as alternate function push-pull */
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);

	/* configure USART Rx as alternate function push-pull */
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);

	/* USART configure */
	usart_deinit(USART1);
	usart_baudrate_set(USART1, 115200U);
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);

	usart_enable(USART1);
}


/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
	usart_data_transmit(USART1, (uint8_t)ch);
	while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));

	return ch;
}
