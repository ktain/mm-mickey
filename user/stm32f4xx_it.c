#include "main.h"


void SysTick_Handler(void)
{
	Millis++;
	
	systick();	
}


// Button 0
void EXTI9_5_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) {
		button0();
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}


// Button 1
void EXTI15_10_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line10) != RESET) {
		button1();
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
}




