
#ifndef __BH1750_H
#define __BH1750_H		

//  SCL   PC0
//  SDA   PC1
    
#define BH1750_ADDR							0x46  //定义器件在IIC总线中的从地址

//***************BH1750内部寄存器地址***********
#define BH1750_POWER_DOWN				0x00  //关闭模块
#define BH1750_POWER_ON					0x01  //打开模块等待测量指令
#define BH1750_RESET						0x07  //重置数据寄存器值在PowerOn模式下有效 
#define BH1750_CONT_H_MODE			0x10  //高分辨率 单位1lx 测量时间120ms
#define BH1750_CONT_H_MODE2			0x11  //高分辨率模式2 单位0.5lx 测量时间120ms
#define BH1750_CONT_L_MODE			0x13  //低分辨率 单位4lx 测量时间16ms
#define	BH1750_ONE_H_MODE				0x20  //一次高分辨率 测量 测量后模块转到 PowerDown模式
#define BH1750_ONE_H_MODE2			0x21  //同上类似
#define BH1750_ONE_L_MODE				0x23  //同上类似 
#define BH1750_CHG_MEAS_TIME_H	0x40
#define BH1750_CHG_MEAS_TIME_L	0x60

//************************************
/*模拟IIC输入输出端口定义*/
#define BH1750_SCL_H         gpio_bit_set(GPIOC, GPIO_PIN_0)
#define BH1750_SCL_L         gpio_bit_reset(GPIOC, GPIO_PIN_0)
   
#define BH1750_SDA_H         gpio_bit_set(GPIOC, GPIO_PIN_1)
#define BH1750_SDA_L         gpio_bit_reset(GPIOC, GPIO_PIN_1)

//IO方向设置
#define BH1750_SDA_IN()  gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1)	//PC1输入模式
#define BH1750_SDA_OUT() { gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1); gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_1);}  //PC1输出模式

#define BH1750_SCL_read      gpio_input_bit_get(GPIOC, GPIO_PIN_0)
#define BH1750_SDA_read      gpio_input_bit_get(GPIOC, GPIO_PIN_1)
    
  
//BH1750 IIC控制函数
void BH1750_I2C_GPIO_Config(void);
void BH1750_I2C_Delay(void);
uint8_t BH1750_I2C_Start(void);
void BH1750_I2C_Stop(void);
void BH1750_I2C_Ack(void);
void BH1750_I2C_NoAck(void);
uint8_t BH1750_I2C_WaitAck(void);
void I2C_BH1750_SendByte(uint8_t SendByte);
unsigned char BH1750_I2C_RadeByte(void);
uint8_t BH1750_Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char BH1750_Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);
uint8_t BH1750_Write_Cmd(unsigned char cmd);
void Init_BH1750(void);
uint16_t READ_BH1750(void);

#endif  
