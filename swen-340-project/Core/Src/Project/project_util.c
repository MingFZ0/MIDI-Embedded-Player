/**
 * This file consists of any utility files for the project
 */

#include "GPIO.h"
#include "project.h"
#include "commands.h"
#include "printf.h"
#include "stm32l4xx.h"
#include "LED.h"
#include "convert_to_uint32.h"
#include <string.h>

static int count = 0;

/**
 * Handler for the blue button
 */
void EXTI15_10_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(B1_Pin);
	flip_operation_mode();
}

/**
 * Handler for the small button
 */
void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(S1_Pin);
	small_button_check();
//	printf("%s\r\n", "Small Button Pressed");
}

void SysTick_Handler() {
	count++;
}

uint32_t get_count() {return count;}

/**
 * This method clears the buffer that is passed in
 * Parameter: char *buffer, int size
 * Return: void
 */
void clear_buffer(char *buffer, int size) {
	for (int index = 0; index < size; index++) {
		//printf("	Replacement Before: %c\r\n", *buffer);
		*buffer = 0;
		//printf("	Replacement After: %c\r\n", *buffer);
		buffer++;
	}
	size = 0;
}

/**
 *  Displays the menu
 *  Parameter: None
 *  Return: void
 */
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

/**
 * Runs the local command based on the state passed in
 * Parameter: int state | 1 = play/pause ; 2 = next ; 3 = stop
 * Return: void
 */
void run_local_cmd(int state) {
//	printf("Playing State: %d\r\n", state);
	if (state == 1) {
		if (get_play_state() == 0) {
			printf("%s\r\n", "Playing...");
			set_state_play();
			run_play();
		}
		else {
			printf("%s\r\n", "Pausing...");
			set_state_pause();
		}
	}
	else if (state == 2) {
		int new_song = run_next(get_current_song());
		set_current_song(new_song);
	}
	else if (state == 3) {
		printf("%s\r\n", "Stopping...");
		set_state_clear();
		run_stop();
	}
}

/**
 * Runs the remote commands based on the buffer passed in
 * Paramter: char* buffer
 * Return: void
 */
void run_command(char* buffer) {
	if (strcmp(buffer, "NEXT") == 0) {
		int new_song = run_next(get_current_song());
		set_current_song(new_song);
	}
	else if (strcmp(buffer, "HELP") == 0) {
		display_menu();
	}
	else if (strcmp(buffer, "PLAY") == 0) {
		printf("%s\r\n", "Playing...");
		set_state_play();
		run_play();
	}
	else if (strcmp(buffer, "STOP") == 0) {
		printf("%s\r\n", "Stopping...");
		set_state_clear();
		run_stop();
	}
	else if (strcmp(buffer, "PAUSE") == 0) {
		printf("%s\r\n", "Pausing...");
		set_state_pause();
	}
	else { printf("%s\r\n", "*Invalid Command");}
	printf("\r\n");
}

/**
 * A condensed & updated version of run_pause in the commands.c file
 * Runs a countdown using systck and displays the led based on the state
 * Parameter: struct sys_tick* systck, int time_vars[]	| time_vars is a 2d array consist of {time, time_count} which corrsponds to {second, 10th of a second}
 * Return: void -> the values are modified and saved in the time_vars
 */
void time_countdown(struct sys_tick* systck, int time_vars[]) {
	if (systck->CSR & (1<<16)) {
		time_vars[1]++;
		if (time_vars[1] == 300000) { // Count to 10 tenths then print next second
			time_vars[0]++;
			time_vars[1] = 0;
			if (get_pause_state() == 1) {
				if (time_vars[0] % 2 == 0) {
					LED_Off();
				}
				else {
					LED_On();
				}
			}
		}
	}
	return;
}

/**
 * Helper function used to skip to a certain byte; Useful for parsing song information based on the leading byte
 * Parameter: unsigned char* ptr -> the var type for the song, int byte -> the byte that needs to be looked for
 * Return: void
 */
unsigned char* skip_to_byte(unsigned char* ptr, int byte) {
	int counter = 0;
	while (1) {
		if (*ptr == 255) {
			unsigned char* p_next = ptr + 1;
			if (*p_next == byte) {
				//printf("ptr is at %d | next is at %d \r\n", *ptr, *p_next);
				ptr++;
				return ptr;
			}
		}
		if (counter > 100) {
			return 0;
		}
		counter++;
		ptr++;
	}
}

unsigned char* skip_to_track(unsigned char* ptr) {
	int counter = 0;
	char bytes[] = {0x4D,0x54,0x72,0x6B};
	while (1) {
		if (*ptr == bytes[0]) {
			for (int i = 0; i < 4; i++) {
				char* n_ptr = (ptr + i);
				if (*n_ptr != bytes[i]) {break;}
				else {
//					printf("	Byte: %02X\r\n", *n_ptr);
					if (i == 3) {return ptr;}
				}
			}
		}
		if (counter > 100) {return 0;}
		ptr++;
	}
}

unsigned int get_track_length(unsigned char* ptr) {
	unsigned char* track = ptr + 4;
	printf("	Byte: %02X\r\n", *track);

	uint32_t value1 =  (unsigned int) track[0] << (3*8);	// 0x 78 00 00 00
	uint32_t value2 =  (unsigned int) track[1] << (2*8);  	// 0x 00 56 00 00
	uint32_t value3 =  (unsigned int) track[2] << (1*8); 	// 0x 00 00 34 00
	uint32_t value4 =  (unsigned int) track[3]; 			// 0x 00 00 00 12
	uint32_t result = value1 + value2 + value3 + value4;

	return result;
}


