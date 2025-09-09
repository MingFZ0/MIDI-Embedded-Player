/*
 * project.c
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#include "UART.h"
#include "printf.h"

#include "project.h"

void run_project() {
	char str2[20] = "Hello,";
	char str3[20] = "World!";
	char str4[10] = "\r\n";
	printf("%s %s %s", str2, str3, str4);
}
