#ifndef MSE5611
#define MSE5611
#include "stm32f0xx.h"

void MSE5611_Config(void);
void ReadPressure_Pre(void);
void ReadPressure(void);
void ReadTemp_Pre(void);
void ReadTemp(void);

void Single_Read_Temp(void);
void Single_Read_Pressure(void);
void Pressure_Caculate(void);

#endif
