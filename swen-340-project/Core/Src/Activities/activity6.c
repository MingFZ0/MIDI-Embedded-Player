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

	    int time = 0;
	    // Make a clock that counts up once a second.
	    // Set the RVR
	    systck->RVR = 8000000; // Set RVR to 1/10th of a second
	    // Configure CSR to run.
	    systck->CSR |= 1<<2;   // Use internal clock
	    systck->CSR |= 1;      // Turn on systick timer

	    // loop
	    int count = 0;
	    while (1) {
	        //check if timer got to 0
	        if (systck->CSR & (1<<16)) {
	            count++;
	            if (count == 10) { // Count to 10 tenths then print next second
	                printf ("%d\r\n", time);
	                time++;
	                count = 0;
	            }

	        }
	    }

}


