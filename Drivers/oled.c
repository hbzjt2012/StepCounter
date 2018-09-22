#include "main.h"
#include "oled.h"
#include "oledfont.h"
#include "systick.h"

/*******************************************************************************
* Function Name  : I2C_GPIO_Config
* Description    : Configration Simulation IIC GPIO
* Input          : None 
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_GPIO_Config(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);   //使能GPIOC时钟
	gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6|GPIO_PIN_7); 
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_6|GPIO_PIN_7); 		
}
 
/*******************************************************************************
* Function Name  : IIC_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : NONE
****************************************************************************** */
void IIC_Start(void)
{
	OLED_SDIN_OUT();
	OLED_SCLK_Set();
	OLED_SDIN_Set();
	SysTick_delay_us(1);
	OLED_SDIN_Clr();
	SysTick_delay_us(1);
	OLED_SCLK_Clr();
}
 
/*******************************************************************************
* Function Name  : IIC_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_Stop(void)
{
	OLED_SDIN_OUT();
	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	SysTick_delay_us(1);
	OLED_SCLK_Set();	
	OLED_SDIN_Set();
	SysTick_delay_us(1);
}

/*******************************************************************************
* Function Name  : IIC_Wait_Ack
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	
	OLED_SDIN_IN();
	OLED_SDIN_Set();
	SysTick_delay_us(1);
	OLED_SCLK_Set();
	SysTick_delay_us(1);
	while(OLED_SDIN_Read())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return;
		}
	}
	OLED_SCLK_Clr();
} 

//产生ACK应答
void IIC_Ack(void)
{
	OLED_SCLK_Clr();
	OLED_SDIN_OUT();
	OLED_SDIN_Clr();
	SysTick_delay_us(1);
	OLED_SCLK_Set();
	SysTick_delay_us(1);
	OLED_SCLK_Clr();
}

//不产生ACK应答	
void IIC_NoAck(void)
{
	OLED_SCLK_Clr();
	OLED_SDIN_OUT();
	OLED_SDIN_Set();
	SysTick_delay_us(1);
	OLED_SCLK_Set();
	SysTick_delay_us(1);
	OLED_SCLK_Clr();
}

/*******************************************************************************
* Function Name  : Write_IIC_Byte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void Write_IIC_Byte(uint8_t IIC_Byte)
{
	uint8_t i;
	uint8_t m,da;
	da=IIC_Byte;
	OLED_SDIN_OUT();
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)                
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
		da=da<<1;
		SysTick_delay_us(1);
		OLED_SCLK_Set();
		SysTick_delay_us(1);
		OLED_SCLK_Clr();
		//SysTick_delay_us(1);
	}
}

/*******************************************************************************
* Function Name  : Write_IIC_Command
* Description    : Master Send Command to Slave
* Input          : Will Send Command
* Output         : None
* Return         : None
****************************************************************************** */
void Write_IIC_Command(uint8_t IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);      
	IIC_Wait_Ack();        
	Write_IIC_Byte(0x00);                        
	IIC_Wait_Ack();        
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();        
	IIC_Stop();
}

/*******************************************************************************
* Function Name  : Write_IIC_Data
* Description    : Master Send Data to Slave
* Input          : Will Send Data
* Output         : None
* Return         : None
****************************************************************************** */
void Write_IIC_Data(uint8_t IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);                        
	IIC_Wait_Ack();        
	Write_IIC_Byte(0x40);                        
	IIC_Wait_Ack();        
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();        
	IIC_Stop();
}
 
