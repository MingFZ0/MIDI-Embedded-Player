/*
 * project.c
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#include "UART.h"

#include "project.h"

int run(void) {
	char str2[20] = "Hello,";
	char str3[20] = "World!";
	char str4[10] = "\r\n";
	printf("%s %s %s", str2, str3, str4);
}
