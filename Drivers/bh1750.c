
#include "main.h"
#include "systick.h"
#include <stdio.h>

char  BH1750_test=0; 

/*******************************************************************************
* Function Name  : BH1750_I2C_GPIO_Config
* Description    : Configration Simulation IIC GPIO
* Input          : None 
* Output         : None
* Return         : None
****************************************************************************** */
void BH1750_I2C_GPIO_Config(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);   //ʹ��GPIOCʱ��
	gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0|GPIO_PIN_1); 
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0|GPIO_PIN_1); 		
}

/*******************************************************************************
* Function Name  : BH1750_I2C_delay
* Description    : Simulation IIC Timing series delay
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void BH1750_I2C_delay(void)
{
		
   uint8_t i=30; //��������Ż��ٶ�
   while(i) 
   { 
     i--; 
   }  
}

/*******************************************************************************
* Function Name  : BH1750_I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
uint8_t BH1750_I2C_Start(void)
{
	BH1750_SDA_OUT();
	BH1750_SDA_H;
	BH1750_SCL_H;
	BH1750_I2C_delay();
	if(!BH1750_SDA_read)return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	BH1750_SDA_L;
	BH1750_I2C_delay();
	if(BH1750_SDA_read) return FALSE;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
	BH1750_SDA_L;
	BH1750_I2C_delay();
	return TRUE;
}

/*******************************************************************************
* Function Name  : BH1750_I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void BH1750_I2C_Stop(void)
{
	BH1750_SDA_OUT();
	BH1750_SCL_L;
	BH1750_I2C_delay();
	BH1750_SDA_L;
	BH1750_I2C_delay();
	BH1750_SCL_H;
	BH1750_I2C_delay();
	BH1750_SDA_H;
	BH1750_I2C_delay();
} 

/*******************************************************************************
* Function Name  : BH1750_I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void BH1750_I2C_Ack(void)
{	
	BH1750_SCL_L;
	BH1750_I2C_delay();
	BH1750_SDA_OUT();
	BH1750_SDA_L;
	BH1750_I2C_delay();
	BH1750_SCL_H;
	BH1750_I2C_delay();
	BH1750_SCL_L;
	BH1750_I2C_delay();
}

/*******************************************************************************
* Function Name  : BH1750_I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void BH1750_I2C_NoAck(void)
{	
	BH1750_SCL_L;
	BH1750_I2C_delay();
	BH1750_SDA_OUT();
	BH1750_SDA_H;
	BH1750_I2C_delay();
	BH1750_SCL_H;
	BH1750_I2C_delay();
	BH1750_SCL_L;
	BH1750_I2C_delay();
} 
/*******************************************************************************
* Function Name  : BH1750_I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
uint8_t BH1750_I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	BH1750_SCL_L;
	BH1750_I2C_delay();
	BH1750_SDA_IN();
	BH1750_SDA_H;			
	BH1750_I2C_delay();
	BH1750_SCL_H;
	BH1750_I2C_delay();
	if(BH1750_SDA_read)
	{
      BH1750_SCL_L;
	  BH1750_I2C_delay();
      return FALSE;
	}
	BH1750_SCL_L;
	BH1750_I2C_delay();
	return TRUE;
}

/*******************************************************************************
* Function Name  : BH1750_I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void BH1750_I2C_SendByte(uint8_t SendByte) //���ݴӸ�λ����λ//
{
    uint8_t i=8;
		BH1750_SDA_OUT();
    while(i--)
    {
        BH1750_SCL_L;
        BH1750_I2C_delay();
      if(SendByte&0x80)
        BH1750_SDA_H;  
      else 
        BH1750_SDA_L;   
        SendByte<<=1;
        BH1750_I2C_delay();
				BH1750_SCL_H;
        BH1750_I2C_delay();
    }
    BH1750_SCL_L;
}

/*******************************************************************************
* Function Name  : BH1750_I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
unsigned char BH1750_I2C_RadeByte(void)  //���ݴӸ�λ����λ//
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;
		
		BH1750_SDA_IN();
    BH1750_SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      BH1750_SCL_L;
      BH1750_I2C_delay();
			BH1750_SCL_H;
      BH1750_I2C_delay();	
      if(BH1750_SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    BH1750_SCL_L;
    return ReceiveByte;
} 

//���ֽ�д��*******************************************
uint8_t BH1750_Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!BH1750_I2C_Start())return FALSE;
    BH1750_I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//BH1750_I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!BH1750_I2C_WaitAck()){BH1750_I2C_Stop(); return FALSE;}
    BH1750_I2C_SendByte(REG_Address );   //���õ���ʼ��ַ
    BH1750_I2C_WaitAck();	
    BH1750_I2C_SendByte(REG_data);
    BH1750_I2C_WaitAck();   
    BH1750_I2C_Stop(); 
    delay_1ms(5);
    return TRUE;
}

//���ֽڶ�ȡ*****************************************
unsigned char BH1750_Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
	unsigned char REG_data;     	
	if(!BH1750_I2C_Start())return FALSE;
    BH1750_I2C_SendByte(SlaveAddress); //BH1750_I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!BH1750_I2C_WaitAck()){BH1750_I2C_Stop();BH1750_test=1; return FALSE;}
    BH1750_I2C_SendByte((uint8_t) REG_Address);   //���õ���ʼ��ַ 
    BH1750_I2C_WaitAck();
    BH1750_I2C_Start();
    BH1750_I2C_SendByte(SlaveAddress+1);
    BH1750_I2C_WaitAck();

		REG_data= BH1750_I2C_RadeByte();
    BH1750_I2C_NoAck();
    BH1750_I2C_Stop();
    //return TRUE;
	return REG_data;
}

//��������*****************************************
uint8_t BH1750_Write_Cmd(unsigned char cmd)		    
{
  	if(!BH1750_I2C_Start())return FALSE;
    BH1750_I2C_SendByte(BH1750_ADDR);   
    if(!BH1750_I2C_WaitAck()){BH1750_I2C_Stop(); return FALSE;}
    BH1750_I2C_SendByte(cmd );   //���õ���ʼ��ַ
    BH1750_I2C_WaitAck();	 
    BH1750_I2C_Stop(); 
    delay_1ms(5);
    return TRUE;
}

 //************��ʼ��BH1750*********************************
void Init_BH1750(void)
{
	BH1750_I2C_GPIO_Config();
	
  delay_1ms(200);
	
	BH1750_Write_Cmd(BH1750_POWER_ON);   			//������������
	BH1750_Write_Cmd(BH1750_RESET);      			//����Ĵ������� 
	BH1750_Write_Cmd(BH1750_CONT_H_MODE2);    //���ø߾���Ϊģʽ2 
	
	delay_1ms(200);

}	
//******��ȡBH1750����****************************************
uint16_t READ_BH1750(void)
{
	uint16_t buf;
	
	BH1750_I2C_Start();                      //iic��ʼ�ź�
	BH1750_I2C_SendByte(BH1750_ADDR+1);      //����������ַ+����־λ
	BH1750_I2C_WaitAck();                    //�ȴ��ӻ�Ӧ��
	buf = BH1750_I2C_RadeByte();             //��ȡ����
	BH1750_I2C_WaitAck(); 
	buf = buf << 8;                          //��ȡ������߰�λ����
	buf+=0x00ff&BH1750_I2C_RadeByte();       //��ȡ������ڰ�λ����
	BH1750_I2C_NoAck();
	BH1750_I2C_Stop();
	
	return buf;
}


