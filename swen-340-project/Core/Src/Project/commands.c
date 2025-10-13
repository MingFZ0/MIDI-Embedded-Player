#include "printf.h"
#include "LED.h"
#include "song.h"
#include "convert_to_uint32.h"
#include "convert_to_uint16.h"

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

	unsigned char* ptr = song.p_song;
	printf("%s", "Title: ");
	print_song_meta_event(ptr, 3);

	ptr = song.p_song;
	printf("%s", "Copyright: ");
	print_song_meta_event(ptr, 2);

	ptr = song.p_song;
	printf("%s", "Tempo: ");
	print_song_tempo(ptr);
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


