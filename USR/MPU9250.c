#include "MPU9250.h"

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_CONFIG_2 0x1D

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		  0x75	//IIC地址寄存器(默认数值0x68，只读)


//****************************

#define	GYRO_ADDRESS   0xD0	  //陀螺地址
#define MAG_ADDRESS    0x18   //磁场地址
#define ACCEL_ADDRESS  0xD0
int16_t Acc[3] = {0}, GYO[3] = {0},Mag[3] = {0};
int32_t Gyo_Offset[3] = {0};
uint8_t Mag_Offset[3] = {0};
void MPU9250_Read()
{
	uint8_t i = 0x00;
	for(i = 0;i<3;i++)
	{
		I2C_ReadBytes(I2C1,GYRO_ADDRESS,ACCEL_XOUT_H + 2*i,2,(uint8_t*)(Acc + i),MSB);//Get The X Y Z Acc
		I2C_ReadBytes(I2C1,GYRO_ADDRESS,GYRO_XOUT_H + 2*i,2,(uint8_t*)(GYO + i),MSB);//Get The X Y Z Gyo
		I2C_ReadBytes(I2C1,MAG_ADDRESS,MAG_XOUT_L + 2 * i,2,(uint8_t*)(Mag + i),LSB);//Get The X Y Z Mag
		Mag[i] = Mag[i] * (((uint16_t)Mag_Offset[i] - 128)/256 + 1);//scale
		GYO[i] -= Gyo_Offset[i];//Calibrate
	}
}
void MPU9250_Calibration()
{
	uint8_t i , k;
	for(k = 0;k < 10;k++)
	{
		for(i = 0;i<3;i++)
		{
			I2C_ReadBytes(I2C1,GYRO_ADDRESS,GYRO_XOUT_H + 2*i,2,(uint8_t*)(GYO + i),MSB);
			Gyo_Offset[i] += GYO[i];
		}
	}
	for(i = 0;i<3;i++)
	{
		Gyo_Offset[i] /= 10;
		I2C_ReadBytes(I2C1,MAG_ADDRESS,0x10 + i,1,Mag_Offset + i,MSB);
	}
}
void Init_MPU9250()
{
	uint8_t Value = 0;
	uint8_t Reg = 0x00;
	I2C_WriteRegBytes(I2C1,GYRO_ADDRESS,PWR_MGMT_1,&Value,1);
	Value = 0x07;
	I2C_WriteRegBytes(I2C1,GYRO_ADDRESS,SMPLRT_DIV,&Value,1);
	Value = 0x01;
	I2C_WriteRegBytes(I2C1,GYRO_ADDRESS,CONFIG,&Value,1);
	Value = 0x10;//1000 degree
	I2C_WriteRegBytes(I2C1,GYRO_ADDRESS,GYRO_CONFIG,&Value,1);
	Value = 0x08;//4g 0.63HZ Fliter
	I2C_WriteRegBytes(I2C1,GYRO_ADDRESS,ACCEL_CONFIG,&Value,1);
	Value = 0x04;//1.94 ms
	I2C_WriteRegBytes(I2C1,GYRO_ADDRESS,ACCEL_CONFIG_2,&Value,1);
	Reg = 0x02;
	I2C_WriteRegBytes(I2C1,GYRO_ADDRESS,0x37,&Reg,1);
	Reg = 0x0F;//Fuse Mode
	I2C_WriteRegBytes(I2C1,MAG_ADDRESS,0x0A,&Reg,1);
	MPU9250_Calibration();
}
void MPU9250_Read_Pre()
{
	uint8_t Reg = 0x11;//Single time 16 bits
	I2C_WriteRegBytes(I2C1,MAG_ADDRESS,0x0A,&Reg,1);
}
