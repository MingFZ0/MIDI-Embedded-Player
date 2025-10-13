#include "printf.h"
#include "LED.h"

struct sys_tick {
	uint32_t CSR;
	uint32_t RVR;
	uint32_t CVR;
	uint32_t CALIB;
};

void run_play() {
	LED_On();
}

void run_stop() {
	LED_Off();
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


