#include "printf.h"
#include "UART.h"

#include <stdio.h>

//Write a function that reads one character from the user and prints it. Run from main

uint8_t read_and_print_char() {
	uint8_t byte = USART_Read_Non_Blocking(USART2);
	if (byte != 0) {
		putchar(byte);
//		printf("%c", byte);
	}
	return byte;

//	printf("%c\r\n", byte);

}

void run_activity5() {
	for (;;) {
		read_and_print_char();
	}
}
