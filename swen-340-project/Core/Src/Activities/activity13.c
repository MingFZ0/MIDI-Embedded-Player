#include <stdint.h>
#include "systick.h"
#include "tone.h"
#include "printf.h"

void play_freq(uint16_t hz) {
	uint32_t rollover = hertz_to_systicks (hz);
	if ((get_count() % rollover) < (rollover >> 1)) {
//		printf("1");
		DAC_Set_Value (3000);
	}
	else {
//		printf("0");
		DAC_Set_Value (0);
	}
}

void activity_13() {
	init_systick();
	DAC_Init ();
	DAC_Start ();
	while (1) {
		play_freq(440);
	}
}
