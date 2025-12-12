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

/**
 * Skips the pointer to the following track in the song
 * Return: The moved pointer
 */
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
		counter++;
		ptr++;
	}
}

/**
 * Get the length of the track
 */
uint32_t get_track_length(unsigned char* ptr) {
	ptr = ptr + 4;
//	printf("%02X %02X %02X %02X \r\n", ptr[0], ptr[1], ptr[2], ptr[3]);

	uint32_t value1 =  ((unsigned int) ptr[0]) << (3*8);	// 0x 78 00 00 00
	uint32_t value2 =  ((unsigned int) ptr[1]) << (2*8);  	// 0x 00 56 00 00
	uint32_t value3 =  ((unsigned int) ptr[2]) << (1*8); 	// 0x 00 00 34 00
	uint32_t value4 =  (unsigned int) ptr[3]; 			// 0x 00 00 00 12
	uint32_t result = value1 + value2 + value3 + value4;

	return result;
}

/**
 * Get the event type of the specific event
 */
enum event_types get_event_type(unsigned char* event) {
	unsigned char event_type = (unsigned char) (*event >> 4);
	if (event_type == 0x0A) {return pressure;}
	else if (event_type == 0x09) {return note_on;}
	else if (event_type == 0x08) {return note_off;}
	else if ((event_type == 0x0C) || (event_type == 0x0D)) {return ones;}
	return -1;
}

/**
 * Get the channel of the specific event
 */
uint8_t get_event_channel(unsigned char* event) {
	return (uint8_t) (*event & 0x0F);
}

/**
 * Get the delay of the specific event
 */
uint32_t get_event_delay (uint8_t* event) {

	int value = 0;
	byte_skipped_from_delay_check = 1;
//	unsigned char start_byte = *event;

	//printf(" Start From: %02X \r\n", (unsigned char) *event);
	while (1) {
		unsigned char *current_byte = event;
		value = value << 7;
		value |= (*current_byte & 0x7F);

//		printf("%02X ", *current_byte);
//		printf("	Current Byte: %02X => %d", *current_byte, value);

		if (!(*event & 0b10000000)) {
//			printf(" End with: %02X \r\n", (unsigned char) *event);
			//printf("	Total value: %d\r\n", value);
			break;
		}
		event++;
		byte_skipped_from_delay_check++;
	}

//	printf("	- Delay: %d \r\n", value);

	return value;
}

/**
 * Parse the meta event length
 */
int parse_meta_length(unsigned char* ptr) {

	unsigned char* next = ptr + 1;
//	printf("	ME at %02X of type %02X \r\n", *ptr, *next);

	if (*(next) == 0x00) {
		return 3;
	}
	else if ((*(next) >= 0x01) && (*(next) <= 0x07)) {
		ptr += 2;
		int delay = get_event_delay((uint8_t*) ptr);
		return delay + 4;
	}
	else if ((*next) == 0x20) {
		return 4;
	}
	else if ((*next) == 0x2f) {
		return 3;
	}
	else if ((*next) == 0x51) {
		return 7;
	}
	else if ((*next) == 0x54) {
		return 8;
	}
	else if ((*next) == 0x58) {
		return 8;
	}
	else if ((*next) == 0x59) {
		return 6;
	}
	else if ((*next) == 0x7f) {
		ptr += 2;
		int delay = get_event_delay((uint8_t*) ptr);

		return delay + 2;
	}
	else {
//		printf("%s \r\n", "Not a meta event");
		return 1;
	}
}

/**
 * Reads the track and parse out each event
 */
void read_track(unsigned char* ptr) {

	int track_iter_count = 0;
	int length = get_track_length(ptr);

	ptr += 8;		//Skips the MTrk & track length
	//printf(" Start with: %02X \r\n", *ptr);

	while (track_iter_count < length) {

//		printf(" -> %02X \r\n", *ptr);

		if ((*ptr == 0x00) && (*(ptr + 1) == 0xff)) {

		}

		else if (*ptr == 0xff) {							// Check if it is a meta event & skip it
			int length = parse_meta_length(ptr);
//			printf("	- Length: %d \r\n", length);
			length--;

			for (int i =0; i < length; i++) {
//				printf("%02X ", *ptr);
				ptr++;
				track_iter_count++;
			}
			ptr--;
//			printf("	<- %02X \r\n", *ptr);
		}
		else {												// Check if it is a MIDI event and parse the information

			uint32_t delay = get_event_delay((uint8_t*) ptr);
			ptr+=byte_skipped_from_delay_check;

			enum event_types event_type = get_event_type(ptr);
			uint8_t channel = get_event_channel(ptr);
			ptr++;

			if ((event_type != note_on) && (event_type != note_off)) {		//Ignore all event if it is not note on or note off
				if (event_type == ones) {
					track_iter_count++;
//					printf("%02X ", *ptr);
				}
				else {
//					printf("%02X %02X", *(ptr), *(ptr+1));
					ptr++;
					track_iter_count += 2;
				}
			}
			else {
				ptr++;

				uint8_t velocity = (uint8_t) *ptr;

//				printf(" 	Delay: %lu \r\n", delay);
//				printf(" 	Event Type: %d \r\n", (signed int) event_type);
//				printf(" 	Channel: %d \r\n", channel);
//				printf(" 	Velocity: %d r\n", velocity);
//				printf("%s \r\n", ".");



				track_iter_count += byte_skipped_from_delay_check + 1;
			}
		}

		ptr++;
		track_iter_count++;
	}
}


