#include "BSP.h"
void delay_ms(uint32_t nTime)//TIM3使用率并不大，基本上3s使用一次，阻塞式定时并不太好，
{
	TIM6->CR1 |= TIM_CR1_CEN;//开启TIM3
	while(nTime--)
	{
		while((TIM6->SR & TIM_FLAG_Update)==RESET);
		TIM6->SR &= ~TIM_FLAG_Update;
	}
	TIM6->CR1 &= ~TIM_CR1_CEN;//关闭TIM3
}
static void InitClock(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);  //使能GPIOA、B、F的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);	 										 										 //使能I2C1的时钟
	RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);																				 //使能SPI1的时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);																			 //使能Sys_Clk
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM15|RCC_APB2Periph_TIM16|RCC_APB2Periph_TIM17,ENABLE);//使能TIM1、TIM15 PWM      16、17 SPARE
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM14,ENABLE);  //使能TIM3、6、14
}
static void InitGPIO(void)
{
	GPIO_InitTypeDef GPIO_InitA;
	/*Init SPI GPIO,SCK,M0SI*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);
	GPIO_InitA.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitA.GPIO_OType=GPIO_OType_PP;
	GPIO_InitA.GPIO_PuPd=GPIO_PuPd_DOWN;																							//UP is Better?
	GPIO_InitA.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitA.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitA);
	
	/*Init PWM A8 9 10 11*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_2);
	
	GPIO_InitA.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitA.GPIO_OType=GPIO_OType_PP;
	GPIO_InitA.GPIO_PuPd=GPIO_PuPd_UP;																							//UP is Better?
	GPIO_InitA.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitA.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 ;
	GPIO_Init(GPIOA,&GPIO_InitA);
	
	/*Init PWM B14 15 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_1);
	
	GPIO_InitA.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitA.GPIO_OType=GPIO_OType_PP;
	GPIO_InitA.GPIO_PuPd=GPIO_PuPd_UP;																							//UP is Better?
	GPIO_InitA.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitA.GPIO_Pin=GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_Init(GPIOB,&GPIO_InitA);
	
	/*Init CSN SDN*/
	GPIO_InitA.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitA.GPIO_PuPd=GPIO_PuPd_UP;																							//UP is Better?
	GPIO_InitA.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_InitA);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
	/* INIT I2C1 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_1);	//配置PB8 成第二功能引脚	I2C1_SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_1);	//配置PB9 成第二功能引脚	I2C1_SDA
	
	GPIO_InitA.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitA.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitA.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitA.GPIO_OType = GPIO_OType_PP;
	GPIO_InitA.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitA);
}
static void InitSPI()
{
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_Direction=SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_RxFIFOThresholdConfig(SPI1,SPI_RxFIFOThreshold_QF);
	SPI_Cmd(SPI1,ENABLE);
}
static void InitI2C()
{
	I2C_InitTypeDef I2C_InitStructure;
  I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x02;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_Timing = 0x0090273D;
  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);
}
void sTick()//start Sys tick 20ms
{
	if(SysTick_Config(SystemCoreClock/50))//10,100ms;50,20ms;1000,1ms;100000,10us;1000000,1us;
	{
		while(1);
	}
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
void Alive_LED() //200ms
{
	static uint8_t Counter = 0;
	if(Counter >= 10)
	{
		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1))
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		}
		else
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
		}
		Counter = 0;
	}
	Counter++;
}
void PWM_Config()
{
	TIM_TimeBaseInitTypeDef tInit;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	tInit.TIM_Period=(SystemCoreClock / 48000 ) - 1;
	tInit.TIM_ClockDivision=0;
	tInit.TIM_Prescaler=160 - 1;
	tInit.TIM_CounterMode=TIM_CounterMode_Up;
	tInit.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&tInit);
	TIM_TimeBaseInit(TIM15,&tInit);
	//20ms
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = tInit.TIM_Period * 0.05;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	//TIM1
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = tInit.TIM_Period * 0.075;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = tInit.TIM_Period * 0.1;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = tInit.TIM_Period * 0.125;
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	//TIM15
	TIM_OCInitStructure.TIM_Pulse = tInit.TIM_Period * 0.05;
	TIM_OC1Init(TIM15, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = tInit.TIM_Period * 0.075;
	TIM_OC2Init(TIM15, &TIM_OCInitStructure);
	
  TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM15, ENABLE);
	
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM15, ENABLE);
}
void MPU6050_TimerInit()
{
	TIM_TimeBaseInitTypeDef tInit;
	tInit.TIM_Period=9;//10 - 1,1ms
	tInit.TIM_ClockDivision=TIM_CKD_DIV1;
	tInit.TIM_Prescaler=4800 - 1;//APB2 4800 分频
	tInit.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6,&tInit);
}
void InitSystem(void)
{
	InitClock();
	InitGPIO();
	InitSPI();
	InitI2C();
	PWM_Config();
	MPU6050_TimerInit();
	Init_MPU9250();
	MSE5611_Config();
	LCD_init();
	//sTick();
}
