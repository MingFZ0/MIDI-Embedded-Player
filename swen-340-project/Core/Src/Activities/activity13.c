#include <stdint.h>
#include "systick.h"
#include "tone.h"
#include "printf.h"

void play_freq(uint8_t hz) {
	uint32_t rollover = hertz_to_systicks (hz);
	if ((get_count() % rollover) < (rollover >> 1)) {
		printf("1");
	}
	else {
		printf("0");
	}
}

void activity_13() {
	init_systick();
	while (1) {
		play_freq(1);
	}
}
