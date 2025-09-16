// main.c with demo
// Updated and refactored Larry Kiser copyright 2021


// include project specific header files
#include "error_handler.h"
#include "SysClock.h"
#include "LED.h"
#include "demo.h"
#include "UART.h"
#include "project.h"
#include "activity4.h"

#include <stdio.h>

#include <stdarg.h>
	
int main(void){

	// initialization code
	System_Clock_Init(); // set System Clock = 80 MHz
	LED_Init();
	UART2_Init();
	
//	char buffer[64];
//	int n = sprintf(buffer, "Hello, World!\r\n");
//	USART_Write(USART2, (unsigned char *) buffer, n);

//	run_project();
	run();

	// application run function
//	run_demo() ;
}

