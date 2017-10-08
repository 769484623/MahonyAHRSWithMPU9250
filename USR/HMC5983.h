#ifndef HMC5983
#define HMC5983
#include "stm32f0xx.h"

#define HMC5983_Slave 0x3C
#define HMC5983_ConfigA 0x00
#define HMC5983_ConfigB 0x01
#define HMC5983_Mode 0x02
#define HMC5983_Data_X_MSB 0x03
#define HMC5983_Data_Z_MSB 0x05
#define HMC5983_Data_Y_MSB 0x07
#define HMC5983_Status 0x09
#define HMC5983_ID_A 0x0A
#define HMC5983_ID_B 0x0B
#define HMC5983_ID_C 0x0C
#define HMC5983_Temp_MSB 0x31

#define CRA 0x70
#define CRB 0x00
#define MR 0x00

void HMC5983_Config(void);
void HMC5983_Read_Mag(void);
#endif
