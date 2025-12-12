#include "printf.h"
#include "LED.h"
#include "song.h"
#include "convert_to_uint32.h"
#include "convert_to_uint16.h"
#include "project_util.h"
#include "parse.h"

/**
 * Prints the song event based on the given target_byte
 * Parameter: unsigned char* p_song, int target_byte
 * Return: void
 */
void print_song_meta_event(unsigned char* p_song, int target_byte) {
	unsigned char* ptr = skip_to_byte(p_song, target_byte);

	if (ptr == 0) {
		printf("%s\r\n", "N/A");
		return;
	}

	while (1) {
		if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z') || (*ptr == ' ')) {putchar(*ptr);}
		if (*ptr == '\0' || *ptr == 255) {break;}
		ptr++;
	}
	printf("\r\n");
}

/**
 * Prints the song tempo
 * Parameter: unsigned char* p_song
 * Return: void
 */
void print_song_tempo(unsigned char* p_song) {
	unsigned char* ptr = skip_to_byte(p_song, 81);

	if (ptr == 0) {
		printf("%s\n", "N/A");
		return;
	} else {
		ptr++;
		if (*ptr != 3) {
			printf("%s\n", "N/A");
			return;
		}
		ptr++;
	}

	//0x09,0xa3,0x19
	uint32_t value1 = ptr[0] << (2*8);	// 0x 09 00 00
	uint32_t value2 = ptr[1] << (1*8);  // 0x 00 a3 00
	uint32_t value3 = ptr[2];			// 0x 00 00 19
	uint32_t value4 = value1 + value2 + value3;
	printf("%lu", value4);
	printf("\r\n");
}

//======== Below are all of the commands that makes up the main functionalities of the program ========

/**
 * Run play
 * Turns on the LED
 * Parameter: none
 * Return: void
 */
void run_play() {
	LED_On();

}

/**
 * Run stop
 * Turns off the LED
 * Parameter: none
 * Return: void
 */
void run_stop() {
	LED_Off();
}

/**
 * Displays the next Song
 * Parameter: int current_song -> The index of the given song
 * Return: the next song index
 */
int run_next(int current_song) {
	int next_song_index = current_song + 1;
	if (next_song_index > 4) {next_song_index = 0;}
	song song = get_song(next_song_index);
	printf("Song Index: %d\r\n", next_song_index);

	unsigned char* ptr = song.p_song;
	printf("%s", "Title: ");
	print_song_meta_event(ptr, 3);

	ptr = song.p_song;
	printf("%s", "Copyright: ");
	print_song_meta_event(ptr, 2);

	ptr = song.p_song;
	printf("%s", "Tempo: ");
	print_song_tempo(ptr);

	ptr = song.p_song;
	ptr = skip_to_track(ptr);
//	uint32_t length = get_track_length(ptr);
//	printf("\r\n");
//	printf("%s \r\n", "Byte Info: ");
//	printf("Song length: %d\r\n", length);
	read_track(ptr);

	ptr++;
//	length = get_track_length(ptr);
//	printf("%s \r\n", "Next Track:");
//	printf("%s \r\n", "Byte Info: ");
//	printf("Song length: %d\r\n", length);
	read_track(skip_to_track(ptr));

	return next_song_index;
}

/**
 * OUTDATED! DO NOT USE
 * Runs a count loop using systck to flash the led on and off
 * Parameter: struct sys_tick* systck, int count, int time, int re_vars[]
 * Return: void
 */
void run_pause(struct sys_tick* systck, int count, int time, int re_vars[]) {
	int re_count = count;
	int re_time = time;
	if (systck->CSR & (1<<16)) {
		re_count++;
		if (re_count == 10) { // Count to 10 tenths then print next second
			//printf ("%d\r\n", re_time);
			re_time++;
			re_count = 0;
			if (re_time % 2 == 0) {
				LED_Off();
				//printf("%s\r\n", "Flash off");
			}
			else {
				LED_On();
				//printf("%s\r\n", "Flash on");
			}
		}
	}
	re_vars[0] = re_count;
	re_vars[1] = re_time;
	return;
}