/**************************************************************************************************************************
* Function Name  : OLED_WR_Byte
* Description    : Write One Byete into RAM of OLED
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/  
void OLED_WR_Byte(uint8_t dat,OLEDWriteMode mode)
{	
  if(mode)
	{
		Write_IIC_Command(dat);
	}
	else 
	{
		Write_IIC_Data(dat);
	}	  
} 

/**************************************************************************************************************************
* Function Name  : OLED_Set_Position
* Description    : Set position of display
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/  	
void OLED_Set_Position(uint8_t x, uint8_t y) 
{ 
	OLED_WR_Byte(0xb0+y,WRcmd);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,WRcmd);
	OLED_WR_Byte((x&0x0f)|0x01,WRcmd); 
}   	  
/**************************************************************************************************************************
* Function Name  : OLED_Display_On
* Description    : Open OLED
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/      
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,WRcmd);  //SET DCDC命令
	OLED_WR_Byte(0X14,WRcmd);  //DCDC ON
	OLED_WR_Byte(0XAF,WRcmd);  //DISPLAY ON
}
/**************************************************************************************************************************
* Function Name  : OLED_Display_Off
* Description    : Close OLED
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,WRcmd);  //SET DCDC命令
	OLED_WR_Byte(0X10,WRcmd);  //DCDC OFF
	OLED_WR_Byte(0XAE,WRcmd);  //DISPLAY OFF
}		   			 
/**************************************************************************************************************************
* Function Name  : OLED_Clear
* Description    : Clear OLED
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,WRcmd);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,WRcmd);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,WRcmd);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,WRdata); 
	} //更新显示
}

/**************************************************************************************************************************
* Function Name  : OLED_DisChar
* Description    : Display string
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/
void OLED_DisChar(uint8_t x,uint8_t y,uint8_t chr,AddUnderLineYesOrNot mode)
{     
	uint8_t c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			 OLED_Set_Position(x,y);	
				for(i=0;i<8;i++)
				{
						 OLED_WR_Byte(F8X16[c*16+i],WRdata);
				} 
				OLED_Set_Position(x,y+1);
				for(i=0;i<8;i++)
				{					
					if (mode==LineYes)
						 OLED_WR_Byte(F8X16[c*16+i+8]|0x80,WRdata);
					else 
					   OLED_WR_Byte(F8X16[c*16+i+8],WRdata);
				}
			}
			else {	
				OLED_Set_Position(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],WRdata);
			}
}
//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
  		
/**************************************************************************************************************************
* Function Name  : OLED_DisNum
* Description    : Display number, (x,y)coordinate of starting point,len is digits, mode 0 fill mode,mode 1 add mode
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/
void OLED_DisNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,AddUnderLineYesOrNot mode)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;	
  	
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if (mode==LineYes)
				  OLED_DisChar(x+(size/2)*t,y,'0',LineYes);
				else
					OLED_DisChar(x+(size/2)*t,y,'0',LineNot);
				continue;
			}else enshow=1; 
		 	 
		}
		if (mode==LineYes)
				OLED_DisChar(x+(size/2)*t,y,temp+'0',LineYes);
		else
				OLED_DisChar(x+(size/2)*t,y,temp+'0',LineNot); 	
	}
}
/**************************************************************************************************************************
* Function Name  : OLED_DisChar
* Description    : Display string
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/
void OLED_DisCharAddUnderLine(uint8_t x,uint8_t y,uint8_t chr)
{      	
	uint8_t c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Position(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i]|0x40,WRdata);
			OLED_Set_Position(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],WRdata);
			}
			else {	
				OLED_Set_Position(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],WRdata);
			}
}
 
 
/**************************************************************************************************************************
* Function Name  : OLED_DisString
* Description    : Display string by chr[]
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/
void OLED_DisString(uint8_t x,uint8_t y,uint8_t *chr)
{
	uint8_t j=0;
	while (chr[j]!='\0')
	{		
   OLED_DisChar(x,y,chr[j],LineNot);
	 x+=8;
	if(x>120){x=0;y+=2;}
		j++;
	}
}
/**************************************************************************************************************************
* Function Name  : OLED_DisChinese
* Description    : Display Chinese by Hzk[]
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/
void OLED_DisChinese(uint8_t x,uint8_t y,char * p,uint8_t Num,FontSize fontSize)
{      			    
	uint8_t n,t,adder=0,fontsize;
	if (fontSize==mini)fontsize=12;
	else fontsize=16;
  for(n=0;n<Num;n++)
	{
		OLED_Set_Position(x,y);
		for(t=0;t<fontsize<<1;t++)
		{
			if(t==fontsize)  OLED_Set_Position(x,y+1);	
			OLED_WR_Byte(*(p+t),WRdata);
			adder++;
		 }	
    p+=(fontsize<<1);
    x+=fontsize;		 
	}		
}
 
/**************************************************************************************************************************
* Function Name  : OLED_DisBMP
* Description    : Draw BMP(128*64) x(0～127)Column	,y(0～7).page
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/	
void OLED_DisBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{ 	
 unsigned int j=0;
 uint8_t x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Position(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],WRdata);	    	
	    }
	}
} 

/**************************************************************************************************************************
* Function Name  : OLED_Init
* Description    : Initialize SSD1306	
* Input          : None
* Output         : None
* Return         : None
**************************************************************************************************************************/			    
void OLED_Init(void)
{
	//IIC GPIO Config
	IIC_GPIO_Config();
	delay_1ms(200); 
	
	//SSD1306    
	OLED_SCLK_Set();
	OLED_SDIN_Set(); 
	delay_1ms(800); 
	
	OLED_WR_Byte(0xAE,WRcmd);//--turn off oled panel
	OLED_WR_Byte(0x00,WRcmd);//---set low column address
	OLED_WR_Byte(0x10,WRcmd);//---set high column address
	OLED_WR_Byte(0x40,WRcmd);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,WRcmd);//--set contrast control register
	OLED_WR_Byte(0xCF,WRcmd); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,WRcmd);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,WRcmd);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,WRcmd);//--set normal display
	OLED_WR_Byte(0xA8,WRcmd);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,WRcmd);//--1/64 duty
	OLED_WR_Byte(0xD3,WRcmd);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,WRcmd);//-not offset
	OLED_WR_Byte(0xd5,WRcmd);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,WRcmd);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,WRcmd);//--set pre-charge period
	OLED_WR_Byte(0xF1,WRcmd);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,WRcmd);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,WRcmd);
	OLED_WR_Byte(0xDB,WRcmd);//--set vcomh
	OLED_WR_Byte(0x40,WRcmd);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,WRcmd);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,WRcmd);//
	OLED_WR_Byte(0x8D,WRcmd);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,WRcmd);//--set(0x10) disable
	OLED_WR_Byte(0xA4,WRcmd);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,WRcmd);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,WRcmd);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,WRcmd); /*display ON*/ 

	OLED_Clear();
	OLED_Set_Position(0,0); 	
}  



