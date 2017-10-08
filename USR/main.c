#include "main.h"
#include "stdio.h"
#include <math.h>
#include <string.h>
#define Gravity 9.8
#define Factor 0.030517 
//Factor = 1000/32768
#define PI 3.1415926
extern float Height;
extern int16_t Acc[3], GYO[3],Mag[3];
void Sensor_Read()
{
	MPU9250_Read_Pre();
	Single_Read_Temp();//2ms
	ReadPressure_Pre();
	MPU9250_Read();
	Mahony_update(GYO[0] * Factor,GYO[1] * Factor,GYO[2] * Factor,
								Acc[0],Acc[1],Acc[2],
								Mag[0],Mag[1],Mag[2]);
	Mahony_computeAngles();
	ReadPressure();
	Pressure_Caculate();
}
int main(void)
{
	InitSystem();
	Mahony_Init(150);
	LCD_clear();
  while (1)
  {
		char T[10] = {0};
		Sensor_Read();
		sprintf(T,"%.2f",getRoll());
		LCD_E_Str(0,0,T);
		memset(T,0,10);
		sprintf(T,"%.2f",getPitch());
		LCD_E_Str(0,1,T);
		memset(T,0,10);
		sprintf(T,"%.2f",getYaw());
		LCD_E_Str(0,2,T);
		memset(T,0,10);
		sprintf(T,"%.2f",Height);
		LCD_E_Str(0,3,T);
		memset(T,0,10);
		//LCD_clear();
  }
}
