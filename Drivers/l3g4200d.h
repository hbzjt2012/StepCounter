
#ifndef __L3G4200D_H
#define __L3G4200D_H		

//  3V3   PC8
//  SCL   PC9
//  SDA   PC10
//  SAO   PC11
//  GND   PC12
    
#define TRUE 1
#define FALSE 0
    
#define	L3G4200_Addr   0xD2 //定义器件在IIC总线中的从地址,根据ALT ADDRESS地址引脚不同修改
//***************L3G4200D内部寄存器地址***********
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define REFERENCE 0x25
#define OUT_TEMP 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define INT1_CFG 0x30
#define INT1_SRC 0x31
#define INT1_TSH_XH 0x32
#define INT1_TSH_XL 0x33
#define INT1_TSH_YH 0x34
#define INT1_TSH_YL 0x35
#define INT1_TSH_ZH 0x36
#define INT1_TSH_ZL 0x37
#define INT1_DURATION 0x38

//************************************
/*模拟IIC输入输出端口定义*/
#define SCL_H         gpio_bit_set(GPIOC, GPIO_PIN_9)
#define SCL_L         gpio_bit_reset(GPIOC, GPIO_PIN_9)
   
#define SDA_H         gpio_bit_set(GPIOC, GPIO_PIN_10)
#define SDA_L         gpio_bit_reset(GPIOC, GPIO_PIN_10)
    
#define SAO_H         gpio_bit_set(GPIOC, GPIO_PIN_11)
#define SAO_L         gpio_bit_reset(GPIOC, GPIO_PIN_11)
    
#define VCC_3V3_H     gpio_bit_set(GPIOC, GPIO_PIN_8)
#define VCC_3V3_L     gpio_bit_reset(GPIOC, GPIO_PIN_8)

#define GND_H     gpio_bit_set(GPIOC, GPIO_PIN_12)
#define GND_L     gpio_bit_reset(GPIOC, GPIO_PIN_12)

//IO方向设置
#define SDA_IN()  gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_10)	//PC10输入模式
#define SDA_OUT() { gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_10); gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_10);}  //PC10输出模式

#define SCL_read      gpio_input_bit_get(GPIOC, GPIO_PIN_9)
#define SDA_read      gpio_input_bit_get(GPIOC, GPIO_PIN_10)
    
  
//L3G4200D IIC控制函数
void I2C_GPIO_Config(void);
void I2C_Delay(void);
uint8_t I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
uint8_t I2C_WaitAck(void);
void I2C_SendByte(uint8_t SendByte);
unsigned char I2C_RadeByte(void);
uint8_t Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);
void Init_L3G4200D(void);
void READ_L3G4200D(void);
void Send_data(uint8_t axis);
void DATA_printf(unsigned char *s,short temp_data);

#endif  
