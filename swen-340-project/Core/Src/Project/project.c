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
#include "commands.h"
#include <string.h>

#include "project.h"

const int SPACE_KEY = 13;
//const int SPECIAL_KEY = 27;

//States
// -1 = initaization, 0 = off, 1 = on
int state_pause = 0;
int state_pause_flash = 0;
int state_play = 0;


struct sys_tick {
	uint32_t CSR;
	uint32_t RVR;
	uint32_t CVR;
	uint32_t CALIB;
};

void clear_buffer(char *buffer, int size) {
	for (int index = 0; index < size; index++) {
		//printf("	Replacement Before: %c\r\n", *buffer);
		*buffer = 0;
		//printf("	Replacement After: %c\r\n", *buffer);
		buffer++;
	}
	size = 0;
}

void display_menu() {
//	char menu[20] = "***REMOTE LED CONTROL MENU***";
//	char menu1[20] = "Available User Commands";
	printf("\r\n");
	printf("%s\r\n", "Available User Commands");
	printf("%s\r\n", "HELP - List out available commands");
	printf("%s\r\n", "NEXT - Show next song info");
	printf("%s\r\n", "PLAY - Play the song (Led On)");
	printf("%s\r\n", "PAUSE - Pause the song (Led flash)");
	printf("%s\r\n", "STOP - Stop the song (Led off)");
}

void run_command(char* buffer) {
	if (strcmp(buffer, "NEXT") == 0) {
		printf("%s\r\n", "Run NEXT");
	}
	else if (strcmp(buffer, "HELP") == 0) {
		display_menu();
	}
	else if (strcmp(buffer, "PLAY") == 0) {
		state_pause = 0;
		state_play = 1;
		run_play();
	}
	else if (strcmp(buffer, "STOP") == 0) {
		state_play = 0;
		state_pause = 0;
		run_stop();
	}
	else if (strcmp(buffer, "PAUSE") == 0) {
		if (state_pause == 0) {
			state_pause = 1;
			state_play = 0;
		}
	}
	else { printf("%s\r\n", "*Invalid Command");}
}

void run_project() {
	display_menu();

	//Buffer
	char buffer[10];
	int buffer_index = 0;
	clear_buffer(buffer, 10);

	//Timer
	struct sys_tick* systck = (struct sys_tick*) 0xE000E010;
	int timer_count = 0;
	int time = 0;
	systck->RVR = 8000000;
	systck->CSR |= 1<<2;
	systck->CSR |= 1;


	while (1) {
		if (state_pause == 1) {
			int re_vars[2] = {0,0};
			run_pause(systck, timer_count, time, re_vars);
			timer_count = re_vars[0];
			time = re_vars[1];
		}

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




