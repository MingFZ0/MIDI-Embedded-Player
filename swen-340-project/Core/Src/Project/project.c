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
#include "GPIO.h"
#include "stm32l4xx.h"
#include <string.h>

#include "project_util.h"

//const int SPECIAL_KEY = 27;
const int SPACE_KEY = 13;

//States
static int REMOTE_MODE = 1;
static int CURRENT_SONG = -1;	// 	-1 = initialization | 0 = off | 1 = on
static int STATE_PAUSE = 0;
static int STATE_PLAY = 0;

//Project Vars
static struct sys_tick* SYSTCK = (struct sys_tick*) 0xE000E010;
static int TIMER_COUNT = 0;
static int TIME = 0;

static char BUFFER[10];
static int BUFFER_INDEX = 0;

static int SM_BTN_PRESSED = 0;
static int SM_BTN_PRESSED_TIMER_COUNT = 0;
static int SM_BTN_PRESSED_TIME = 0;

struct sys_tick {
	uint32_t CSR;
	uint32_t RVR;
	uint32_t CVR;
	uint32_t CALIB;
};

void flip_operation_mode() {
	if (REMOTE_MODE == 0) {
		REMOTE_MODE = 1;
		printf("\r\n%s\r\n", "***REMOTE MODE ACTIVE***");
	}
	else {
		REMOTE_MODE = 0;
		printf("\r\n%s\r\n", "***LOCAL MODE ACTIVE***");
	}
}

void small_button_check() {
	if (SM_BTN_PRESSED == 0 && TIME == 0 && TIMER_COUNT == 0) {
		SM_BTN_PRESSED = 1;
		SM_BTN_PRESSED_TIMER_COUNT = TIMER_COUNT;
		SM_BTN_PRESSED_TIME = TIME;
		printf("Init Timer Count: %d | Init Time: %d\r\n", TIMER_COUNT, TIME);
	}
	else if (SM_BTN_PRESSED == 1 && TIMER_COUNT > SM_BTN_PRESSED_TIMER_COUNT) {
		SM_BTN_PRESSED = 0;
		printf("Final Timer Count: %d | Final Time: %d\r\n", TIMER_COUNT, TIME);
	}
//
//	printf("Pressed\r\n");
}

int get_current_mode() {return REMOTE_MODE;}
void set_state_pause() {STATE_PAUSE = 1;STATE_PLAY = 0;}
void set_state_play() {STATE_PAUSE = 0;STATE_PLAY = 1;}
void set_state_clear() {STATE_PAUSE = 0;STATE_PLAY = 0;}

void set_current_song(int song) {CURRENT_SONG = song;}
int get_current_song() {return CURRENT_SONG;}

void project_init() {
	GPIO_Init();

	//Buffer
	clear_buffer(BUFFER, 10);

	//Timer
	SYSTCK->RVR = 8000000;
	SYSTCK->CSR |= 1<<2;
	SYSTCK->CSR |= 1;
}

void run_remote_op(uint8_t byte) {
	putchar(byte);

	if (BUFFER_INDEX < 10) {
		BUFFER[BUFFER_INDEX] = (char) byte;
		BUFFER_INDEX++;
	}

	if (byte == SPACE_KEY) {
		BUFFER[BUFFER_INDEX-1] = '\0';
		run_command(BUFFER);
		clear_buffer(BUFFER, 10);
		BUFFER_INDEX = 0;
	}
}

void run_local_op() {

}

void run_project() {

	project_init();

	printf("%s\r\n", "***REMOTE MODE ACTIVE***");
	printf("\r\n");
	display_menu();
	printf("\r\n");

	while (1) {

		if ((SM_BTN_PRESSED == 1) || (TIME > 0) || (TIMER_COUNT > 0)) {
			int re_vars[2] = {0,0};
			time_countdown(SYSTCK, TIMER_COUNT, TIME, re_vars);
			TIMER_COUNT = re_vars[0];
			TIME = re_vars[1];

			printf("%d | %d\r\n", TIME, TIMER_COUNT);

			if () {
				SM_BTN_PRESSED_TIME = 0;
				SM_BTN_PRESSED_TIMER_COUNT = 0;
				TIMER_COUNT = 0;
				TIME = 0;
			}
		}

		if (STATE_PAUSE == 1) {
			int re_vars[2] = {0,0};
			run_pause(SYSTCK, TIMER_COUNT, TIME, re_vars);
			TIMER_COUNT = re_vars[0];
			TIME = re_vars[1];
		}

		uint8_t byte = USART_Read_Non_Blocking(USART2);
		if (byte == 0) {continue;}
		if (REMOTE_MODE == 1) {
			run_remote_op(byte);
		}
		else {run_local_op();}
	}
}
