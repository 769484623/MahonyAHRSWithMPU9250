#include "I2C_Libary.h"
#include <stdlib.h>
#include <string.h>
#define MAX_ERROR_NUM 8
uint16_t Current_Slave_Addr = 0;
I2C_Error_TypeDef Error = {0};
void I2C_Wait_Flag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG ,FlagStatus Exit_Term,uint16_t Waiting_Time)
{
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != Exit_Term && Waiting_Time > 0)
	{
		Waiting_Time--;
	}
	if(Waiting_Time)
	{
		
	}
}
void I2C_WriteByte(I2C_TypeDef * I2Cx,uint8_t Slave_Addr,uint8_t Data)
{
	Current_Slave_Addr = Slave_Addr;
	I2C_Wait_Flag(I2Cx, I2C_FLAG_BUSY,RESET,255);//IF BUSY
	I2C_TransferHandling(I2Cx,Slave_Addr,1,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);//If Write OK
	I2C_SendData(I2Cx,Data);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);
}
void I2C_WriteRegBytes_1(I2C_TypeDef * I2Cx , uint8_t Slave_Addr,uint8_t Reg,uint8_t* Data,uint8_t Length)
{
	uint8_t i = 0;
	Current_Slave_Addr = Slave_Addr;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);//IF BUSY
	I2C_TransferHandling(I2Cx,Slave_Addr,1,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);//If Write OK
	I2C_SendData(I2Cx,Reg);
	I2C_TransferHandling(I2Cx,Slave_Addr ,Length ,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);
	for(i = 0;i < Length;i++)
	{
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);//If Write OK
		I2C_SendData(I2Cx,Data[i]);
	}
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);
}
void I2C_WriteRegBytes(I2C_TypeDef * I2Cx , uint8_t Slave_Addr,uint8_t Reg,uint8_t* Data,uint8_t Length)
{
	uint8_t i = 0;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);//IF BUSY
	I2C_TransferHandling(I2Cx,Slave_Addr,Length + 1,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);//If Write OK
	I2C_SendData(I2Cx,Reg);
	//I2C_TransferHandling(I2Cx,Slave_Addr ,Length ,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);---->mpu6050 need to delete this
	for(i = 0;i < Length;i++)
	{
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);//If Write OK
		I2C_SendData(I2Cx,Data[i]);
	}
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);
}
void I2C_ReadBytes(I2C_TypeDef * I2Cx ,uint8_t Slave_Addr,uint8_t Register,uint8_t Length,uint8_t* Array,enum LMSB Word)
{
	uint8_t i = 0;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);//IF BUSY
	I2C_TransferHandling(I2Cx,Slave_Addr,1,I2C_SoftEnd_Mode,I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);//If Write OK
	I2C_SendData(I2Cx,Register);
	I2C_TransferHandling(I2Cx,Slave_Addr ,Length ,I2C_AutoEnd_Mode,I2C_Generate_Start_Read);
	for(i = 0;i < Length;i++)
	{
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
		if(Word == MSB)
		{
			Array[Length - i - 1] = I2C_ReceiveData(I2Cx);
		}
		else
		{
			Array[i] = I2C_ReceiveData(I2Cx);
		}
	}
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);
}
