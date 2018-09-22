#include "main.h"

void LED_Init(void)
{
	 rcu_periph_clock_enable(RCU_GPIOB); 
   gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,  
                  GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10); 
   gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,  
                            GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10); 
	
	LED4_OFF();
	LED5_OFF();
	LED6_OFF();
}

 
