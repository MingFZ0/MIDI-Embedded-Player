/*
 * project.c
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#include "UART.h"
#include "printf.h"
#include "UART.h"
#include "activity5.h"
#include <string.h>

#include "project.h"

const int SPACE_KEY = 13;
//const int SPECIAL_KEY = 27;

void clear_buffer(char *buffer, int size) {
	for (int index = 0; index < size; index++) {
		//printf("	Replacement Before: %c\r\n", *buffer);
		*buffer = 0;
		//printf("	Replacement After: %c\r\n", *buffer);
		buffer++;
	}
	size = 0;
}

//int compare_string(char* target, char* expected) {
//	int limit = 20;
//	for (int _ = 0; _ < limit; _++) {
//		printf("	Target: %d\r\n", *target);
//		printf("	Expected: %d\r\n", *expected);
//
//		if (*target == '\0' && *expected == '\0') {
//			break;
//		}
//		if (*target != *expected) {return 0;}
//		target++;
//		expected++;
//	}
//	return 1;
//}

void run_command(char* buffer) {
	if (strcmp(buffer, "NEXT") == 0) {
		printf("%s\r\n", "Run NEXT");
	}
	else if (strcmp(buffer, "PLAY") == 0) {
		printf("%s\r\n", "Run PLAY");
	}
	else if (strcmp(buffer, "PAUSE") == 0) {
		printf("%s\r\n", "Run PAUSE");
	}
	else if (strcmp(buffer, "STOP") == 0) {
		printf("%s\r\n", "Run STOP");
	}
	else { printf("%s\r\n", "*Invalid Command");}
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

void run_project() {
	display_menu();

	char buffer[10];
	int buffer_index = 0;
	clear_buffer(buffer, 10);

	while (1) {
		uint8_t byte = USART_Read_Non_Blocking(USART2);
		if (byte == 0) {continue;}
		putchar(byte);

		if (buffer_index < 10) {
			buffer[buffer_index] = (char) byte;
			buffer_index++;
		}

		if (byte == SPACE_KEY) {
			buffer[buffer_index-1] = '\0';
			//printf("	Buffer Before Cleaning: %s\r\n", buffer);
			run_command(buffer);
			clear_buffer(buffer, 10);
			buffer_index = 0;
			//printf("	Buffer after Cleaning: %s\r\n", buffer);
		}

	}
}




