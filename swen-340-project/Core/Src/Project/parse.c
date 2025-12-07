#include "parse.h"
#include "printf.h"
#include "stm32l4xx.h"
#include "GPIO.h"

#include <stdint.h>
#include <string.h>

static int byte_skipped_from_delay_check = 0;

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
				unsigned char* n_ptr = (ptr + i);
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

int get_track_length(unsigned char* ptr) {
	ptr = ptr + 4;

	uint32_t value1 =  (unsigned int) ptr[0] << (3*8);	// 0x 78 00 00 00
	uint32_t value2 =  (unsigned int) ptr[1] << (2*8);  	// 0x 00 56 00 00
	uint32_t value3 =  (unsigned int) ptr[2] << (1*8); 	// 0x 00 00 34 00
	uint32_t value4 =  (unsigned int) ptr[3]; 			// 0x 00 00 00 12
	uint32_t result = value1 + value2 + value3 + value4;

	return result;
}

uint32_t get_event_delay (uint8_t* event) {

	int value = 0;
	byte_skipped_from_delay_check = 0;

	//printf(" Start From: %02X \r\n", (unsigned char) *event);
	while (1) {
		unsigned char *current_byte = event;
		value = value << 7;
		value |= (*current_byte & 0x7F);

		//printf("	Current Byte: %02X => %d", *current_byte, value);

		if (!(*event & 0b10000000)) {
			//printf(" End with: %02X \r\n", (unsigned char) *event);
//			printf("	Total value: %d\r\n", value);
			break;
		}
		event++;
		byte_skipped_from_delay_check++;
	}
//	printf("\r\n");
	return value;
}

enum event_types get_event_type(unsigned char* event) {
	unsigned char event_type = (unsigned char) (*event >> 4);

	if (event_type == 0x0A) {
		printf("	%s \r\n", "Polyphonic Key Pressure");
		return pressure;
	}
	else if (event_type == 0x09) {
		printf("	%s \r\n", "Note On Event");
		return note_on;
	}
	else if (event_type == 0x08) {
		printf("	%s \r\n", "Note Off Event");
		return note_off;
	}
	return -1;
}

uint8_t get_event_channel(unsigned char* event) {
	return (uint8_t) (*event & 0x0F);
}



void read_track(unsigned char* ptr) {
	int track_iter_count = 0;
	int length = get_track_length(ptr);

	ptr += 8;		//Skips the MTrk & track length
	//printf(" Start with: %02X \r\n", *ptr);
	while (track_iter_count < length) {

		uint32_t delay = get_event_delay((uint8_t*) ptr);

		enum event_types event_type = get_event_type(ptr);
		uint8_t channel = get_event_channel(ptr);

		if ((event_type >= 0) && (event_type <= 2)) {
			printf(" Found in: %02X | %d \r\n", *ptr, track_iter_count);
			printf(" Delay: %lu \r\n", delay);
			printf(" Event Type: %d \r\n", event_type);
			printf(" Channel: %d \r\n", channel);
			printf("\r\n");

			track_iter_count += byte_skipped_from_delay_check;
			track_iter_count += 3;
		} else {
			//printf("		%02X | %d\r\n", *ptr, track_iter_count);
		}

		//printf(" Iter count: %d \r\n", track_iter_count);
		ptr++;
		track_iter_count++;
	}
	printf(" Ends with: %02X | %d \r\n", *ptr, track_iter_count);
}


