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
//static int TIMER_COUNT = 0;
//static int TIME = 0;

static int TIMER[2] = {0,0};

static char BUFFER[10];
static int BUFFER_INDEX = 0;

static int SM_BTN_PRESSED = 0;
static int SM_BTN_PRESSED_MOMENT = 0;
static int SM_BTN_PRESSED_ITERATOR = 0;

static int SM_BTN_DOWN_TIMESTAMP[2] = {0,0};
static int SM_BTN_UP_TIMESTAMP[2] = {0,0};

static int SM_BTN_HELD_TIME = 0;

static int LOCAL_MODE_BIN_CONTROL = 0;	// 1 = play/pause	| 2 = next	| 3 = stop


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

void record_timestamp(int time_stamp[2], int timer[2]) {
	time_stamp[0] = timer[0];
	time_stamp[1] = timer[1];
}
int timestamp_diff(int timestamp1[2], int timestamp2[2]) {
	int stamp1_count = (timestamp1[0] * 10) + (timestamp1[1]);
	int stamp2_count = (timestamp2[0] * 10) + (timestamp2[1]);
	int result = stamp2_count - stamp1_count;
//	if (result > 10) {printf("%d - %d = %d\r\n", stamp2_count, stamp1_count, result);}
	return result;
}

void small_button_check() {
	if (REMOTE_MODE == 1) {return;}
	if (SM_BTN_PRESSED == 0 && SM_BTN_PRESSED_ITERATOR == 0) {

		if (LOCAL_MODE_BIN_CONTROL == 0) {LOCAL_MODE_BIN_CONTROL = 1;}
		else if (LOCAL_MODE_BIN_CONTROL == 1) {LOCAL_MODE_BIN_CONTROL = 2;}

		SM_BTN_PRESSED = 1;
		SM_BTN_PRESSED_MOMENT = SM_BTN_PRESSED_ITERATOR;
		record_timestamp(SM_BTN_DOWN_TIMESTAMP, TIMER);
		SM_BTN_DOWN_TIMESTAMP[1]++;
//		printf("		Button Down at: %d, %d\r\n", SM_BTN_DOWN_TIMESTAMP[0], SM_BTN_DOWN_TIMESTAMP[1]);
	}
	else if (SM_BTN_PRESSED == 1 && SM_BTN_PRESSED_ITERATOR > 0) {
		SM_BTN_PRESSED = 2;
		record_timestamp(SM_BTN_UP_TIMESTAMP, TIMER);
		SM_BTN_PRESSED_MOMENT = SM_BTN_PRESSED_ITERATOR;
		SM_BTN_HELD_TIME = timestamp_diff(SM_BTN_DOWN_TIMESTAMP, SM_BTN_UP_TIMESTAMP);

//		printf("		Button Up at: %d, %d\r\n", SM_BTN_UP_TIMESTAMP[0], SM_BTN_UP_TIMESTAMP[1]);
//		printf("		Button Held For: %d\r\n", SM_BTN_DOWN_TIMESTAMP);

		if (SM_BTN_HELD_TIME >= 10) {
			LOCAL_MODE_BIN_CONTROL = 3;
		}


	}
}



int get_current_mode() {return REMOTE_MODE;}
int get_pause_state() {return STATE_PAUSE;}
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
	SYSTCK->RVR = 8000000 - 1;
	SYSTCK->CSR |= 1<<2;
	SYSTCK->CSR |= 1;
}

void run_remote_op() {

	if (STATE_PAUSE == 1) {
		time_countdown(SYSTCK, TIMER);
	}

	uint8_t byte = USART_Read_Non_Blocking(USART2);
	if (byte == 0) {return;}
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

	time_countdown(SYSTCK, TIMER);
	if (timestamp_diff(SM_BTN_DOWN_TIMESTAMP,TIMER) > 11 && SM_BTN_PRESSED == 0) {
//		printf("%s\r\n", "Stop Listening for second button");
		SM_BTN_DOWN_TIMESTAMP[0] = 0;
		SM_BTN_DOWN_TIMESTAMP[1] = 0;
//		TIMER[0] = 0;
//		TIMER[1] = 0;
		if (LOCAL_MODE_BIN_CONTROL > 0 && LOCAL_MODE_BIN_CONTROL < 3) {
			run_local_cmd(LOCAL_MODE_BIN_CONTROL);
			LOCAL_MODE_BIN_CONTROL = 0;
		}
	}

	if (SM_BTN_PRESSED > 0 || SM_BTN_PRESSED_ITERATOR > 0) {

		SM_BTN_PRESSED_ITERATOR++;

		if (SM_BTN_PRESSED == 2 && (SM_BTN_PRESSED_ITERATOR > (SM_BTN_PRESSED_MOMENT + 1))) {
			SM_BTN_PRESSED = 0;
			SM_BTN_PRESSED_ITERATOR = 0;

			if (LOCAL_MODE_BIN_CONTROL == 3) {
				run_local_cmd(LOCAL_MODE_BIN_CONTROL);
				LOCAL_MODE_BIN_CONTROL = 0;
			}
		}
	}



	//Clear the USART Reading
	USART_Read_Non_Blocking(USART2);
}

void run_project() {

	project_init();

	printf("%s\r\n", "***REMOTE MODE ACTIVE***");
	printf("\r\n");
	display_menu();
	printf("\r\n");

	while (1) {

		if (REMOTE_MODE == 1) {
			run_remote_op();
		}
		else {run_local_op();}
	}
}
