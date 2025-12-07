#include "printf.h"
#include "stm32l4xx.h"
#include "GPIO.h"

#include <stdint.h>
#include <string.h>

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

void read_track(unsigned char* ptr) {
	int length = get_track_length(ptr);

	unsigned char* track = ptr + 8;		//Skips the MTrk & track length
	for (int i = 0; i< length; i++) {

		unsigned char event = (unsigned char) (*track >> 4);
		unsigned char channel = (unsigned char) (*track & 0x0F);

		printf("	Before: %02X => %02X | %02X ", *track, event, channel);

		if (event == 0x0A) {
			printf("	%s \r\n", "Polyphonic Key Pressure");
		}
		else if (event == 0x09) {
			printf("	%s \r\n", "Note On Event");
		}
		else if (event == 0x08) {
			printf("	%s \r\n", "Note Off Event");
		}
		else {
			printf("\r\n");
		}

		track++;
	}
}
