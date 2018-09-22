#ifndef __OLED_H
#define __OLED_H			  	 

//  GND   GND
//  VCC   5V/3.3v
//  SCL   PC6(SCL)
//  SDA   PC7(SDA) 

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE   0u
#define SIZE        16u
#define XLevelL		  0x00
#define XLevelH		  0x10
#define Max_Column	128u
#define Max_Row		  64u
#define	Brightness	0xFF 
#define X_WIDTH 	  128u
#define Y_WIDTH 	  64u	    
 
//-----------------OLED端口定义----------------  					   

#define OLED_SCLK_Clr() gpio_bit_reset(GPIOC, GPIO_PIN_6) //SCL
#define OLED_SCLK_Set() gpio_bit_set(GPIOC, GPIO_PIN_6)

#define OLED_SDIN_Clr() gpio_bit_reset(GPIOC, GPIO_PIN_7) //SDA
#define OLED_SDIN_Set() gpio_bit_set(GPIOC, GPIO_PIN_7)

/*  SDA端口动态改变输入输出状态定义 */
#define OLED_SDIN_IN()  gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7)     //配置上拉输入
#define OLED_SDIN_OUT() { gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7); gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_7);}      //配置推挽输出

#define OLED_SCLK_Read() gpio_input_bit_get(GPIOC, GPIO_PIN_6)
#define OLED_SDIN_Read() gpio_input_bit_get(GPIOC, GPIO_PIN_7)

typedef struct 
{
	uint8_t x;
	uint8_t y;
}position;

typedef enum 
{
	WRdata=0,//写数据
	WRcmd=!WRdata,//写命令
}OLEDWriteMode;
 
typedef enum 
{
	mini=0,
	max=!mini,
}FontSize;
typedef enum 
{
	LineNot=0,
	LineYes=!LineNot,
}AddUnderLineYesOrNot; 

//模拟IIC控制函数
void IIC_GPIO_Config(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NoAck(void);
void Write_IIC_Byte(uint8_t IIC_Byte);
void Write_IIC_Command(uint8_t IIC_Command);
void Write_IIC_Data(uint8_t IIC_Data);
 
//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,OLEDWriteMode mode);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DisPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_DisChar(uint8_t x,uint8_t y,uint8_t chr,AddUnderLineYesOrNot mode);
void OLED_DisNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,AddUnderLineYesOrNot mode);
void OLED_DisString(uint8_t x,uint8_t y, uint8_t *p);	 
void OLED_Set_Position(uint8_t x, uint8_t y);

void OLED_DisChinese(uint8_t x,uint8_t y,char * p,uint8_t Num,FontSize fontSize);

void OLED_DisBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[]);
 

#endif  
	 
 
 



