#ifndef INC_PARSE_H_
#define INC_PARSE_H_

#include <stdint.h>
#include "stm32l4xx.h"

enum event_types {
	note_off,
	note_on,
	pressure,
	ones
};

struct midi_header {
	char chunk_type [4];
	unsigned int length;
	short format;
	unsigned short ntrks;
	short division;
};

struct event {
	uint32_t delay;
	enum event_types event_type;
	uint8_t channel;
	uint8_t note;
	uint8_t value;
};

unsigned char* skip_to_byte(unsigned char* ptr, int byte);
unsigned char* skip_to_track(unsigned char* ptr);
uint32_t get_track_length(unsigned char* ptr);
void read_track(unsigned char* ptr);

#endif
