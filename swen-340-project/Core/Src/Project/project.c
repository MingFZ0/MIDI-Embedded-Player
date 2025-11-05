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
static int CURRENT_SONG = -1;
static int STATE_PAUSE = 0;
static int STATE_PLAY = 0;

static struct sys_tick* SYSTCK = (struct sys_tick*) 0xE000E010;
static int TIMER_COUNT = 0;
static int TIME = 0;

static char BUFFER[10];
static int BUFFER_INDEX = 0;

//static int REMOTE_MODE = 1;

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
	printf("%s\r\n", "Available User Commands");
	printf("%s\r\n", "HELP - List out available commands");
	printf("%s\r\n", "NEXT - Show next song info");
	printf("%s\r\n", "PLAY - Play the song (Led On)");
	printf("%s\r\n", "PAUSE - Pause the song (Led flash)");
	printf("%s\r\n", "STOP - Stop the song (Led off)");
}

void run_command(char* buffer) {
	if (strcmp(buffer, "NEXT") == 0) {
		CURRENT_SONG = run_next(CURRENT_SONG);
	}
	else if (strcmp(buffer, "HELP") == 0) {
		display_menu();
	}
	else if (strcmp(buffer, "PLAY") == 0) {
		printf("%s\r\n", "Playing...");
		STATE_PAUSE = 0;
		STATE_PLAY = 1;
		run_play();
	}
	else if (strcmp(buffer, "STOP") == 0) {
		printf("%s\r\n", "Stopping...");
		STATE_PLAY = 0;
		STATE_PAUSE = 0;
		run_stop();
	}
	else if (strcmp(buffer, "PAUSE") == 0) {
		printf("%s\r\n", "Pausing...");
		if (STATE_PAUSE == 0) {
			STATE_PAUSE = 1;
			STATE_PLAY = 0;
		}
	}
	else { printf("%s\r\n", "*Invalid Command");}
	printf("\r\n");
}

void project_init() {
	//Buffer
	clear_buffer(BUFFER, 10);

	//Timer
	SYSTCK->RVR = 8000000;
	SYSTCK->CSR |= 1<<2;
	SYSTCK->CSR |= 1;
}

void run_project() {

	project_init();

	printf("\r\n");
	display_menu();
	printf("\r\n");


	while (1) {
		if (STATE_PAUSE == 1) {
			int re_vars[2] = {0,0};
			run_pause(SYSTCK, TIMER_COUNT, TIME, re_vars);
			TIMER_COUNT = re_vars[0];
			TIME = re_vars[1];
		}

		uint8_t byte = USART_Read_Non_Blocking(USART2);
		if (byte == 0) {continue;}
		putchar(byte);

		if (BUFFER_INDEX < 10) {
			BUFFER[BUFFER_INDEX] = (char) byte;
			BUFFER_INDEX++;
		}

		if (byte == SPACE_KEY) {
			BUFFER[BUFFER_INDEX-1] = '\0';
			//printf("	Buffer Before Cleaning: %s\r\n", buffer);
			run_command(BUFFER);
			clear_buffer(BUFFER, 10);
			BUFFER_INDEX = 0;
			//printf("	Buffer after Cleaning: %s\r\n", buffer);
		}

	}
}




