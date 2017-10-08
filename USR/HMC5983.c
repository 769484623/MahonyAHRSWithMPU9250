#include "HMC5983.h"
int16_t Mag[3] = {0};
void HMC5983_WriteByte(uint8_t Register,uint8_t Data)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);//IF BUSY
	I2C_TransferHandling(I2C1,HMC5983_Slave,2,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);//If Write OK
	I2C_SendData(I2C1,Register);
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);//If Write OK
	I2C_SendData(I2C1,Data);
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET);
}
void HMC5983_Config(void)
{
	HMC5983_WriteByte(HMC5983_ConfigA,CRA);
	HMC5983_WriteByte(HMC5983_ConfigB,CRB);
	HMC5983_WriteByte(HMC5983_Mode,MR);
}
void HMC5983_Read_Mag()
{
	I2C_ReadBytes(I2C1,HMC5983_Slave,HMC5983_Data_X_MSB,2,(uint8_t*)Mag,MSB);
	I2C_ReadBytes(I2C1,HMC5983_Slave,HMC5983_Data_Y_MSB,2,(uint8_t*)(Mag+1),MSB);
	I2C_ReadBytes(I2C1,HMC5983_Slave,HMC5983_Data_Z_MSB,2,(uint8_t*)(Mag+2),MSB);
}
