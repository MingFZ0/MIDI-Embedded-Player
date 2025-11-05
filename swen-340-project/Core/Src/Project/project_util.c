/**
 * This file consists of any utility files for the project
 */

#include "project.h"
#include "commands.h"
#include "printf.h"
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
