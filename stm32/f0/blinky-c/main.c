/* STM32F0 CMSIS Blinky */

#include "stm32f0xx.h"

void SystemInit(void) 
{
	/* HSI for now */
}

int main(void) 
{
	/* enable GPIOC clock */

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;     
	
	// PC8 output
	
	GPIOC->MODER |= (1 << (8 * 2));        

	while (1) {

		GPIOC->ODR ^= (1 << 8);            

		for (volatile int i = 0; i < 50000; i++);
    }
}
