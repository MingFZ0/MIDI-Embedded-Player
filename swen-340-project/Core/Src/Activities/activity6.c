// Create a struct to represent the SysTick registers and
// print out the value in the CSR register
// (See slides for register information)

#include "printf.h"
#include <stdint.h>

struct sys_tick {
	uint32_t CSR;
	uint32_t RVR;
	uint32_t CVR;
	uint32_t CALIB;
};

void activity6_run() {
	struct sys_tick* systck = (struct sys_tick*) 0xE000E010;
	printf("%lu\r\n", systck->CSR);

	systck->RVR = 8000000;
	systck->CSR != 1<<2;
	systck->CSR |= 1;

	int time = 0;
	int count = 0;
	while (1) {
		if (systck->CSR & (1<<16)) {
			count++;
			if (count == 10) {
				printf("%d\r\n", time);
			}
			time++;
		}
	}

}


