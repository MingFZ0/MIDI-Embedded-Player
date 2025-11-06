/**
 * This file consists of any utility files for the project
 */

#include "GPIO.h"
#include "project.h"
#include "commands.h"
#include "printf.h"
#include "stm32l4xx.h"
#include "LED.h"
#include <string.h>

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

void time_countdown(struct sys_tick* systck, int time_vars[]) {
	if (systck->CSR & (1<<16)) {
		time_vars[1]++;
		if (time_vars[1] == 10) { // Count to 10 tenths then print next second
			time_vars[0]++;
			time_vars[1] = 0;
			if (get_current_mode() == 0 && get_pause_state() == 1) {
				if (time_vars[1] % 2 == 0) {LED_Off();}
				else {LED_On();}
			}
		}
	}
	return;
}

void EXTI15_10_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(B1_Pin);
	flip_operation_mode();
}

void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(S1_Pin);
	small_button_check();
//	printf("%s\r\n", "Small Button Pressed");
}
