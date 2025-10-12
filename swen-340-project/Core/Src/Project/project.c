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
//	char str2[20] = "Hello,";
//	char str3[20] = "World!";
//	char str4[10] = "\r\n";
//	printf("%s %s %s", str2, str3, str4);
	display_menu();
}

void display_menu() {
//	char menu[20] = "***REMOTE LED CONTROL MENU***";
//	char menu1[20] = "Available User Commands";
	printf("\r\n");
	printf("%s\r\n", "Available User Commands");
	printf("%s\r\n", "NEXT - Show next song info");
	printf("%s\r\n", "PLAY - Play the song (Led On)");
	printf("%s\r\n", "PAUSE - Pause the song (Led flash)");
	printf("%s\r\n", "STOP - Stop the song (Led off)");
}
