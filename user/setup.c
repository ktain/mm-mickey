#include "main.h"

/* Global variables */
volatile uint32_t Micros;	// volatile denotes no compiler optimations will be made
volatile uint32_t Millis;


void setup() {
	systick_setup();
	led_setup();
	button_setup();
	usart_setup();
	motor_setup();
	encoder_setup();
	sensor_setup();
}

void systick_setup(void) {  
	SystemInit();
	SystemCoreClockUpdate();

	if (SysTick_Config (SystemCoreClock / 1000)) // 1ms per interrupt
		while (1); 

	//set systick interrupt priority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	// 4 bits for preemp priority 0 bit for sub priority
	NVIC_SetPriority(SysTick_IRQn, 0);
	  
	Millis = 0;
}


void led_setup(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	// LEDs on GPIOA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Emitters on GPIOC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// Emitters on GPIOD 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Turn emitters off
	GPIO_ResetBits(GPIOC, GPIO_Pin_10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_11);
	GPIO_ResetBits(GPIOC, GPIO_Pin_12);
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}


void sensor_setup(void)
{	
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	
	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);       
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
}


uint16_t readADC(ADC_TypeDef* ADCx, uint8_t channel, uint8_t sampleTime)
{    
	ADC_RegularChannelConfig(ADCx, channel, 1, sampleTime);
	ADC_SoftwareStartConv(ADCx);
	while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADCx);
}


void button_setup(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	  /* Enable GPIOB clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	  /* Connect EXTI Line5 to PB5 pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);
	
    /* Configure PB5 pin as input floating */
	  /* Push button 0 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  /* Configure EXTI Line5 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
		
		/* Enable and set EXTI Line5 Interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
		
		
		/* Connect EXTI Line15_10 to PB11 pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
		
		/* Push button 1 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



void encoder_setup(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5|RCC_APB1Periph_TIM2, ENABLE);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
	
	TIM_SetAutoreload (TIM5, 0xffffffff);//0xffffffff is the max value for 32 bit, the autoreload value will be 0xffff for 16 bit timer
	TIM_SetAutoreload (TIM2, 0xffffffff);
	/* Configure the encoder */
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); 	// Left
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);	// Right
	/* TIM4 counter enable */
	TIM_Cmd(TIM5, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}


void motor_setup() {
	
	/* GPIO Setup */
  GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  /* GPIOB Configuration: TIM4 CH1 (PB6), TIM4 CH2 (PB7), TIM4 CH3 (PB8) and TIM4 CH4 (PB9) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  /* Connect TIM4 pins to AF */  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4); 
	
	
	/* Mode Setup */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//TIM4 clock = APB1 clock * 2 = 84MHz
	//Tim4 clock = (period+1) * (prescaler+1) * PWM_frequency
	//PWM frequency = 84000000 / (999+1) / (3+1) = 21000Hz = 21KHz
	//the value for TIM_TimeBaseStructure.TIM_Period is the scale for PWM
	//for example, if the value is 999, which mean the PWM is scaled from 0-999,
	//where motor don't move when at 0 and move output max power when at 999.
	//we actually devided pwm to 1000 parts evenly, however since it starts with 0 instead
	//of 1, the max is 999. the scale for the pwm is based on your reference. usually 999(1000)
	//is enough for a 6V faulfalber motor, you might need larger value if useing 3V version since it's more powerful

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = MOTORPERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = 3;	// 1 = 42kHz, 2 = 28kHz 3 = 21kHZ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


	//since we use all 4 channels for TIM4 as PWM. we need to set all 4 channels here.
	//even though there are likely 4 or more channels for each general purpose timer, the frequency can be one frequency for each timer
	//which means the PWM frequencies are all same for all 4 channel if all PWM pins are from TIM4
	//we change the duty cycle in order to change the motor PWM speed.
	//so we change the "TIM_OCInitStructure.TIM_Pulse"(TIM4->CCRx  x is 1 to 4) for duty cycle for each PWM channels
	//since the PWM frequency we set is 21KHz, so every 1/21000 second the Timer check the value for TIM4->CCRx to update the motor value)
	//motor is more responsive than your sensor sample rate in this case which is what we wanted
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (USESLOWDECAY)? MOTORPERIOD:0;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);


	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (USESLOWDECAY)? MOTORPERIOD:0;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (USESLOWDECAY)? MOTORPERIOD:0;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (USESLOWDECAY)? MOTORPERIOD:0;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);
}


void usart_setup(void) {
	
	// USART setup
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef USART_ClockInitstructure;

	/* Enable GPIOB clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* Enable USART1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//usart1 is on APB2 at 84Mhz max, usart2,3,6 are on APB1 at 42Mhz max

  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

  /* Configure USART as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
                                   //  Tx           Rx     
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_OverSampling8Cmd(USART1, ENABLE);

  USART_InitStructure.USART_BaudRate   = BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;
  USART_InitStructure.USART_Parity     = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode       =  USART_Mode_Rx | USART_Mode_Tx;

	/* USART Clock Initialization  */
  USART_ClockInitstructure.USART_Clock   = USART_Clock_Disable;
  USART_ClockInitstructure.USART_CPOL    = USART_CPOL_High;
  USART_ClockInitstructure.USART_LastBit = USART_LastBit_Disable;
  USART_ClockInitstructure.USART_CPHA    = USART_CPHA_1Edge;

  /* USART configuration */
  USART_Init(USART1, &USART_InitStructure);
  USART_ClockInit(USART1, &USART_ClockInitstructure);

  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}
