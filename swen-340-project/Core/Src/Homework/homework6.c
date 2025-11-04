#include "GPIO.h"
#include <stdint.h>
#include "printf.h"
#include "UART.h"
#include "stm32l476xx.h"

void USART2_IRQHandler(void) {
	printf("Character: %c\r\n", USART2->RDR);
}

void homework6_run() {
	NVIC_SetPriority(USART2_IRQn, 0);
	NVIC_EnableIRQ(USART2_IRQn);
	USART2->CR1 |= (1<<5);
}
