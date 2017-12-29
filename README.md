# MahonyAHRSWithMPU9250
MahonyAHRS with  STM32F0 &amp; MPU9250 &amp; MS5611 (Using hardware I2C)

Thanks to liangxingchao on CSDN, I realized that there are some tips I need to deliver to you other developers.

First is the relation between MPU9250_Read_Pre() and MPU9250_Read().
Before you read the MPU9250, you need to call MPU9250_Read_Pre(), and wait for about 8ms to let the 9250 get the mag data. In my code I didn't add delay just because the time of reading and calculating from MS5611 is almost 8ms.

And you need to change the para of Mahony_Init(float sampleFrequency), which is in main().
