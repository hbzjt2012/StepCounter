/*!
    \file  main.c
    \brief led spark with systick, USART print and key example
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#include "gd32f3x0.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "bmp.h"
#include "StepCount.h"

extern unsigned char TX_DATA[4];  
extern short T_X,T_Y,T_Z;

extern SensorData GMeter;
short x,y,z;
int16_t ucData[32] = {0};
unsigned short m = 0;
unsigned char n = 0;
DATATYPE DateBufferX[10] = {9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000};
DATATYPE DateBufferY[10] = {9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000};
DATATYPE DateBufferZ[10] = {9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000};
unsigned short StepCountFinal = 0;
unsigned char StepFlag  = 0;
unsigned char InitFlag = 0;
unsigned char m_count = 0;
unsigned char GMeterAmax = 0;

uint16_t TempCount = 0;

float datalx = 0.0; 

rtc_parameter_struct   rtc_info;
char outputstring[40];

uint8_t menu_index = 1;

void CountStep(void)
{
    /*计步算法*/
	unsigned char i = 0;
    signed short X = 0, Y = 0, Z = 0;
    
    READ_L3G4200D();	     //读取L3G4200D数据
    DATA_printf(TX_DATA,T_X);
    if(TX_DATA[0]==0x2D)
        x=-((TX_DATA[1]&0x0f)*100+(TX_DATA[2]&0x0f)*10+(TX_DATA[3]&0x0f));
    else
        x=((TX_DATA[1]&0x0f)*100+(TX_DATA[2]&0x0f)*10+(TX_DATA[3]&0x0f));
    Send_data('X');
    DATA_printf(TX_DATA,T_Y);
    if(TX_DATA[0]==0x2D)
        y=-((TX_DATA[1]&0x0f)*100+(TX_DATA[2]&0x0f)*10+(TX_DATA[3]&0x0f));
    else
        y=((TX_DATA[1]&0x0f)*100+(TX_DATA[2]&0x0f)*10+(TX_DATA[3]&0x0f));
    Send_data('Y');
    DATA_printf(TX_DATA,T_Z);
    if(TX_DATA[0]==0x2D)
        z=-((TX_DATA[1]&0x0f)*100+(TX_DATA[2]&0x0f)*10+(TX_DATA[3]&0x0f));
    else
        z=((TX_DATA[1]&0x0f)*100+(TX_DATA[2]&0x0f)*10+(TX_DATA[3]&0x0f));
    Send_data('Z');
    printf("\r\n");
    
    ucData[0]=x;
    ucData[1]=y;
    ucData[2]=z;
    ucData[6] = StepCountFinal;
    X = ucData[0];
    Y = ucData[1];
    Z = ucData[2];
    
    if(m == DATASIZE)
    {
        m = 0;
        if(m_count == 0)
        {
            m_count = 1;
        }
    }
    if(n == 5) {
        n = 0;
    }
    DateBufferX[n] = X;
    DateBufferY[n] = Y;
    DateBufferZ[n] = Z;
    if(InitFlag < 5) {
        GMeter.X.Data[m] = X;
        GMeter.Y.Data[m] = Y;
        GMeter.Z.Data[m] = Z;
        InitFlag++;
    } else {
        GMeter.X.Data[m] = (DateBufferX[0] + DateBufferX[1] + DateBufferX[2] + DateBufferX[3] + DateBufferX[4])/ 5;
        GMeter.Y.Data[m] = (DateBufferY[0] + DateBufferY[1] + DateBufferY[2] + DateBufferX[3] + DateBufferX[4])/ 5;
        GMeter.Z.Data[m] = (DateBufferZ[0] + DateBufferZ[1] + DateBufferZ[2] + DateBufferX[3] + DateBufferX[4])/ 5;
    }
    if(m_count == 1) {
        if(GMeter.X.MaxMark == m || GMeter.X.MinMark == m || GMeter.Y.MaxMark == m || GMeter.Y.MinMark == m || GMeter.Z.MaxMark == m || GMeter.Z.MinMark == m) {
            unsigned char tempXMaxMark = GMeter.X.MaxMark;
            unsigned char tempXMinMark = GMeter.X.MinMark;
            unsigned char tempYMaxMark = GMeter.Y.MaxMark;
            unsigned char tempYMinMark = GMeter.Y.MinMark;
            unsigned char tempZMaxMark = GMeter.Z.MaxMark;
            unsigned char tempZMinMark = GMeter.Z.MinMark;
            if(GMeter.X.MaxMark == m) {
                GMeter.X.Max = -200;
            }
            if(GMeter.X.MinMark == m) {
                GMeter.X.Min = +200;
            }
            if(GMeter.Y.MaxMark == m) {
                GMeter.Y.Max = -200;
            }
            if(GMeter.Y.MinMark == m) {
                GMeter.Y.Min = +200;
            }
            if(GMeter.Z.MaxMark == m) {
                GMeter.Z.Max = -200;
            }
            if(GMeter.Z.MinMark == m) {
                GMeter.Z.Min = +200;
            }
            for(i = 0; i < DATASIZE; i++) {
                if(GMeter.X.MaxMark == m) {
                    if(GMeter.X.Data[i] >= GMeter.X.Max) {
                        GMeter.X.Max = GMeter.X.Data[i];
                        tempXMaxMark = i;
                    }
                }
                if(GMeter.X.MinMark == m) {
                    if(GMeter.X.Data[i] <= GMeter.X.Min) {
                        GMeter.X.Min = GMeter.X.Data[i];
                        tempXMinMark = i;
                    }
                }
                if(GMeter.Y.MaxMark == m) {
                    if(GMeter.Y.Data[i] >= GMeter.Y.Max) {
                        GMeter.Y.Max = GMeter.Y.Data[i];
                        tempYMaxMark = i;
                    }
                }
                if(GMeter.Y.MinMark == m) {
                    if(GMeter.Y.Data[i] <= GMeter.Y.Min) {
                        GMeter.Y.Min = GMeter.Y.Data[i];
                        tempYMinMark = i;
                    }
                }
                if(GMeter.Z.MaxMark == m) {
                    if(GMeter.Z.Data[i] >= GMeter.Z.Max) {
                        GMeter.Z.Max = GMeter.Z.Data[i];
                        tempZMaxMark = i;
                    }
                }
                if(GMeter.Z.MinMark == m) {
                    if(GMeter.Z.Data[i] <= GMeter.Z.Min) {
                        GMeter.Z.Min = GMeter.Z.Data[i];
                        tempZMinMark = i;
                    }
                }
            }
            GMeter.X.MaxMark = tempXMaxMark;
            GMeter.X.MinMark = tempXMinMark;
            GMeter.Y.MaxMark = tempYMaxMark;
            GMeter.Y.MinMark = tempYMinMark;
            GMeter.Z.MaxMark = tempZMaxMark;
            GMeter.Z.MinMark = tempZMinMark;
        }
    }
    if(GMeter.X.Data[m] >= GMeter.X.Max) {
        GMeter.X.Max = GMeter.X.Data[m];
        GMeter.X.MaxMark = m;
    }
    if(GMeter.X.Data[m] <= GMeter.X.Min) {
        GMeter.X.Min = GMeter.X.Data[m];
        GMeter.X.MaxMark = m;
    }
    if(GMeter.Y.Data[m] >= GMeter.Y.Max) {
        GMeter.Y.Max = GMeter.Y.Data[m];
        GMeter.Y.MaxMark = m;
    }
    if(GMeter.Y.Data[m] <= GMeter.Y.Min) {
        GMeter.Y.Min = GMeter.Y.Data[m];
        GMeter.Y.MinMark = m;
    }
    if(GMeter.Z.Data[m] >= GMeter.Z.Max) {
        GMeter.Z.Max = GMeter.Z.Data[m];
        GMeter.Z.MaxMark = m;
    }
    if(GMeter.Z.Data[m] <= GMeter.Z.Min) {
        GMeter.Z.Min = GMeter.Z.Data[m];
        GMeter.Z.MinMark = m;
    }
    GMeter.X.Base      = (GMeter.X.Max + GMeter.X.Min) / 2;
    GMeter.Y.Base      = (GMeter.Y.Max + GMeter.Y.Min) / 2;
    GMeter.Z.Base      = (GMeter.Z.Max + GMeter.Z.Min) / 2;
    GMeter.X.UpLimit   = (GMeter.X.Base + GMeter.X.Max * 2) / 3;
    GMeter.Y.UpLimit   = (GMeter.Y.Base + GMeter.Y.Max * 2) / 3;
    GMeter.Z.UpLimit   = (GMeter.Z.Base + GMeter.Z.Max * 2) / 3;
    GMeter.X.DownLimit = (GMeter.X.Base + GMeter.X.Min * 2) / 3;
    GMeter.Y.DownLimit = (GMeter.Y.Base + GMeter.Y.Min * 2) / 3;
    GMeter.Z.DownLimit = (GMeter.Z.Base + GMeter.Z.Min * 2) / 3;
    GMeterAmax = DataSelect(&GMeter);
    switch(GMeterAmax) {
    case 1:
        if((GMeter.X.Data[m] > GMeter.X.UpLimit) && StepFlag == 0) {
            StepFlag = 1;
        }
        if((GMeter.X.Data[m] < GMeter.X.DownLimit) && StepFlag ==1) {
            StepFlag = 0;
            StepCountFinal++;
           // StepCountFinal++;
        }
        break;
    case 2:
        if((GMeter.Y.Data[m] > GMeter.Y.UpLimit) && StepFlag == 0) {
            StepFlag = 1;
        }
        if((GMeter.Y.Data[m] < GMeter.Y.DownLimit) && StepFlag ==1) {
            StepFlag = 0;
            StepCountFinal++;
           // StepCountFinal++;
        }
        break;
    case 3:
        if((GMeter.Z.Data[m] > GMeter.Z.UpLimit) && StepFlag == 0) {
            StepFlag = 1;
        }
        if((GMeter.Z.Data[m] < GMeter.Z.DownLimit) && StepFlag ==1) {
            StepFlag = 0;
            StepCountFinal++;
          //  StepCountFinal++;
        }
        break;
    default:
        break;
    }
    ucData[6]   = GMeter.X.Data[m] >> 8;
    ucData[7]   = GMeter.X.Data[m] & 0x00FF;
    ucData[8]   = GMeter.Y.Data[m] >> 8;
    ucData[9]   = GMeter.Y.Data[m] & 0x00FF;
    ucData[10]  = GMeter.Z.Data[m] >> 8;
    ucData[11]  = GMeter.Z.Data[m] & 0x00FF;
    ucData[12]  = GMeter.X.Base >> 8;
    ucData[13]  = GMeter.X.Base & 0x00FF;
    ucData[14]  = GMeter.X.UpLimit >> 8;
    ucData[15]  = GMeter.X.UpLimit & 0x00FF;
    ucData[16]  = GMeter.X.DownLimit >> 8;
    ucData[17]  = GMeter.X.DownLimit & 0x00FF;
    ucData[18]  = GMeter.X.Max >> 8;
    ucData[19]  = GMeter.X.Max & 0x00FF;
    ucData[20]  = GMeter.X.Min >> 8;
    ucData[21]  = GMeter.X.Min & 0x00FF;
    ucData[22]  = StepCountFinal;
    ucData[23]  = StepFlag;
    ucData[24]  = 0;
    ucData[25]  = 0;
    m++;
    n++;

