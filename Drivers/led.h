#ifndef __LED_H
#define __LED_H

#define LED5_ON()  gpio_bit_set(GPIOB, GPIO_PIN_8)
#define LED5_OFF() gpio_bit_reset(GPIOB, GPIO_PIN_8)

#define LED6_ON()  gpio_bit_set(GPIOB, GPIO_PIN_9)
#define LED6_OFF() gpio_bit_reset(GPIOB, GPIO_PIN_9)

#define LED4_ON()  gpio_bit_set(GPIOB, GPIO_PIN_10)
#define LED4_OFF() gpio_bit_reset(GPIOB, GPIO_PIN_10)

void LED_Init(void);

#endif

