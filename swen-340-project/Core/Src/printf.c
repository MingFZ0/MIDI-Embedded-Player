#include "UART.h"
#include <stdio.h>
#include <stdarg.h>

char buffer[80];

int vspfunc(char *format, ...) {
	va_list aptr;
	int ret;
	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);
	USART_Write(USART2, (unsigned char *) buffer, ret);
	return(ret);
}
