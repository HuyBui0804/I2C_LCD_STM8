#include "LCD_I2C.h"

uint8_t LCD_PIN;
uint32_t u32TempClear;
void delay_ms(uint32_t u32Ticks)
{
	while(u32Ticks)
	{
		SysTick	->CTRL &=~(1<<2);
		SysTick	->LOAD =9000;
		SysTick	->CTRL |=(1<<0);
		while((SysTick->CTRL &(1<<16))==0);
		SysTick	->CTRL &=~(1<<0);
		u32Ticks--;
	}
}

void Config_I2C_Pin(void)
{
	/*GPIOB Clock enable*/
	RCC->APB2ENR |=(1U<<3);
	
	/*Alternate function output Open-drain PB6[SCL]*/
	GPIOB->CRL |=(3U<<26);
	/*Output mode, max speed 50 MHz*/
	GPIOB->CRL |=(3U<<24);
	
	/*Alternate function output Open-drain PB7[SDA]*/
	GPIOB->CRL |=(3U<<30);
	/*Output mode, max speed 50 MHz*/
	GPIOB->CRL |=(3U<<28);
}


void I2C_Init(void)
{
	/*Enable clock I2C1*/
	RCC->APB1ENR |=(1U<<21);

	/*Program the peripheral input clock  8M*/
	I2C1->CR2 =0;
	I2C1->CR2 |= (8U<<0);

	/*Generate a 100 kHz SCL frequency*/
	I2C1->CCR = 0x28;

	/*Maximum rise time in Fm/Sm*/
	I2C1->TRISE = 0x09;

	/*Enable peripheral[I2C]*/
	I2C1->CR1 |=(1U<<0);
}
	
void I2C_Write(uint8_t u8Adress,uint8_t u8Data)
{
	/*Waiting no communication on the bus before generate a Start condition*/
	while (I2C1->SR2 &(1U<<1));
	/*Generate a Start condition*/
	I2C1->CR1 |=(1U<<8);
	
	/*Cleared SB and writing DR register with Address*/
	while((I2C1->SR1&(1U<<0))==0);

	u32TempClear = I2C1->SR1;
	/*Wite is 0 read is 1*/
	I2C1->DR = (u8Adress<<1)|0;
	
	/*Cleared SR1 SR2*/
	while((I2C1->SR1&(1U<<1))==0);
	
	u32TempClear = I2C1->SR1;
	u32TempClear = I2C1->SR2;
	
	/*Waiting TX is not Empty*/
	while((I2C1->SR1&(1U<<7)) ==0);
	/*Writing Data */
	I2C1->DR = u8Data;
	/*waiting data byte transfer succeeded*/
	while((I2C1->SR1&(1U<<2)) ==0);

	/*Program Stop request*/
	I2C1->CR1 |=(1U<<9);
}


void LCD_I2C_Enable(void)
{
	LCD_PIN |= (1<<PIN_E);
	I2C_Write(PCF8574_ADRESS,LCD_PIN);
	delay_ms(1);
	LCD_PIN &=~(1<<PIN_E);
	I2C_Write(PCF8574_ADRESS,LCD_PIN);
	delay_ms(1);
}

void LCD_Send_8Bits(uint8_t u8DataSend)
{
	LCD_PIN =(LCD_PIN&0x0F)|(u8DataSend&0xF0);
	LCD_I2C_Enable();
	LCD_PIN =(LCD_PIN&0x0F)|(u8DataSend <<4 );
	LCD_I2C_Enable();
}

void LCD_I2C_Command(uint8_t u8DataCmd)
{
	LCD_PIN &=~(1<<PIN_RS);
	LCD_Send_8Bits(u8DataCmd);
}

void LCD_I2C_Data(uint8_t u8DataData)
{
	LCD_PIN |= (1<<PIN_RS);
	LCD_Send_8Bits(u8DataData);
}


void LCD_I2C_Init(void)
{
	delay_ms(2);
	LCD_PIN |=(1<<PIN_POWER);
	LCD_PIN &=~(1<<PIN_RW); 
	delay_ms(2);
	LCD_I2C_Command(0x33);
	delay_ms(10);
	LCD_I2C_Command(0x32);
	delay_ms(10);
	LCD_I2C_Command(0x28);
	delay_ms(2);
	LCD_I2C_Command(0x0C);
	delay_ms(1);
	LCD_I2C_Command(0x01);
	delay_ms(10);
}

void LCD_I2C_String(char *u8String)
{
	uint32_t i;
	for(i=0;u8String[i]!=0;i++)  
	{
		LCD_I2C_Data (u8String[i]);
	}
}
	

















