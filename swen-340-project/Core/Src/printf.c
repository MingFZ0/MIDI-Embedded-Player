#include "UART.h"
#include "string.h"
#include <stdio.h>
#include <stdarg.h>

char buffer[120];

int putchar(int value) {
	USART_Write(USART2, (unsigned char*) &value, 1);
	if (value == '\r') {
		USART_Write(USART2, (unsigned char*) "\n", 1);
	}
	return 1;
}

int puts(const char* string) {
	int ret = strlen (string);
	USART_Write (USART2, (unsigned char*) string, ret);
	USART_Write (USART2, (unsigned char*) "\n", ret);
	return ret + 1;
}

int printf(const char *format, ...) {
	va_list aptr;
	int ret;
	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);
	USART_Write(USART2, (unsigned char *) buffer, ret);
	return(ret);
}
