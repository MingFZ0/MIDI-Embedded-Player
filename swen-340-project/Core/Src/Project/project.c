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

#include "project_util.h"

//const int SPECIAL_KEY = 27;
const int SPACE_KEY = 13;

//States
static int CURRENT_SONG = -1;	// 	-1 = initialization | 0 = off | 1 = on
static int STATE_PAUSE = 0;
static int STATE_PLAY = 0;

//Project Vars
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

void set_state_pause() {
	STATE_PAUSE = 1;
	STATE_PLAY = 0;
}
void set_state_play() {
	STATE_PAUSE = 0;
	STATE_PLAY = 1;
}
void set_state_clear() {
	STATE_PAUSE = 0;
	STATE_PLAY = 0;
}

void set_current_song(int song) {CURRENT_SONG = song;}
int get_current_song() {return CURRENT_SONG;}

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
