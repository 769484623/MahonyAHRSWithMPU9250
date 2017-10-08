#ifndef _LCD
#define _LCD
#include "stm32f0xx.h"
#define LCD_RST_High() GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define LCD_RST_LOW() GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define LCD_CE_High() GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define LCD_CE_LOW() GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define LCD_DC_High() GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define LCD_DC_LOW() GPIO_ResetBits(GPIOA,GPIO_Pin_4)
void delay_1us(void);
void LCD_write_byte(uint8_t dat, uint8_t command);
void LCD_clear(void);
void LCD_init(void);
void LCD_Set_XY(uint8_t X, uint8_t Y);
void LCD_write_char(uint8_t c);
void LCD_E_Str(uint8_t X,uint8_t Y,char *s);
#endif
