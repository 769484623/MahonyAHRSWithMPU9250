#ifndef _BSP
#define _BSP
#include "stm32f0xx.h"
//Init System Peri

void InitSystem(void);
void Alive_LED(void);
static void InitClock(void);
static void InitGPIO(void);
static void InitSPI(void);
static void InitI2C(void);
static void InitTimer(void);
void delay_ms(uint32_t nTime);
#endif
