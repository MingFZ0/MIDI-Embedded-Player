#include "GPIO.h"
#include <stdint.h>
#include "printf.h"
#include "UART.h"
#include "stm32l476xx.h"

static uint8_t COMMAND[32];
static int i = 0;

uint8_t* get_command() {
	return COMMAND;
}

void USART2_IRQHandler(void) {
	uint8_t chr = USART_Read(USART2);
	printf("%c", (signed char) chr);
	COMMAND[i++] = chr;
}

void homework6_run() {
	NVIC_EnableIRQ(USART2_IRQn);
	USART2->CR1 |= (1<<5);
}
