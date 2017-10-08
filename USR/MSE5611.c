#include "MSE5611.h"
#include <math.h>

#define MSE5611_Slave 0xEC

#define MSE5611_Reset 0x1E
#define Convert_D1_256 0x40
#define Convert_D1_512 0x42
#define Convert_D1_1024 0x44
#define Convert_D1_2048 0x46
#define Convert_D1_4096 0x48
#define Convert_D2_256 0x50
#define Convert_D2_512 0x52
#define Convert_D2_1024 0x54
#define Convert_D2_2048 0x56
#define Convert_D2_4096 0x58
#define ADC_Read 0x00

#define PROM_Read_0 0xA0
#define PROM_Read_1 0xA2
#define PROM_Read_2 0xA4
#define PROM_Read_3 0xA6
#define PROM_Read_4 0xA8
#define PROM_Read_5 0xAA
#define PROM_Read_6 0xAC
#define PROM_Read_7 0xAE

#define Sea_Level_Pressure 101325.0
uint16_t Reserve = 0;
uint16_t C[6] = {0};
uint16_t MSE5611_CRC = 0;
uint32_t D[2] = {0};

int32_t Temp = 0;
int32_t P = 0;
float Height = 0;
void Reset_MSE5611()
{
	I2C_WriteByte(I2C1,MSE5611_Slave,MSE5611_Reset);
	delay_ms(4);
}
void ReadPressure_Pre()
{
	I2C_WriteByte(I2C1,MSE5611_Slave,Convert_D1_4096);
}
void ReadPressure()
{
	I2C_ReadBytes(I2C1,MSE5611_Slave,ADC_Read,3,(uint8_t*)D,MSB);
}
void Single_Read_Pressure()
{
	ReadPressure_Pre();
	delay_ms(2);
	ReadPressure();
}
void ReadTemp_Pre()
{
	I2C_WriteByte(I2C1,MSE5611_Slave,Convert_D2_4096);
}
void ReadTemp()
{
	I2C_ReadBytes(I2C1,MSE5611_Slave,ADC_Read,3,(uint8_t*)( D + 1 ),MSB);
}
void Single_Read_Temp() 
{
	ReadTemp_Pre();
	delay_ms(2);
	ReadTemp();
}
void Pressure_Caculate()
{
	int64_t OFF = 0,OFF2 = 0,SENS = 0, SENS2 = 0,T2 = 0;
	int64_t dT = 0;
	dT = D[1] - (((int64_t)C[4]) << 8);
	Temp = 2000 + (((int64_t)dT * C[5])>>23);
	if(Temp < 2000)
	{
		T2 = ( dT*dT )>>31;
		OFF2 = 5 * (Temp - 2000) * (Temp - 2000) >> 1;
		SENS2 = 5 * (Temp - 2000) * (Temp - 2000) >> 2;
	}
	if(Temp < -1500)
	{
		OFF2 = OFF2 + 7 * (Temp + 1500) * (Temp + 1500);
		SENS2 = SENS2 + ( 11 * (Temp +1500) * (Temp +1500) >> 1 );
	}
	OFF = (((int64_t)C[1]) << 16) + ((C[3] * dT) >> 7);
	SENS = (((int64_t)C[0]) << 15) + ((C[2] * dT) >> 8);
	OFF -= OFF2;
	Temp -= T2;
	SENS -= SENS2;
	P = (((D[0] * SENS)>>21) - OFF)>>15;
	Height = 44330 * (1 - pow(P/Sea_Level_Pressure ,1 / 5.225));
}
void MSE5611_Config()
{
	Reset_MSE5611();
	I2C_ReadBytes(I2C1,MSE5611_Slave,PROM_Read_0,2,(uint8_t*)&Reserve,MSB);
	I2C_ReadBytes(I2C1,MSE5611_Slave,PROM_Read_1,2,(uint8_t*)C,MSB);
	I2C_ReadBytes(I2C1,MSE5611_Slave,PROM_Read_2,2,(uint8_t*)(C+1),MSB);
	I2C_ReadBytes(I2C1,MSE5611_Slave,PROM_Read_3,2,(uint8_t*)(C+2),MSB);
	I2C_ReadBytes(I2C1,MSE5611_Slave,PROM_Read_4,2,(uint8_t*)(C+3),MSB);
	I2C_ReadBytes(I2C1,MSE5611_Slave,PROM_Read_5,2,(uint8_t*)(C+4),MSB);
	I2C_ReadBytes(I2C1,MSE5611_Slave,PROM_Read_6,2,(uint8_t*)(C+5),MSB);
	I2C_ReadBytes(I2C1,MSE5611_Slave,PROM_Read_7,2,(uint8_t*)&MSE5611_CRC,MSB);
}
