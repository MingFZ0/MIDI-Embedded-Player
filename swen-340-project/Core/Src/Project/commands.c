#include "printf.h"
#include "LED.h"
#include "song.h"

struct sys_tick {
	uint32_t CSR;
	uint32_t RVR;
	uint32_t CVR;
	uint32_t CALIB;
};

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

void print_song_meta_event(unsigned char* p_song, int target_byte) {
	unsigned char* ptr = skip_to_byte(p_song, target_byte);

	if (ptr == 0) {
		printf("%s\n", "N/A");
		return;
	}

	while (1) {
		if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z') || (*ptr == 32)) {putchar(*ptr);}
		if (*ptr == '\0' || *ptr == 255) {break;}
		ptr++;
	}
	printf("\r\n");
}

void run_play() {
	LED_On();
}

void run_stop() {
	LED_Off();
}

int run_next(int current_song) {
	int next_song_index = current_song + 1;
	if (next_song_index > 4) {next_song_index = 0;}
	song song = get_song(next_song_index);
	printf("Song Index: %d\r\n", next_song_index);
	printf("%s", "Title: ");
	print_song_meta_event(song.p_song, 3);
	printf("%s", "Copyright: ");
	print_song_meta_event(song.p_song, 2);
	return next_song_index;
}

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


