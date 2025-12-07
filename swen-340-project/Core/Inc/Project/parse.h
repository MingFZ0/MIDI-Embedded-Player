#ifndef INC_PARSE_H_
#define INC_PARSE_H_

#include <stdint.h>
#include "stm32l4xx.h"

enum event_types {
	note_off,
	note_on,
	pressure
};

struct event {
	uint8_t channel;
	enum event_types event_type;
	uint8_t note;
	uint32_t delay;
	uint32_t value;
};

unsigned char* skip_to_byte(unsigned char* ptr, int byte);
unsigned char* skip_to_track(unsigned char* ptr);
int get_track_length(unsigned char* ptr);
void read_track(unsigned char* ptr);

#endif
