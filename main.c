#include "stm32f10x.h"                  // Device header
#include "LCD_I2C.h"

uint8_t display_num =0;
int main(void)
{
	Config_I2C_Pin();
	I2C_Init();
	LCD_I2C_Init();
	LCD_I2C_Command(0x80);
	LCD_I2C_String("Lap Trinh");
	LCD_I2C_Command(0xC0);
	LCD_I2C_String("Nhung AZ");
	while(1)
	{

		display_num++;
		if(display_num ==9) display_num=0;
		LCD_I2C_Command(0xCA);
		LCD_I2C_Data(display_num+0x30);
		delay_ms(1000);
	}
}













