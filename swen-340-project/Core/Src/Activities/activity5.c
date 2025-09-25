#include "printf.h"
#include "UART.h"

#include <stdio.h>

//Write a function that reads one character from the user and prints it. Run from main

void read_and_print_char() {
	uint8_t byte = USART_Read(USART2);
//	printf("%c\r\n", byte);
	putchar(byte);
}

void run_activity5() {
	for (;;) {
		read_and_print_char();
	}
}
