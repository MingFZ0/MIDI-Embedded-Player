// Write a function that turns on your led when the user types 0
// Bonus turn off when they press f

#include "LED.h"
#include "demo.h"
#include "UART.h"
#include "activity5.h"
#include "printf.h"

#include <stdio.h>

void activity7_run() {
	while (1) {
		uint8_t byte = USART_Read_Non_Blocking(USART2);
		if (byte == 'o')
			LED_On();
		if (byte == 'f')
			LED_Off();
		if (byte != 0) {putchar(byte);}
	}
}
