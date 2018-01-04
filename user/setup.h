#ifndef SETUP_H
#define SETUP_H

// LED Pinout
#define LED1_ON    GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define LED2_ON    GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define LED3_ON    GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define LED4_ON    GPIO_SetBits(GPIOA, GPIO_Pin_7)

#define LED1_OFF   GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define LED2_OFF   GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define LED3_OFF   GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define LED4_OFF   GPIO_ResetBits(GPIOA, GPIO_Pin_7)

#define ALL_LED_ON		LED1_ON; LED2_ON; LED3_ON; LED4_ON;
#define ALL_LED_OFF		LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF;

// Emitter Pinout
#define LF_EM_ON   GPIO_SetBits(GPIOD, GPIO_Pin_2)
#define RF_EM_ON   GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define LD_EM_ON   GPIO_SetBits(GPIOC, GPIO_Pin_12)
#define RD_EM_ON   GPIO_SetBits(GPIOC, GPIO_Pin_11)

#define LF_EM_OFF  GPIO_ResetBits(GPIOD, GPIO_Pin_2)
#define RF_EM_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_10)
#define LD_EM_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_12)
#define RD_EM_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_11)


// ADC Pinout
#define read_Voltage       (readADC(ADC1, 2, ADC_SampleTime_28Cycles)*VOLTAGEMULTIPLIER)
#define read_LF_Sensor     readADC(ADC1, 13,  ADC_SampleTime_28Cycles)
#define read_LD_Sensor     readADC(ADC1, 12, ADC_SampleTime_28Cycles)
#define	read_RD_Sensor     readADC(ADC1, 11,  ADC_SampleTime_28Cycles)
#define read_RF_Sensor     readADC(ADC1, 10, ADC_SampleTime_28Cycles)

// Motor Timers
#define LPWMB TIM4->CCR4
#define LPWMA TIM4->CCR3
#define RPWMB TIM4->CCR1
#define RPWMA TIM4->CCR2

void setup(void);
void systick_setup(void);
void led_setup(void);
void button_setup(void);
void encoder_setup(void);
void sensor_setup(void);
void motor_setup(void);
void usart_setup(void);
uint16_t readADC(ADC_TypeDef* ADCx, uint8_t channel, uint8_t sampleTime);


#endif
