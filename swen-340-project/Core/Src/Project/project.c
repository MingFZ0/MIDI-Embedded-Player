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
#include "tone.h"
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

static int SM_BTN_PRESSED = 0;					// 0 is none | 1 is pressed down | 2 is released
static int SM_BTN_PRESSED_MOMENT = 0;			// Marks the count of the iterator at the time the small button is interacted
static int SM_BTN_PRESSED_ITERATOR = 0;

static int LAST_BTN_DOWN_TIME = 0;
static int LAST_BTN_UP_TIME = 0;
static int BTN_HELD_TIME = 0;

static int LOCAL_MODE_BIN_CONTROL = 0;			// 1 = play/pause	| 2 = next	| 3 = stop

/**
 * This method flips between the operations mode of remote and local
 * Parameter: None
 * Return: void
 */
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

int convert_count_to_ms(int count) {
	return (count / 8000);
}

/**
 * The function that gets called in the button's Interrupt Handler
 * Parameter: None
 * Return: void
 */
void small_button_check() {
	if (REMOTE_MODE == 1) {return;}

	//Button Down
	//Compares the Button press & singles out UP to register once with the iterator being at 0
	if (SM_BTN_PRESSED == 0 && SM_BTN_PRESSED_ITERATOR == 0) {

		if (LOCAL_MODE_BIN_CONTROL == 0) {LOCAL_MODE_BIN_CONTROL = 1;}
		else if ((LOCAL_MODE_BIN_CONTROL == 1)) {LOCAL_MODE_BIN_CONTROL = 2;}

		SM_BTN_PRESSED = 1;										//Sets the state to be DOWN
		SM_BTN_PRESSED_MOMENT = SM_BTN_PRESSED_ITERATOR;
		LAST_BTN_DOWN_TIME = get_count();
		//printf("		Button Down at: %d\r\n", LAST_BTN_DOWN_TIME);
	}

	//Button Up
	//Compares the Button press & singles out DOWN to register once ONLY when state is DOWN
	else if (SM_BTN_PRESSED == 1 && SM_BTN_PRESSED_ITERATOR > 0) {

		SM_BTN_PRESSED = 2;										//Sets the state to be UP
		SM_BTN_PRESSED_MOMENT = SM_BTN_PRESSED_ITERATOR;

		LAST_BTN_UP_TIME = get_count();
		BTN_HELD_TIME = convert_count_to_ms(LAST_BTN_UP_TIME - LAST_BTN_DOWN_TIME);
		if (BTN_HELD_TIME >= 100) {LOCAL_MODE_BIN_CONTROL = 3;}
	}
}

int get_current_mode() {return REMOTE_MODE;}
int get_play_state() {return STATE_PLAY;}
int get_pause_state() {return STATE_PAUSE;}
void set_state_pause() {STATE_PAUSE = 1;STATE_PLAY = 0;}
void set_state_play() {STATE_PAUSE = 0;STATE_PLAY = 1;}
void set_state_clear() {STATE_PAUSE = 0;STATE_PLAY = 0;}

void set_current_song(int song) {CURRENT_SONG = song;}
int get_current_song() {return CURRENT_SONG;}

/**
 * Initization for the project
 * Parameter: None
 * Return: Void
 */
void project_init() {
	GPIO_Init();
	DAC_Init ();
	DAC_Start ();

	//Buffer
	clear_buffer(BUFFER, 10);

	//Timer
	SYSTCK->RVR = 79;
	SYSTCK->CSR |= 1 << 1;
	SYSTCK->CSR |= 1<<2;
	SYSTCK->CSR |= 1;
}

/**
 * The method to run for all of the remote mode functionalities
 * Parameter: None
 * Return: void
 */
void run_remote_op() {

	if (STATE_PAUSE == 1 && STATE_PLAY == 0) {
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

/**
 * The method to run for all of the local mode functionalities
 * Parameter: none
 * Return: void
 */
void run_local_op() {

	//Runs a timer
	time_countdown(SYSTCK, TIMER);

	//This code runs 1 second after the button has been pressed DOWN
	int diff = convert_count_to_ms(get_count() - LAST_BTN_UP_TIME);
	if (diff > 100 && SM_BTN_PRESSED == 0) {
		//printf("%s\r\n", "Stop Listening for second button");

		//Runs the corresponding command based on the button state
		if (LOCAL_MODE_BIN_CONTROL > 0 && LOCAL_MODE_BIN_CONTROL < 3) {
			run_local_cmd(LOCAL_MODE_BIN_CONTROL);
			LOCAL_MODE_BIN_CONTROL = 0;
		}
	}

//	//When the button is pressed down, the iterator will start incrementing
	if (SM_BTN_PRESSED > 0 || SM_BTN_PRESSED_ITERATOR > 0) {

		SM_BTN_PRESSED_ITERATOR++;

		//After the button is released, reset the button state and iterator
		if (SM_BTN_PRESSED == 2 && (SM_BTN_PRESSED_ITERATOR > (SM_BTN_PRESSED_MOMENT + 1))) {
			SM_BTN_PRESSED = 0;
			SM_BTN_PRESSED_ITERATOR = 0;

			//Registers when the button is held for longer than a second and released -> runs the corrsponding command
			if (LOCAL_MODE_BIN_CONTROL == 3) {
				run_local_cmd(LOCAL_MODE_BIN_CONTROL);
				LOCAL_MODE_BIN_CONTROL = 0;
			}
		}
	}
	//Clear the USART Reading
	USART_Read_Non_Blocking(USART2);
}

/**
 * Running the actual project
 * Parameter: None
 * Return: Void
 */
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
