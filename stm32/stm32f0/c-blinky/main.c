/* STM32F0 CMSIS Blinky */

#include "stm32f0xx.h"

int main(void) 
{
	/* enable GPIOC clock */

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;     
	
	GPIOC->MODER |= (1 << (8 * 2));        // PC8 output

	while (1) {

		GPIOC->ODR ^= (1 << 8);            

		for (volatile int i = 0; i < 50000; i++);
    }
}
