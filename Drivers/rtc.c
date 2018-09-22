#include "main.h"
#include <stdio.h>

rtc_parameter_struct   rtc_initpara;
__IO uint32_t prescaler_a = 0, prescaler_s = 0;

void Init_RTC(void)
{
	/* enable PMU clock */
	rcu_periph_clock_enable(RCU_PMU);
	/* enable the access of the RTC registers */
	pmu_backup_write_enable();
	
	rcu_osci_on(RCU_IRC40K);
	rcu_osci_stab_wait(RCU_IRC40K);
	rcu_rtc_clock_config(RCU_RTCSRC_IRC40K);

	prescaler_s = 0x18F;
	prescaler_a = 0x63;
	
	rcu_periph_clock_enable(RCU_RTC);
	rtc_register_sync_wait();
}

/*!
    \brief      use hyperterminal to setup RTC time and alarm
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_setup(void)
{
    /* setup RTC time value */
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

    rtc_initpara.rtc_factor_asyn = prescaler_a;
    rtc_initpara.rtc_factor_syn = prescaler_s;
    rtc_initpara.rtc_year = 0x18;
    rtc_initpara.rtc_day_of_week = RTC_FRIDAY;
    rtc_initpara.rtc_month = RTC_SEP;
    rtc_initpara.rtc_date = 0x21;
    rtc_initpara.rtc_display_format = RTC_24HOUR;
    rtc_initpara.rtc_am_pm = RTC_AM;

    /* current time input */
    printf("=======Configure RTC Time========\n\r");
    printf("  please input hour:\n\r");
    while (tmp_hh == 0xFF){    
        tmp_hh = usart_input_threshold(23);
        rtc_initpara.rtc_hour = tmp_hh;
    }
    printf("  %0.2x\n\r", tmp_hh);
    
    printf("  please input minute:\n\r");
    while (tmp_mm == 0xFF){    
        tmp_mm = usart_input_threshold(59);
        rtc_initpara.rtc_minute = tmp_mm;
    }
    printf("  %0.2x\n\r", tmp_mm);

    printf("  please input second:\n\r");
    while (tmp_ss == 0xFF){
        tmp_ss = usart_input_threshold(59);
        rtc_initpara.rtc_second = tmp_ss;
    }
    printf("  %0.2x\n\r", tmp_ss);

    /* RTC current time configuration */
    if(ERROR == rtc_init(&rtc_initpara)){    
        printf("\n\r** RTC time configuration failed! **\n\r");
    }else{
        printf("\n\r** RTC time configuration success! **\n\r");
        rtc_show_time();
        RTC_BKP0 = BKP_VALUE;
    }  
}

/*!
    \brief      display the current time
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_show_time(void)
{
    uint32_t time_subsecond = 0;
    uint8_t subsecond_ss = 0,subsecond_ts = 0,subsecond_hs = 0;

    rtc_current_time_get(&rtc_initpara);  

    /* get the subsecond value of current time, and convert it into fractional format */
    time_subsecond = rtc_subsecond_get();
    subsecond_ss=(1000-(time_subsecond*1000+1000)/400)/100;
    subsecond_ts=(1000-(time_subsecond*1000+1000)/400)%100/10;
    subsecond_hs=(1000-(time_subsecond*1000+1000)/400)%10;
    
    printf("Current time: 20%0.2x-%0.2x-%0.2x %0.2x:%0.2x:%0.2x .%d%d%d \n\r", \
					rtc_initpara.rtc_year, rtc_initpara.rtc_month , rtc_initpara.rtc_date,\
          rtc_initpara.rtc_hour, rtc_initpara.rtc_minute, rtc_initpara.rtc_second,\
          subsecond_ss, subsecond_ts, subsecond_hs);
}

/*!
    \brief      get the input character string and check if it is valid
    \param[in]  none
    \param[out] none
    \retval     input value in BCD mode
*/
uint8_t usart_input_threshold(uint32_t value)
{
    uint32_t index = 0;
    uint32_t tmp[2] = {0, 0};

    while (index < 2){
        while (RESET == usart_flag_get(USART1, USART_FLAG_RBNE));
        tmp[index++] = usart_data_receive(USART1);
        if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39)){
            printf("\n\r please input a valid number between 0 and 9 \n\r");
            index--;
        }
    }

    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
    if (index > value){
        printf("\n\r please input a valid number between 0 and %d \n\r", value);
        return 0xFF;
    }
    
    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) <<4);
    return index;
}