//    OLED_DisNum(1u,2u,ucData[22],3,16,LineYes);
		printf("CountSteps: %i \r\n",(uint16_t)ucData[22]);
}

void PowerOn(void)
{	
		rtc_current_time_get(&rtc_info);  
		
    OLED_Clear();
    OLED_DisString(0u,0u,(uint8_t *)"StepCounter V1.0");
    OLED_DisString(0u,3u,(uint8_t *)"By:hanyeguxingwo");
		sprintf(outputstring,"Date:20%0.2x-%0.2x-%0.2x",rtc_info.rtc_year, rtc_info.rtc_month , rtc_info.rtc_date);
	  OLED_DisString(4u,6u,(uint8_t *)outputstring);
    //OLED_DisString(4u,6u,(uint8_t *)"Date:2018-09-17");
    delay_1ms(3000);
    OLED_Clear();
    OLED_DisBMP(0u,0u,128u,8u,(uint8_t *)EEWORLD_logo);
    delay_1ms(5000);
    OLED_Clear();
}

void TestFunc()
{
	LED4_ON();
	delay_1ms(500);
	LED5_ON();
	delay_1ms(500);
	LED6_ON();
	delay_1ms(500);

	LED4_OFF();
	delay_1ms(500);
	LED5_OFF();
	delay_1ms(500);
	LED6_OFF();
	delay_1ms(500);

	//usart_data_transmit(USART1, 'c');
	//printf("Hello World! Printf Test On GD350 MCU!\n");

	READ_L3G4200D();	     //读取L3G4200D数据
	DATA_printf(TX_DATA,T_X);
			
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure systick */
    systick_config();
	
		LED_Init();
		USART1_Init();
		OLED_Init();
		Init_L3G4200D();
		Init_BH1750();
		Init_RTC();
		Init_Key();
	
		PowerOn();
		
    while (1)
		{
			TempCount++;
			
			/* check if RTC has aready been configured */
			if (BKP_VALUE != RTC_BKP0){    
					rtc_setup(); 
			}else{
					/* detect the reset source */
					if (RESET != rcu_flag_get(RCU_FLAG_PORRST)){
							printf("power on reset occurred....\n\r");
					}else if (RESET != rcu_flag_get(RCU_FLAG_EPRST)){
							printf("external reset occurred....\n\r");
					}
					printf("no need to configure RTC....\n\r");

					rtc_show_time();
			}
			rcu_all_reset_flag_clear();
			
			/* check whether the button is pressed */
			if(RESET == gpio_input_bit_get(GPIOB, GPIO_PIN_7)){
					delay_1ms(100);
					/* check whether the button is pressed */
					if(RESET == gpio_input_bit_get(GPIOB, GPIO_PIN_7)){
							menu_index++;
							if(menu_index == 3)
								menu_index = 1;
						  OLED_Clear();
					}
			}
			
			CountStep();
			
			if(TempCount == 15)
			{
				LED4_ON();
				LED5_OFF();
				LED6_OFF();
			}else if(TempCount == 30)
			{
				LED4_OFF();
				LED5_ON();
				LED6_OFF();
			}else if(TempCount == 45)
			{
				LED4_OFF();
				LED5_OFF();
				LED6_ON();
				TempCount = 0;
			}
			
			datalx = READ_BH1750() / 1.2; //根据手册读取的数据除以1.2就得到单位位lx的光强度检测值
			
			//根据实时光强调节OLED背光强度
			if(datalx > 0 && datalx < 600)
			{
				OLED_WR_Byte(0x81,WRcmd);//--set contrast control register
				OLED_WR_Byte(0xFF,WRcmd); // Set SEG Output Current Brightness
			}else if(datalx > 600 && datalx < 1060)
			{
				OLED_WR_Byte(0x81,WRcmd);//--set contrast control register
				OLED_WR_Byte(0x8F,WRcmd); // Set SEG Output Current Brightness
			}else if(datalx > 1060 && datalx < 1900)
			{
				OLED_WR_Byte(0x81,WRcmd);//--set contrast control register
				OLED_WR_Byte(0x3F,WRcmd); // Set SEG Output Current Brightness
			}else if(datalx > 1900 && datalx < 5000)
			{
				OLED_WR_Byte(0x81,WRcmd);//--set contrast control register
				OLED_WR_Byte(0x0F,WRcmd); // Set SEG Output Current Brightness
			}
			
			rtc_current_time_get(&rtc_info); 
			
			switch(menu_index)
			{
				case 1:
					OLED_DisString(0u,0u,(uint8_t *)"StepCounter V1.0");
					OLED_DisString(0u,2u,(uint8_t *)"Steps: ");
					OLED_DisNum(60u,2u,ucData[22],5,16,LineYes);
					OLED_DisString(0u,4u,(uint8_t *)"Light:");
					OLED_DisString(110u,4u,(uint8_t *)"lx");
					OLED_DisNum(60u,4u,datalx,5,16,LineYes);
					OLED_DisString(0u,6u,(uint8_t *)"W:eeworld.com.cn");
				break;
				case 2:
					OLED_DisBMP(48u,0u,80u,4u,(uint8_t *)CLOCK_logo);
					sprintf(outputstring,"Date:20%0.2x-%0.2x-%0.2x",rtc_info.rtc_year, rtc_info.rtc_month , rtc_info.rtc_date);
					OLED_DisString(4u,4u,(uint8_t *)outputstring);
					sprintf(outputstring,"Time:%0.2x:%0.2x:%0.2x",rtc_info.rtc_hour, rtc_info.rtc_minute, rtc_info.rtc_second);
					OLED_DisString(4u,6u,(uint8_t *)outputstring);
				break;
				default:
					OLED_DisString(0u,0u,(uint8_t *)"StepCounter V1.0");
					OLED_DisString(0u,2u,(uint8_t *)"Steps: ");
					OLED_DisNum(60u,2u,ucData[22],5,16,LineYes);
					OLED_DisString(0u,4u,(uint8_t *)"Light:");
					OLED_DisString(110u,4u,(uint8_t *)"lx");
					OLED_DisNum(60u,4u,datalx,5,16,LineYes);
					OLED_DisString(0u,6u,(uint8_t *)"W:eeworld.com.cn");
				break;
			}
			
    }
}

