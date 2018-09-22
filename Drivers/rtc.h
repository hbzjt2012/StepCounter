
#ifndef __RTC_H
#define __RTC_H

#define BKP_VALUE    0x32F0

void Init_RTC(void);
void rtc_setup(void);
void rtc_show_time(void);
uint8_t usart_input_threshold(uint32_t value);

#endif

