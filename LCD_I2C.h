#ifndef _LCD_I2C_H_
#define _LCD_I2C_H_
#include "stm32f10x.h" 
#define PCF8574_ADRESS 0x27


#define	PIN_RS 		0
#define PIN_RW		1
#define PIN_E		2
#define PIN_POWER 	3


void delay_ms(uint32_t u32Ticks);
void Config_I2C_Pin(void);
void I2C_Init(void);
void I2C_Write(uint8_t u8Adress,uint8_t u8Data);
void LCD_I2C_Enable(void);
void LCD_Send_8Bits(uint8_t u8DataSend);
void LCD_I2C_Command(uint8_t u8DataCmd);
void LCD_I2C_Data(uint8_t u8DataData);
void LCD_I2C_Init(void);
void LCD_I2C_String(char *u8String);

#endif
